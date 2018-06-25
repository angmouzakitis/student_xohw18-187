/* --------------------------------------------------------------------------------
 * High Level driver API - System Adinistrator Functions.   
 *
 * Created by V. Piperaki, A.Mouzakitis and M.D Grammatikakis
 *
 * TEI of Crete <dreams@cs.teicrete.gr>
 *
 * 2017 - 2018
 *
 * System Admin makes the follows steps: 
 * Step 1: Initially, getBasicGroup Function is called. If system administrator
 * 	   is verified, it can call the following functions.
 * Step 2: setGidPerNoCInport and  setGidPerNoCOutput functions are called
 * 	   to map Linux groups to input and output ports. 
 * Step 3: Firewall is temporarily setup via setupFWTemp to allow the system
 * 	   administrator to write patient information o BRAM.
 * Step 4: Then system administrator  writes patient data in tables
 *	   (i.e.,   patient_clinic[i], patient_no[i], patient_mac[i],
 *	   patient_name[i]) via writePatientData function 
 * Step 5: Then, system administrator uses writePatientData function to write 
 * 	   patient data (i.e.,  patient_clinic[i], patient_no[i], patient_mac[i],
 *	   patient_name[i]) to an appropriate BRAM (defined in Step 2).  
 * ------------------------------------------------------------------------------ */

//#define FW_ADMIN_LOGS
#define FW_ADMIN_LOGS2

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <grp.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h> 

#include "keccak.h" // SHA-3 library file
#include "user_mode.h" //High Level driver API - User space library file

#define NO_PATIENTS_IN_BRAMS 4

/* Function that checks in which group belongs the current user */
gid_t getBasicGroup()
{
	gid_t myGid = -1, i = 0;
	int ngroups = NGROUPS_MAX;
	gid_t list_of_groups[ngroups];

	gid_t gid_system_admin = groupIdFromName("root"); //save system_admin groupid

	/* check if the current user id  belongs to system_admin group (root) */
	grouplist(list_of_groups, &ngroups); //call function grouplist to get groups for each current user  
	for (i=0; i<ngroups; i++)
		if (list_of_groups[i] == gid_system_admin) {
			myGid = gid_system_admin;
			break;
		}
	return myGid;
}

/* Function that shows:
 *   - Total packets passed per port
 *   - Dropped packets from port(due to Fifo full error)
 *   - Droped packets from port (due to firewall rule)
 *   -  all above stats per all ports totally.
*/
void testStats(){

	unsigned int inport;
	for(inport=1; inport<=4; inport++){
		readStatsTotalPerPort(inport);
	}
	for(inport=1; inport<=4; inport++){
	 	readStatsFifoPerPort(inport);
	}
	for(inport=1; inport<=4; inport++){
	 	readStatsFwPerPort(inport);
	}
	readStatsTotal(inport);
	readStatsFifo(inport);
	readStatsFw(inport);
}
	
/* Function to setup temporary firewall rules for clinic, clinic1, clinic2 
 * (note: BRAM4 accessible via any input port, use setupSFW)
 * This function is called from system admin only
 */
void setupFW_Temp(){

	setupCFW(1, 0x00000000, 0x00000100, 1, 0, 0, 1);
	setupCFW(2, 0x00000000, 0x00000100, 1, 0, 0, 2);
	setupCFW(3, 0x00000000, 0x00000100, 1, 0, 0, 3);

	// admin CAN access BRAM4 in read/write mode
	setupSFW(ADMIN_OUTPUT_PORT, 0x40003000, 0x40003100, 1, 0, 0);

#ifdef FW_ADMIN_LOGS
printf("setupFw_Temp: Allow read/write on BRAM1, BRAM2, BRAM3 from input ports 1, 2, 3, respectively \n");
printf("setupFw_Temp: Allow read/write on BRAM4 by all ports \n");
#endif

}


/* Function to setup final firewall rules for clinic, clinic1, clinic2 
   (note: BRAM4 accessible via any input port, use setupSFW)
   This function is called from system admin only
   */
void setupFW_Final(){

	setupCFW(1, 0x00000000, 0x00000100, 1, 1, 0, 1);
	setupCFW(2, 0x00000000, 0x00000100, 1, 1, 0, 2);
	setupCFW(3, 0x00000000, 0x00000100, 1, 1, 0, 3);

	/* clinic/clinic1/clinic2 (ALL) CAN access BRAM4 for read-only. Since they come from diff ports, must use SFW */
	setupSFW(ADMIN_OUTPUT_PORT, 0x40003000, 0x40003100, 1, 1, 0);

#ifdef FW_ADMIN_LOGS
printf("setupFw_Final: Allow read on BRAM1, BRAM2, BRAM3 from input ports 1, 2, 3, respectively \n");
printf("setupFW_Final: Allow read on BRAM4 by all ports \n");
#endif
}

/* Function to hash Patient location in BRAM via SHA-3 kECCAK
 */
int hashPatientNo(unsigned int patient_no)
{
	int i;
	long x;
    	sha3_context c;
    	const uint8_t *hash; 
    	const uint8_t c1 = 0xa3; 
	uint8_t buf[200]; 


#ifndef SHA3_USE_KECCAK 
    /* [FIPS 202] KAT follow */ 
    const static uint8_t sha3_256_empty[256 / 8] = { 
        0xa7, 0xff, 0xc6, 0xf8, 0xbf, 0x1e, 0xd7, 0x66, 
	0x51, 0xc1, 0x47, 0x56, 0xa0, 0x61, 0xd6, 0x62, 
	0xf5, 0x80, 0xff, 0x4d, 0xe4, 0x3b, 0x49, 0xfa,  
	0x82, 0xd8, 0x0a, 0x4b, 0x80, 0xf8, 0x43, 0x4a 
    }; 
    const static uint8_t sha3_256_0xa3_200_times[256 / 8] = { 
        0x79, 0xf3, 0x8a, 0xde, 0xc5, 0xc2, 0x03, 0x07, 
        0xa9, 0x8e, 0xf7, 0x6e, 0x83, 0x24, 0xaf, 0xbf, 
        0xd4, 0x6c, 0xfd, 0x81, 0xb2, 0x2e, 0x39, 0x73, 
        0xc6, 0x5f, 0xa1, 0xbd, 0x9d, 0xe3, 0x17, 0x87 
    }; 
    const static uint8_t sha3_384_0xa3_200_times[384 / 8] = { 
        0x18, 0x81, 0xde, 0x2c, 0xa7, 0xe4, 0x1e, 0xf9, 
        0x5d, 0xc4, 0x73, 0x2b, 0x8f, 0x5f, 0x00, 0x2b, 
        0x18, 0x9c, 0xc1, 0xe4, 0x2b, 0x74, 0x16, 0x8e, 
        0xd1, 0x73, 0x26, 0x49, 0xce, 0x1d, 0xbc, 0xdd, 
        0x76, 0x19, 0x7a, 0x31, 0xfd, 0x55, 0xee, 0x98, 
        0x9f, 0x2d, 0x70, 0x50, 0xdd, 0x47, 0x3e, 0x8f 
    }; 
    const static uint8_t sha3_512_0xa3_200_times[512 / 8] = { 
        0xe7, 0x6d, 0xfa, 0xd2, 0x20, 0x84, 0xa8, 0xb1, 
        0x46, 0x7f, 0xcf, 0x2f, 0xfa, 0x58, 0x36, 0x1b, 
        0xec, 0x76, 0x28, 0xed, 0xf5, 0xf3, 0xfd, 0xc0, 
        0xe4, 0x80, 0x5d, 0xc4, 0x8c, 0xae, 0xec, 0xa8, 
        0x1b, 0x7c, 0x13, 0xc3, 0x0a, 0xdf, 0x52, 0xa3, 
        0x65, 0x95, 0x84, 0x73, 0x9a, 0x2d, 0xf4, 0x6b, 
        0xe5, 0x89, 0xc5, 0x1c, 0xa1, 0xa4, 0xa8, 0x41, 
        0x6d, 0xf6, 0x54, 0x5a, 0x1c, 0xe8, 0xba, 0x00 
    }; 
#endif 

	memset(buf, c1, sizeof(buf)); 
	sha3_Init256(&c); 
	sha3_Update(&c, buf, sizeof(buf)); 
	hash = (uint8_t *) sha3_Finalize(&c); 

	x = *hash;
	return 0;
}


/* Function to setup firewall rules and patient data.
   This function is called from system admin to a) define firewall rules temporatrily and b) write patient data
   (e.g patient number, mac address (or key) and  patient name) per NoC output port (equvalently BRAM number)
   We assume clinic/clinic1/clinic2 (mapped previously to inports 1/2/3) access patient data from BRAM1/2/3
   Patient data location in BRAM based on hashing patient no
   */
void writePatientData(unsigned int patient_clinic, unsigned int patient_no, char *mac_address, char *patient_name)
{
	unsigned int inport = patient_clinic; // note: take inport = outport for writing patient data
	unsigned int bram_no = patient_clinic; // note: take inport = outport for writing patient data
	unsigned int written_value;
	unsigned int addr_reg;
	int i, rc1 = 0, rc2 = 0;

	/* convert patient MAC adress (key) from string to int array */
	unsigned int iMac[6];
	if(sscanf(mac_address, "%x:%x:%x:%x:%x:%x", &iMac[0], &iMac[1], &iMac[2], &iMac[3], &iMac[4], &iMac[5]) != 6){
		printf("writePatientData: Invalid mac address.\n\n");
	        exit(1);
	}
	/* convert patient name fron string to int array */
	int n, N;
	int name_size = strlen(patient_name); //length of string
	unsigned int iName[name_size + 1];
	for(n=0; n<name_size; n++){
		iName[n] = patient_name[n];
	}
	iName[name_size] = (int)'\0';

	
	struct tms tb;
	double t1, t2, min, max, avg_time=0.0;
	double ticspersec;

	/* compute clock ticks per sec */
	ticspersec=(double)sysconf(_SC_CLK_TCK); 
	t1=(double)times(&tb); // start timer

	N = hashPatientNo(patient_no); // hash patient_no for location of patient data in BRAM (bram_no)

	t2=(double)times(&tb); // end timer
      	avg_time+=(t2-t1)/ticspersec;
	max = t2-t1;
	if (max > t2-t1)
		max = t2-t1;
	if (min < t2-t1)
		min = t2-t1;
  	//printf("Delay_hash: %lf sec (max:%lf, min:%lf) \n", avg_time/1000.0, max, min);
	fflush(stdout);

	addr_reg = N*OFFSET_PATIENT_NO;

#ifdef FW_ADMIN_LOGS
printf("writePatientData: (input_port:%d) BRAM%d[%d]<-(pat_no:%d, mac:%x:%x:%x:%x:%x:%x, pat_name:%s) H:%d \n",
	inport, bram_no, addr_reg, patient_no, iMac[0], iMac[1], iMac[2], iMac[3], iMac[4], iMac[5], patient_name, N);
#endif

	/* Write patient data to specific (input port, ouput port, offset in BRAM) */
	rc1 = accessBramFW(0x0, inport,  bram_no, addr_reg, &patient_no);

#ifdef FW_ADMIN_LOGS
// check values directly to bram
written_value=0xFFFFFFFF;
rc2 = accessBram(0x1, bram_no, addr_reg, &written_value);
printf("writePatientData: BRAM%d[%x)<-(%d %d) (patient_no) \n", bram_no, addr_reg, patient_no, written_value);
#endif

	/* Write MAC to specific (input port, ouput port, offset in BRAM) */
	for(i=0; i<6; i++){
		addr_reg = N*OFFSET_PATIENT_NO + OFFSET_MAC_ADDR + (0x4*i);
#ifdef FW_ADMIN_LOGS
printf("writePatientData: (inport: %d) - BRAM%d[%x)<-%x (patient_mac) (in decimal %d) \n", inport, bram_no, addr_reg, iMac[i], iMac[i]);
#endif
		rc1 = accessBramFW(0x0, inport, bram_no, addr_reg, &iMac[i]);

#ifdef FW_ADMIN_LOGS
// check values by reading directly bram
written_value=0xFFFFFFFF;
rc2 = accessBram(0x1, bram_no, addr_reg, &written_value);
printf("writePatientData: BRAM%d[%x)<-(%x %x) (patient_mac) \n", bram_no, addr_reg, iMac[i], written_value);
#endif
	}

	/* Write name to specific (input port, ouput port, offset in BRAM) */
	for(i=0; i<(name_size + 1); i++){
		addr_reg = N*OFFSET_PATIENT_NO + OFFSET_PATIENT_NAME + (0x4*i);
#ifdef FW_ADMIN_LOGS
printf("writePatientData: (inport: %d) - BRAM%d[%x)<-%c  (patient_name) (in decimal %d) \n", inport, bram_no, addr_reg, iName[i], iName[i]);
#endif
		rc2 = accessBramFW(0x0, inport, bram_no, addr_reg, &iName[i]);

#ifdef FW_ADMIN_LOGS
// check values
rc2 = accessBram(0x1, bram_no, addr_reg, &written_value);
printf("writePatientData: BRAM%d[%x)<-(%c %c) (patient_name) \n", bram_no, addr_reg, iName[i], written_value);
#endif

	}
}

void testScenario1(){

	gid_t myGid;
	char group_name[][20] = {
		"fwgroup",
		"fwgroup",
		"fwgroup1",
		"fwgroup2"
	};
	/* note: take inport = outport for patient setup */
	char patient_clinic[20] = { 
		1,
		1,
		2,
		3
	};
	/* pairs of patient numbers, names & MAC (keys) */
	int patient_no[20] = {
		15,
		16,
		25,
		35
	};
	char patient_name[][20] = {
		"patient_A_clinic",
		"patient_B_clinic",
		"patient_A_clinic1",
		"Patient_B_clinic2"
	};
	char patient_mac[][20] = {
		"E4:EE:FD:00:0E:68",
		"E4:EE:FD:00:07:17",
		"E4:EE:FD:00:0E:68",
		"E4:EE:FD:00:07:17"
		//"aa:bb:cc:dd:ee:ff" //no works with lowercase
	};

	/* variables for monitoring timing */
	struct tms tb;
	double ticspersec;
	double t1, t2, min, max, avg_time=0.0;
	double sum_avg_time_all_iterations=0;
	int i, j;
	//testStats();

	/* compute clock ticks per sec */
	ticspersec=(double)sysconf(_SC_CLK_TCK);                                                                                   
// step2: set / map fwgroups:clinics (clinic, clinic1, clinic2) to NoC input ports (1-3) via FW
#ifdef FW_ADMIN_LOGS2
printf("step 2: Map Linux groups (fwgroup, fwgroup1, fwgroup2) to input and output ports (1-3) via FW \n");
#endif
	for(i=1; i<=999; i++) { 
		/* start from fwgroup = 2nd entry (same as first) and map to 1st input port */
		for(j=1; j<=USER_NOC_INPUT_PORTS; j++) { 
			t1=(double)times(&tb); // start timer
			setGidPerNoCInport(group_name[j], j);

#ifdef FW_ADMIN_LOGS
//printf("Call setGidPerNoCInport_(%d_%d) \n", i, i%USER_NOC_INPUT_PORTS+1);
#endif

			t2=(double)times(&tb); // end timer
      			avg_time+=(t2-t1)/ticspersec;
			max = t2-t1;
			if (max > t2-t1)
				max = t2-t1;
			if (min < t2-t1)
				min = t2-t1;
		}
	}
#ifdef FW_ADMIN_LOGS
  	printf("Delay_setGidPerNoCInport: %lf sec (max:%lf, min:%lf) \n", avg_time/(1000*USER_NOC_INPUT_PORTS), max, min);
	fflush(stdout);
#endif

// step2: set / map fwgroups:clinics (clinic, clinic1, clinic2) to NoC output ports (1-3) via FW
	for(i=1; i<=1000; i++) { 
		t1=(double)times(&tb); // start timer

		// start from fwgroup = 2nd entry (same as first) and map to 1st output port
		for(j=1; j<=USER_NOC_OUTPUT_PORTS; j++) { 
			t1=(double)times(&tb); // start timer
			setGidPerNoCOutport(group_name[j], j);

			t2=(double)times(&tb); // end timer
      			avg_time+=(t2-t1)/ticspersec;
			max = t2-t1;
			if (max > t2-t1)
				max = t2-t1;
			if (min < t2-t1)
				min = t2-t1;
		}
	}
#ifdef FW_ADMIN_LOGS
  	printf("Delay_setGidPerNoCOutport: %lf sec (max:%lf, min:%lf) \n", avg_time/(1000*USER_NOC_OUTPUT_PORTS), max, min);
	fflush(stdout);
#endif

//step 3: Firewall is temporarily setup by system Admin
#ifdef FW_ADMIN_LOGS2
printf("step 3: Firewall is temporarily set to allow write access to BRAMs\n");
#endif
	for(i=1; i<=1000; i++) { 
		t1=(double)times(&tb); // start timer
        	//setup firewall temporarily by admin
		setupFW_Temp();
		t2=(double)times(&tb); // end timer
      		avg_time+=(t2-t1)/ticspersec;
		max = t2-t1;
		if (max > t2-t1)
			max = t2-t1;
		if (min < t2-t1)
			min = t2-t1;
	}
#ifdef 	FW_ADMIN_LOGS
  	printf("%d_Delay_setupFW_Temp: %lf sec (max:%lf, min:%lf) \n", i, avg_time/1000, max, min);
	fflush(stdout);
#endif


// step 4: system Admin writes patient data in tables 
#ifdef FW_ADMIN_LOGS2
printf("step 4: Write patient data (patienti_no, patient_mac, patient_name) to BRAMs (1-3) \n");
#endif
	for(i=1; i<=1; i++) { 
		t1=(double)times(&tb); // start timer
		for(i=0; i<NO_PATIENTS_IN_BRAMS; i++) {
			/* 4 patients mapped to 3 BRAMS (BRAM1, BRAM2, BRAM3)
			 * note: take inport = outport for patient setup
			 */
			writePatientData(patient_clinic[i], patient_no[i], patient_mac[i], patient_name[i]); 
#ifdef FW_ADMIN_LOGS2
printf("writePatientData: patient_no:%d mac:%s name:%s BRAM:%d \n", patient_no[i], patient_mac[i], patient_name[i], patient_clinic[i]); 
#endif
		}
		t2=(double)times(&tb); // end timer
      		avg_time+=(t2-t1)/ticspersec;
	}
#ifdef FW_ADMIN_LOGS
  	printf("Delay_writePatientData: %lf sec \n",avg_time/(1000*NO_PATIENTS_IN_BRAMS));
	fflush(stdout);
#endif
	//testStats();

//step 5: setup firewall for read access permanently for user access
#ifdef FW_ADMIN_LOGS2
printf("step 5: Firewall is finally set to allow read only access to patient data for each group\n");
#endif
	for(i=1; i<=1000; i++) { 
		t1=(double)times(&tb); // start timer
		setupFW_Final();
		t2=(double)times(&tb); // end timer
      		avg_time+=(t2-t1)/ticspersec;
		max = t2-t1;
		if (max > t2-t1)
			max = t2-t1;
		if (min < t2-t1)
			min = t2-t1;
	}
#ifdef FW_ADMIN_LOGS
printf("Delay_setupFW_Final: %lf sec (max:%lf, min:%lf) \n", avg_time/1000, max, min);
fflush(stdout);
#endif

// setup: accessing statistics to verify all ok (only root)
	for(i=1; i<=1000; i++) { 
		t1=(double)times(&tb); // start timer
		testStats(); //access statistics
		t2=(double)times(&tb); // end timer
      		avg_time+=(t2-t1)/ticspersec;
		max = t2-t1;
		if (max > t2-t1)
			max = t2-t1;
		if (min < t2-t1)
			min = t2-t1;
  		//printf("%d_Delay_testStats: %lf sec (max:%lf, min:%lf) \n", i, avg_time/1000, max, min);
	}
#ifdef FW_ADMIN_LOGS
  	printf("Delay_testStats: %lf sec (max:%lf, min:%lf) \n", avg_time/1000, max, min);
	fflush(stdout);
#endif

}

int main(int argc, char *argv[])
{
//step 1: Initially, getBasicGroup Function is called
#ifdef FW_ADMIN_LOGS2
printf("System admin setup\n");
getchar();
printf("step 1: Check if current user is system admin \n");
#endif
	gid_t myGid = getBasicGroup();
	if(myGid < 0){
		printf("You are not authorized to use this function. \n\n");
		exit(1);
	}
	else
// step 2-5 
		testScenario1();

}
