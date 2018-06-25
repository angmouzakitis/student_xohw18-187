/* ---------------------------------------------------------
 * High Level driver API - Fwgroup1:clinic1 Functions.   
 *
 * Created by V. Piperaki, A. Mouzakitis and M.D Grammatikakis
 *
 * TEI of Crete <dreams@cs.teicrete.gr>
 *
 * 2017 - 2018 
 *
 * Clinics (clinic, clinic1, clinic2) respectively groups
 * (fwgroup, fwgroup1 and fwgroup2) have only read access 
 * to all BRAMs (1, 2 and 3) as set up by system administratori
 * (systemadmin_setup.c). 
 * In this file we show read access from fwgroup1:clinic1 
 * ------------------------------------------------------- */
//#define FW_CLINIC1_LOGS
#define FW_CLINIC1_LOGS2


#ifdef FW_CLINIC1_LOGS
#define fw_printf(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define fw_printf(fmt, ...)
#endif

#ifdef FW_CLINIC1_LOGS2
#define fw2_printf(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define fw2_printf(fmt, ...)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "user_mode.h" //High Level driver API - User space library file
//#include "stdlib.h"

gid_t getBasicGroup()
{
	gid_t myGid = -1, i = 0;
	int ngroups = NGROUPS_MAX;
	gid_t list_of_groups[ngroups];
	fw_printf("getBasicGroup: ngroups:%d \n", ngroups);
	gid_t gid_fwgroup = groupIdFromName("fwgroup1"); 
	/* check the current user id */
	grouplist(list_of_groups, &ngroups); //call function grouplist to get groups for each current user 
	for (i=0; i<ngroups; i++) {
		fw_printf("getBasicGroup: checking gid_fwgroupd:%d with group:%d \n", (int) gid_fwgroup, (int) list_of_groups[i]);
		if (list_of_groups[i] == gid_fwgroup) {
			myGid = gid_fwgroup;
			fw2_printf("getBasicGroup:group id is:%d \n", (int) list_of_groups[i]);
			break;
		}
	}
	return myGid;
}

/* Function to return Patient location (number) in BRAM 
 */
int hashPatientNo(unsigned int patient_no)
{

	int x = (patient_no+3733)%2; //	return (patient_no+3733)%2;
	return x;
}


/* Function to write the data of a patient  through the firewall in specific Bram.
 */
void writePatientData(unsigned int patient_no, char* mac_address, char* patient_name) 
{
	int i = 0, N = 0;	
	volatile int rc1, rc2;
	unsigned int bram_patient_no_direct, bram_mac_direct, bram_patient_name_direct;
	unsigned int bram_patient_no, bram_mac, bram_patient_name;
	unsigned char bram_mac_address[18];
	unsigned int inport_and_bram_no;

	// convert patient MAC adress (key) from string to int array
	// to be rewritten ... not working e.g. for WX:XX:XX:XX:XX:XX
	unsigned int iMac[6];
	if(sscanf(mac_address, "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]) != 6){
		printf("writePatientData: Invalid mac address.\n\n");
		exit(1);
	}

	// convert patient name fron string to int array
	int name_size = strlen(patient_name); //length of string
	unsigned int iName[name_size + 1];
	for(i=0; i<name_size; i++){
		iName[i] = patient_name[i]; // casting to integer (type int)
	}
	iName[name_size] = (int)'\0';

	N=hashPatientNo(patient_no);
        printf("writePatientData: HASH: patient_no:%d -> N:%d - OFFSET_PATIENT_NO:%d (N*OFFSET_PATIENT_NO):%d H:%d\n", 
								patient_no, N, OFFSET_PATIENT_NO, N*OFFSET_PATIENT_NO, N);
	//check if current user is in specific bram_no by calling accessBramFW function (user_mode.h)
	for(inport_and_bram_no=1; inport_and_bram_no<=USER_NOC_OUTPUT_PORTS; inport_and_bram_no++){
		//search patient_no in specific bram no
		bram_patient_no = 0xFFFFFFFF; // read from BRAM again
		rc1 = accessBramFW(0x0, inport_and_bram_no, inport_and_bram_no, N*OFFSET_PATIENT_NO, &bram_patient_no);
		printf("writePatientData: Read_BRAM (inport:%d - output_port:%d - offset:%d) -> patient_no:%d  H:%d\n", 
                                      inport_and_bram_no, inport_and_bram_no, N*OFFSET_PATIENT_NO, bram_patient_no, N);

		if(patient_no == bram_patient_no && strcmp(mac_address, bram_mac_address) == 0){
	
			//search mac_address in specific inport_and_bram_no
			for(i=0; i<6; i++){
				// write mac in specific bram ao
				rc1 = accessBramFW(0x0, inport_and_bram_no, inport_and_bram_no, N*OFFSET_PATIENT_NO+OFFSET_MAC_ADDR, &iMac[i]);
		                printf("writePatientData: Read_BRAM (inport:%d - output_port:%d - offset:%d) -> mac: %x (in decimal %d) \n",
					inport_and_bram_no,inport_and_bram_no, (N*OFFSET_PATIENT_NO)+OFFSET_MAC_ADDR + (0x4*i), iMac[i], iMac[i]);
			}

			for(i=0; i<(name_size + 1); i++){
				rc2 = accessBramFW(0x0, inport_and_bram_no, inport_and_bram_no, 
								N*OFFSET_PATIENT_NO + OFFSET_PATIENT_NAME + (0x4*i), &iName[i]);
				printf("setupPatientInBram: (inport: %d) - BRAM%d[%x)<-%c  (patient_name) (in decimal %d) \n",
					inport_and_bram_no, inport_and_bram_no, N*OFFSET_PATIENT_NO + OFFSET_PATIENT_NAME + (0x4*i), iName[i], iName[i]);
        		}
		} else {
			printf("writePatientData: Patient_no:%d not found\n", patient_no);
			exit(1);
		}
	}
}




/* Function of clinic1 (doctor) to read patient data.
 * This function is based on patient_number to get back data(variable data)
 * Return value: 0 on success
 *               -EACCES on permission error
 *               -EINVAL on invalid MAC address
 */
int readPatientData(unsigned int patient_no, char **mac_address, char ** patient_name)
{
	int N;	
	int i = 0;
	unsigned int inport, bram_no;
	unsigned int addr_reg;
	int rc1, rc2, rc3;
	unsigned int bram_patient_no;
	char bram_mac_address [18];
        unsigned int iMac[6]; // read 6 x 16 bits (i.e. each time 2 bytes correspond to part of mac_address (e.g. EE))
        unsigned int iName[22], len; // read 22 x 16 bits (i.e. each time 2 bytes correspond to two characters of name
				     // notice that: total chars read is 44 while 44 x 2 (wasted 16 bits per 32 bit access) equals
				     // OFFSET_PATIENT_NO   (0x00000080) - OFFSET_PATIENT_NAME (0x00000028)

	fw_printf("**********IN_____readPatientData: patient_mac:%s \n", *mac_address);
	fw_printf("**********IN_____readPatientData: patient_name:%s \n", *patient_name);

	*mac_address = (char*)malloc(sizeof(char) * 100);
	*patient_name = (char*)malloc(sizeof(char) * 100);

	// initial values lost due to malloc
	fw_printf("**********IN_____readPatientData: patient_mac:%s \n", *mac_address);
	fw_printf("**********IN_____readPatientData: patient_name:%s \n", *patient_name);

	gid_t myGid = getBasicGroup();
        inport = getInputPortForGid(myGid);
        bram_no = getOutputPortForGid(myGid);
        fw_printf("readPatientData: myGid:%d -> inport:%d bram_no:%d \n", myGid, inport, bram_no); 

	N=hashPatientNo(patient_no);
        fw_printf("readPatientData: HASH: patient_no:%d -> N:%d - OFFSET_PATIENT_NO:%d (N*OFFSET_PATIENT_NO):%d H:%d\n", 
								patient_no, N, OFFSET_PATIENT_NO, N*OFFSET_PATIENT_NO, N);

	bram_patient_no = 0xFFFFFFFF; // overwritten in order to be read from BRAM again
	addr_reg = N*OFFSET_PATIENT_NO;

	rc1 = accessBramFW(0x1, inport, bram_no, addr_reg , &bram_patient_no);
	fw_printf("readPatientData: Read_BRAM (inport:%d - output_port:%d - offset:%d) -> bram_patient_no:%d  H:%d\n", 
		inport, bram_no, addr_reg, bram_patient_no, N);

	if((rc1 >= 0) && (patient_no == bram_patient_no)){
		// read mac_address in specific inport_and_bram_no
		for(i=0; i<6; i++){
			// (op_code, inport,  outport, addr_reg, *data)
			addr_reg = N*OFFSET_PATIENT_NO + OFFSET_MAC_ADDR + (0x4*i);
			rc2 = accessBramFW(0x1, inport, bram_no, addr_reg, &iMac[i]);
	                fw_printf("readPatientData: Read_BRAM (inport:%d - output_port:%d - offset:%d) -> mac: %x (in decimal %d) \n",
				inport, bram_no, addr_reg, iMac[i], iMac[i]);
		}
       		// convert patient MAC adress (key) from string to int array
		if (snprintf(bram_mac_address, sizeof(bram_mac_address), "%02x:%02x:%02x:%02x:%02x:%02x", 
							iMac[0], iMac[1], iMac[2], iMac[3], iMac[4], iMac[5]) !=17){
			printf("readPatientData: Invalid mac address.\n");
			return -EINVAL;
		}
        	bram_mac_address[17] = (int)'\0';
		strcpy(*mac_address, bram_mac_address); // NEVER IN STRINGS RETURNED *mac_address = bram_mac_address;
		fw_printf("readPatientData: mac_address:%s \n", *mac_address);

		for(i=0; i<22; i++){
			addr_reg = N*OFFSET_PATIENT_NO + OFFSET_PATIENT_NAME + (0x4*i);
			rc1 = accessBramFW(0x1, inport, bram_no, addr_reg, &iName[i]);
			fw_printf("readPatientData: ok_%d (inport: %d) - BRAM%d[%x)<-%c  (patient_name) (in decimal %d) \n",
				i, inport, bram_no, (N*OFFSET_PATIENT_NO)+OFFSET_PATIENT_NAME + (0x4*i), iName[i], iName[i]);
			(*patient_name)[i] = iName[i];
		}
		fw_printf("readPatientData: patient_name:%s \n", *patient_name);
	} else {
		printf("can't readPatientData: Patient_no:%d not found rc1:%d \n", patient_no, rc1);
		return -EACCES;
	}

}

void testScenario1(){

        int patient_no[20] = {
                15,
                16,
                25,
                35
        };

	char *read_patient_mac  = "xx:xx:xx:xx:xx:xx";
	char *read_patient_name = "12345678901234567890123456789012345678901234";
	int i, j;

#if defined(_WRITEDENIED)
// TEST kernel memory_dump upon writePatientData
	char *patient_mac[4] = {
                "E4:EE:FD:00:0E:68",
                "E4:EE:FD:00:07:17",
                "E4:EE:FD:00:0E:68",
                "E4:EE:FD:00:07:17"
	};

	char *patient_name[4] = {
		"patient_A_00",
		"patient_B_00",
		"patient_A_01",
		"patient_B_02"
	};

	// overwrite patient with no 15 - NEXT TWO LINES
	printf("Patient write data - patient no:%d to mac_address:%s and patient_name:%s \n", 
		patient_no[2], patient_mac[1], patient_name[1]);
	getchar();
	writePatientData(patient_no[2], patient_mac[1], patient_name[1]);

	//writePatientData(patient_no[1], patient_mac[0], patient_name[0]);
	//writePatientData(patient_no[2], patient_mac[3], patient_name[3]);
	//writePatientData(patient_no[3], patient_mac[2], patient_name[2]);
getchar();
#endif

	fw_printf("BEFORE_readPatientData: patient_no:%d, mac_address:%s,read_patient_name:%s \n", patient_no[2], read_patient_mac, read_patient_name);

#if defined(_READOK)
        if (readPatientData(patient_no[2], &read_patient_mac, &read_patient_name) == -EACCES) {
		exit(1);
	}
#elif defined(_READDEDENIED)
        if (readPatientData(patient_no[3], &read_patient_mac, &read_patient_name) == -EACCES) {
		exit(1);
	}
#else
#error "define _READOK or _READDEDENIED"
#endif

	fw2_printf("readPatientData: patient_no:%d, mac_address:%s, read_patient_name:%s \n", patient_no[2], read_patient_mac, read_patient_name);

}


int main(int argc, char *argv[])
{

//gid_t gi = getBasicGroup();
testScenario1();
}

