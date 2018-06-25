/* --------------------------------------------------------------------
 * Complex setup FW WRITE test.   
 *
 * Created by V. Piperaki, A. Mouzakitis and M.D Grammatikakis
 *
 * TEI of Crete <dreams@cs.teicrete.gr>
 *
 * 2017 - 2018 
 *
 * 1. Complex Setup of NoC Firewall (setupCFW) for each output port, 
 *    by specifying deny read, deny write, deny read & write or accept all.
 *     This gives a total of 4^4 = 256 different combinations.
 *      - setupCFW(1, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO);
 *	- setupCFW(2, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO);
 *	- setupCFW(3, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO);
 *	- setupCFW(4, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO);
 * 2. Perform direct write (accessBram) to each BRAM (at a specific
 *    offset with a default_written_value) from each input port
 *    and subsequently verify with subsequent direct read from BRAM 
 *    that data (with a read value) has indeed been written.
 *    Assuming testing for a single BRAM offset, the total number of 
 *    (orthogonal) cases corresponding to input/output port
 *    combinations are 4 x 4 = 16, raising the total number of unique cases
 *     to 16 x 256 = 4096 wich 1024 are for WRITE TEST.
 *      - rc1 = accessBram(0x0, BRAM_NO, 0x00000000, &default_written_value)
 *      - rc2 = accessBram(0x1, BRAM_NO, 0x00000000, &read_value); 
 * 3. Write BRAM memory location via NoC Firewall with a different (always increasing) write_value.
 *       - rc3 = cccessBramFW(0x0, inport, outport, 0x0, &write_value);       
 * 4. Read the BRAM value directly (actual_value). The read operation examines
 * the return code from previous write operation 
 *    to avoid the possibility of a write/read reorder.  
 *      - rc4 = accessBram(0x1, BRAM_NO, 0x00000000, &actual_value);
 * 5. Finally, the check_write function is called with the complete setup pattern, 
 * write_value and actual_value.
 *    For each port, the above Write Test is passes if and only if:
 *      - values write_value and actual_value are equal and the setup pattern for that port is 
 *    	  deny read (accept write) or accept  all, or 
 *      - values write_value and actual_value are non-equal and setup pattern for that port is
	   deny write (accept read) or deny all. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h> 
#include "user_mode.h"

#define BRAM_NO 1 // FIXED THROUGHOUT THE TEST

/* this is pass rule, w -> w will passa */
enum rule {w = 0x90, r = 0xA0, wr = 0x80, none = 0xB0};
enum rule pattern[4], patt;

const char *getPatternName(enum rule pattern){
	switch(pattern){
		case w: return "w";
		case r: return "r";
		case wr: return "wr";
		case none: return "none";
	}
}

/*  Function check write */
void check_write(unsigned int input_port, enum rule pattern, 
                 unsigned int actual_value, unsigned int write_value) {
switch(pattern){
        case w:
                if(write_value == actual_value)
		   printf("_Input_Port:%d  Accept:W  -> Rule 'W' PASS (write_value, actual_value):(%x :%x) \n", input_port, write_value, actual_value);
		else
		   printf("_Input_Port:%d  Accept:W  -> Rule 'W' ________________FAIL (write_value, actual_value):(%x %x)\n\n", input_port, write_value, actual_value);

		break;
        case r:
                if(write_value != actual_value)
		   printf("_Input_Port:%d  Accept:R  -> Rule 'W' PASS (write_value, actual_value):(%x :%x) \n", input_port, write_value, actual_value);
		else
		   printf("_Input_Port:%d  Accept:R  -> Rule 'W' ________________FAIL (write_value, actual_value):(%x %x)\n\n", input_port, write_value, actual_value);

		break;
        case wr:
                if(write_value == actual_value)
		   printf("_Input_Port:%d  Accept:WR -> Rule 'W' PASS (write_value, actual_value):(%x :%x) \n", input_port, write_value, actual_value);
		else
		   printf("_Input_Port:%d  Accept:WR -> Rule 'W' ________________FAIL (write_value, actual_value):(%x %x)\n\n", input_port, write_value, actual_value);

		break;
        case none: // deny all
                if(write_value != actual_value)
		   printf("_Input_Port:%d  Accept:-  -> Rule 'W' PASS (write_value, actual_value):(%x :%x) \n", input_port, write_value, actual_value);
		else
		   printf("_Input_Port:%d  Accept:-  -> Rule 'W' ________________FAIL (write_value, actual_value):(%x %x)\n\n", input_port, write_value, actual_value);

		break;
    }

}


int main(int argc, char *argv[])
{
unsigned int rule;
int i, j, k, l, m = 0;
unsigned int wBit, rBit = 0;
volatile int readVal = 0, writeVal = 0;
volatile int rc1 = 0, rc2 = 0, rc3 = 0;
volatile int rc4 = 0; 

unsigned int default_written_value = (unsigned int) 0; // written directly to BRAM
unsigned int write_value   = (unsigned int) 1; // written via FW
unsigned int actual_value   = (unsigned int) 65535;
unsigned int read_value = (unsigned int) 65534;

int count = 0;

pattern[3] = w;
pattern[2] = r;
pattern[1] = wr;
pattern[0] = none;

  for (i=0; i<4; i++){  
    for (j=0; j<4; j++){
      for (k=0; k<4; k++){
        for (l=0; l<4; l++){

	printf("\n##################################################\n");
	printf("_###### PORT 1 ## PORT 2 ## PORT 3 ## PORT 4 ######\n");
	printf("_######   %s    ##   %s    ##   %s    ##   %s    ######\n", getPatternName(pattern[i]), getPatternName(pattern[j]), getPatternName(pattern[k]), getPatternName(pattern[l]));

	/* Port 1: Get write and read bit from rule for this port and setup FW on this port */
	rule = pattern[i]; //pattern for all
	wBit = (rule & (1 << 5)) >> 5;
	rBit = (rule & (1 << 4)) >> 4;
	setupCFW(1, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO); // also check [0x0, 0x80] and [0x80, 0x100] -> ECG scenario

	/* Port 2: Get write and read bit from rule for this port and setup FW on this port */
	rule = pattern[j]; //pattern for all
	wBit = (rule & (1 << 5)) >> 5;
	rBit = (rule & (1 << 4)) >> 4;
	setupCFW(2, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO); // also check [0x0, 0x80] and [0x80, 0x100] -> ECG scenario

	/* Port 3: Get write and read bit from rule for this port and setup FW on this port */
	rule = pattern[k]; //pattern for all
	wBit = (rule & (1 << 5)) >> 5;
	rBit = (rule & (1 << 4)) >> 4;
	setupCFW(3, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO); // also check [0x0, 0x80] and [0x80, 0x100] -> ECG scenario

	/* Port 4: Get write and read bit from rule for this port and setup FW on this port */
	rule = pattern[l]; //pattern for all
	wBit = (rule & (1 << 5)) >> 5;
	rBit = (rule & (1 << 4)) >> 4;
	setupCFW(4, 0x00000000, 0x00000100, 1, wBit, rBit, BRAM_NO); // also check [0x0, 0x80] and [0x80, 0x100] -> ECG scenario

	for(m=1; m<5; m++){
	        if (m==1)
	        patt = pattern [i];
	        else if (m==2)
	        patt = pattern [j];
	        else if (m==3)
	        patt = pattern [k];
	        else
	        patt = pattern [l];

               /* 1. Direct default_written_value to each BRAM */ 
                rc1 = accessBram(0x0, BRAM_NO, 0x00000000, &default_written_value); 
		do {
		/* 2. Direct Read read_value to each BRAM  */
                	rc2 = accessBram(0x1, BRAM_NO, 0x00000000, &read_value); 
			count++;
		} while (read_value != default_written_value);
		/* 3. Wrie write_value o BRAM memory */
		rc3 = accessBramFW(0x0, m, BRAM_NO, 0x00000000, &write_value); 
		/* 4. Read actual_value from BRAM memory */
                if (rc3 == 0) 
                rc4 = accessBram(0x1, BRAM_NO, 0x00000000, &actual_value); 

		printf("______________write_value:%d actual_value:%d \n", write_value, actual_value);

		check_write(m, patt, actual_value, write_value);
		actual_value   = (unsigned int) 65535;
                write_value = write_value + (unsigned int) 1;

	}
	printf("_##################################################\n\n");

       }
     }
   }
 }
        exit(0);
}

