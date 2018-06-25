/* --------------------------------------------------------------------

 * Simple setup FW READ test.   
 *
 * Created by V. Piperaki, A. Mouzakitis and M.D Grammaikakis
 *
 * TEI of Crete <dreams@cs.teicrete.gr>
 *
 * 2017 - 2018 
 *
 * A Simple Read Test can be performed in a relatively simple way like 
 * simple write test. After writing an increasing  value to BRAM,  a 
 * subsequent read operation via the NoC Firewall is performed, and data
 * is compared with the data written. The number of cases is alike the 
 * Write Test, i.e. 4096 cases which 1024  are for SIMPLE READ TEST.
 * Thus, in the Simple Read Test, we perform the following steps. 
 * For example:
 * 1. Write BRAM memory location via NoC Firewall with a different 
 * (always increasing) value default_written_value1 
 *      - rc1 = accessBramFW(0x0, 1, 1, addr_reg, &default_written_value)
 * 2. Read BRAM memory location via NoC Firewall in read_value.
 *	- rc2 = accessBramFW(0x1, 1, 1, addr_reg, &read_value)
 * 3. Finally, the check_read function is called with the complete setup pattern,
 *     default_written_value and read_value.
 *    For each port, the above Read Test is passes if and only if:
 *      - values default_written_value and read_value are equal and the setup pattern
 *        for that port is deny read (accept write) or accept all, or
 *      - values default_written_value and read_value are non-equal and setup pattern
 *         for that port is deny write (accept read) or deny all
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h> 
#include "user_mode.h"

// this is pass rule, r -> r will pass
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

/*  Function check read */
void check_read(unsigned int input_port, enum rule pattern, 
                 unsigned int actual_value, unsigned int read_value) {
switch(pattern){
        case w:
		if(read_value != actual_value)
		   printf("_Input_Port:%d  Accept:W  -> Rule 'R' PASS (read_value, actual_value):(%x %x) \n", input_port, read_value, actual_value);
		else
		   printf("_Input_Port:%d  Accept:W  -> Rule 'R' ________________FAIL (read_value, actual_value):(%x %x) \n\n", input_port, read_value, actual_value);
		break;
        case r:
		if(read_value == actual_value)
		   printf("_Input_Port:%d  Accept:R  -> Rule 'R' PASS (read_value, actual_value):(%x %x) \n", input_port, read_value, actual_value);
		else
	           printf("_Input_Port:%d  Accept:R  -> Rule 'R' ________________FAIL (read_value, actual_value):(%x %x) \n\n", input_port, read_value, actual_value);
		break;
        case wr:
		if(read_value == actual_value)
		   printf("_Input_Port:%d  Accept:WR -> Rule 'R' PASS (read_value, actual_value):(%x %x) \n", input_port, read_value, actual_value);
		else
	           printf("_Input_Port:%d  Accept:WR -> Rule 'R' ________________FAIL (read_value, actual_value):(%x %x) \n\n", input_port, read_value, actual_value);
		break;
        case none: // deny all
		if(read_value != actual_value)
		   printf("_Input_Port:%d  Accept:-  -> Rule 'R' PASS (read_value, actual_value):(%x %x) \n", input_port, read_value, actual_value);
		else
	           printf("_Input_Port:%d  Accept:-  -> Rule 'W' ________________FAIL (read_value, actual_value):(%x %x) \n\n", input_port, read_value, actual_value);
		break;
    }

}

int main(int argc, char *argv[])
{
unsigned int rule;
int i, j, k, l = 0;
int ii, jj;
unsigned int m = 0;
unsigned int addr_reg = 0x00000000;
unsigned int wBit, rBit = 0;
volatile int readVal = 0, writeVal = 0;
volatile int rc1 = 0, rc2 = 0, rc3 = 0;

unsigned int default_written_value1 = (unsigned int) 0; // written directly to BRAM
unsigned int         read_value1  = (unsigned int) 10000; // written via FW
unsigned int default_written_value2 = (unsigned int) 1024; // written directly to BRAM
unsigned int         read_value2  = (unsigned int) 10000; // written via FW
unsigned int default_written_value3 = (unsigned int) 2048; // written directly to BRAM
unsigned int         read_value3  = (unsigned int) 10000; // written via FW
unsigned int default_written_value4 = (unsigned int) 3072; // written directly to BRAM
unsigned int         read_value4  = (unsigned int) 10000; // written via FW

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

// Port 1 - simple setup with wr accept
		default_written_value1 += (unsigned int) 1;
		printf("written_value:%d \n",default_written_value1);
		read_value1   = (unsigned int) 65535;
		/* 1. Write BRAM memory location via NoC Firewall  */
                rc1 = accessBramFW(0x0, 1, 1, addr_reg, &default_written_value1);

		// Get write and read bit from rule for this port and setup FW on this port
		rule = pattern [i];
		wBit = (rule & (1 << 5)) >> 5;
		rBit = (rule & (1 << 4)) >> 4;
      		setupSFW(1, 0x40000000, 0x40000100, 1, wBit, rBit); 
		/* 2. Read BRAM memory location via NoC Firewall  */
		rc2 = accessBramFW(0x1, 1, 1, addr_reg, &read_value1);
		printf("default_written_value1:%d, read_value1:%d \n", default_written_value1, read_value1); fflush(stdout);
		/* 3. Call the check_read function */
		check_read(1, rule, default_written_value1, read_value1);

		// reset for new round
      		setupSFW(1, 0x40000000, 0x40000100, 1, 0, 0); 


//Port 2 - simple setup with wr accept
		default_written_value2 += (unsigned int) 1;
		read_value2   = (unsigned int) 65535;

		/* 1. Write BRAM memory location via NoC Firewall  */
                rc1 = accessBramFW(0x0, 2, 2, addr_reg, &default_written_value2);

		//Get write and read bit from rule for this port and setup FW on this port
		rule = pattern [j];
		wBit = (rule & (1 << 5)) >> 5;
		rBit = (rule & (1 << 4)) >> 4;
		setupSFW(2, 0x40001000, 0x40001100, 1, wBit, rBit);

		/* 2. Read BRAM memory location via NoC Firewall  */
		rc2 = accessBramFW(0x1, 2, 2, addr_reg, &read_value2);
		printf("default_written_value2:%d, read_value2:%d \n", default_written_value2, read_value2); fflush(stdout);
		/* 3. Call the check_read function */
		check_read(2, rule, default_written_value2, read_value2);

		// reset for new round
      		setupSFW(2, 0x40001000, 0x40001100, 1, 0, 0); 



//Port 3 - simple setup with wr accept
		default_written_value3 += (unsigned int) 1;
		read_value3   = (unsigned int) 65535;

		/* 1. Write BRAM memory location via NoC Firewall  */
                rc1 = accessBramFW(0x0, 3, 3, addr_reg, &default_written_value3); // write 00000000

		/* Get write and read bit from rule for this port and setup FW on this port */
		rule = pattern [k];
		wBit = (rule & (1 << 5)) >> 5;
		rBit = (rule & (1 << 4)) >> 4;
		setupSFW(3, 0x40002000, 0x40002100, 1, wBit, rBit); 

		/* 2. Read BRAM memory location via NoC Firewall  */
		rc2 = accessBramFW(0x1, 3, 3, addr_reg, &read_value3);
		printf("default_written_value3:%d, read_value3:%d \n", default_written_value3, read_value3); fflush(stdout);
		/* 3. Call the check_read function */
		check_read(3, rule, default_written_value3, read_value3);

		// reset for new round
      		setupSFW(3, 0x40000000, 0x40000100, 1, 0, 0); 

//Port 4 - simple setup with wr accept
		default_written_value4 += (unsigned int) 1;
		read_value4   = (unsigned int) 65535;

		/* 1. Write BRAM memory location via NoC Firewall  */
                rc1 = accessBramFW(0x0, 4, 4, addr_reg, &default_written_value4); // write 00000000

		// Get write and read bit from rule for this port and setup FW on this port
		rule = pattern [l];
		wBit = (rule & (1 << 5)) >> 5;
		rBit = (rule & (1 << 4)) >> 4;
		setupSFW(4, 0x40003000, 0x40003100, 1, wBit, rBit); 

		/* 2. Read BRAM memory location via NoC Firewall  */
		rc2 = accessBramFW(0x1, 4, 4, addr_reg, &read_value4);
		printf("default_written_value4:%d, read_value4:%d \n", default_written_value4, read_value4); fflush(stdout);
		/* 3. Call the check_read function */
		check_read(4, rule, default_written_value4, read_value4);

		// reset for new round
      		setupSFW(4, 0x40000000, 0x40000100, 1, 0, 0); 

		printf("_##################################################\n\n");


	  }
       }
    }
  }
        exit(0);
}
