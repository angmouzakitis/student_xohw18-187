/* ------------------------------------------------------------------
 * High Level driver API - User space methods.   
 *
 * Created by V. Piperaki, A. Mouzakitis, and M.D Grammatikakis
 *
 * June, 2018 
* ------------------------------------------------------------------- */
//#define FW_USER_LOGS
#undef FW_USER_LOGS

#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ioctl_fw.h" // ioctl numbers and definitions
#include "user_validation_functions.h" // functions to take ids, such as groupIdFromName() 

#define OFFSET_PATIENT_NO   0x00000080
#define OFFSET_MAC_ADDR     0x00000010
#define OFFSET_PATIENT_NAME 0x00000028
#define OFFSET_INPUT_PORTS_BRAM4  0x00000000
#define OFFSET_OUTPUT_PORTS_BRAM4 0x0000000C

#define USER_NOC_INPUT_PORTS 3 //number of input_ports for clinics
#define USER_NOC_OUTPUT_PORTS 3 //number of output_ports for clinics
#define ADMIN_INPUT_PORT 4 //input_port 4 for system Admin.
#define ADMIN_OUTPUT_PORT 4 //output_port 4 for system Admin.

char *nodename = "/dev/sw_firewall";

/* Function to check if current user is system administrator by using
 * grouplist method. It is called from other 
 * high level functions to validate access privileges of the current user.
 */
int is_admin()
{
	int systemadmin_flag = 0, i = 0;
	int ngroups = NGROUPS_MAX;
	gid_t list_of_groups[ngroups];
	gid_t gid_systemadmin;
	/* check if the current user id  belongs to system_admin group (root) */
	gid_systemadmin = groupIdFromName("root"); // get systemadmin groupid
	grouplist(list_of_groups, &ngroups); //call function grouplist to take in which groups belong each current user 
	for (i=0; i<ngroups; i++){
		if (list_of_groups[i] == gid_systemadmin) { 
			systemadmin_flag = 1;
			break;
		}
	}
	return systemadmin_flag;
}

/* Function to set/map Linux group names (equivalently clinics, and
 * indirectly doctors) to NoC input ports for access via FW
 * This function must be called from admin.
 */
void setGidPerNoCInport(char *group_name, unsigned int inport){
	gid_t admin_group, gid;
	unsigned int data;
	volatile int rc;

	/* Set parameter restrictions */
	if(inport <= 0 || inport > USER_NOC_INPUT_PORTS){
		printf("setGidPerNoCInport: Invalid number for NoC input port.\n");
		exit(1);
	}
	 /*check if current user is systemAdin.Only if user is sysem admin or root, then save in a table the matching pair (inport, group-id)*/
	if(is_admin()){ 
		admin_group = groupIdFromName("root");
		gid = groupIdFromName(group_name); //get group id from group name
		if((int)gid < 0){
			printf("setGidPerNoCInport: Invalid group.\n");
			exit(1);
		}
		data = (unsigned int) gid;
#ifdef FW_USER_LOGS
		printf("setGidPerNoCInport: calling accessBramFw (inport %d, outport:%d, offset:%d) \n", ADMIN_INPUT_PORT, ADMIN_OUTPUT_PORT, OFFSET_INPUT_PORTS_BRAM4 + (inport-1)*0x4);
#endif

		rc = accessBramFW(0x0, ADMIN_INPUT_PORT, ADMIN_OUTPUT_PORT, 
					OFFSET_INPUT_PORTS_BRAM4 + ((inport-1)*0x4), &data);
		if(rc < 0){
			printf("setGidPerNoCInport: ioctl failed - (rc:%d, meaning:%s) \n", rc, strerror(errno));
			exit(1);
		} else {

#ifdef FW_USER_LOGS
printf("setGidPerNoCInport: Set gid:%d (group_name: %s) to input port:%d.\n", (int) gid, group_name, inport);
#endif
		}
	} else
		printf("SetupGidPerNoCInputPort: You are not authorized as system admin.\n");

}

/* Function to set/ map group names (equivalently clinics, and
 * indirectly doctors) to NoC output ports for access via FW
 * This function must be called from admin.
 */
void setGidPerNoCOutport(char *group_name, int outport){
	gid_t admin_group, gid;
	unsigned int data;
	volatile int rc;

	/* Set parameter restrictions */
	if(outport <= 0 || outport > USER_NOC_OUTPUT_PORTS){
		printf("setGidPerNoCOutport: Invalid number for NoC outport port.\n");
		return; 
	}

	 /*check if current user is systemAdin.Only if user is root, then save in a table the matching pair (outport, group-id)*/
	if(is_admin()){
                admin_group = groupIdFromName("root");
                gid = groupIdFromName(group_name); 
		if((int)gid < 0){
			printf("setGidPerNoCOutport: Invalid group.\n");
			exit(1);
		}
                data = (unsigned int) gid;

#ifdef FW_USER_LOGS
		printf("setGidPerNoCOutport: calling accessBramFw (inport %d, outport:%d, offset:%d) \n", ADMIN_INPUT_PORT, ADMIN_OUTPUT_PORT, OFFSET_OUTPUT_PORTS_BRAM4 + (outport-1)*0x4);
#endif

		rc = accessBramFW(0x0, ADMIN_INPUT_PORT, ADMIN_OUTPUT_PORT, 
					OFFSET_OUTPUT_PORTS_BRAM4 +((outport-1)*0x4), &data);
		if(rc < 0){
			printf("setGidPerNoCOutport: ioctl failed - (rc:%d, meaning:%s) \n", rc, strerror(errno));
			exit(1);
		} else {
#ifdef FW_USER_LOGS
printf("setGidPerNoCOutport: Set gid:%d (group_name: %s) to output port:%d.\n", (int) gid, group_name, outport);
#endif
		}

	} else
		printf("setGidPerNoCOutport: You are not authorized as system admin.\n");
}

/* Function to obtain map of group names (equivalently clinics, and indirectly doctors) to NoC input ports
 * for access via FW.
 * This function can be called both from system Admin and clinics.
 */
int getInputPortForGid(gid_t gid) {
	int inport;
	int outport;
	int found = -1;
	int i;
	unsigned int data;
	volatile int rc;
	for(i=0; i<USER_NOC_INPUT_PORTS; i++){
		inport = ADMIN_INPUT_PORT; 
		outport = ADMIN_INPUT_PORT; 
		rc = accessBramFW(0x1, inport, outport, OFFSET_INPUT_PORTS_BRAM4 + (i*0x4), &data);
		if(rc < 0){
			printf("getInputPortForGid: ioctl failed - (rc:%d, meaning:%s) \n", rc, strerror(errno));
			exit(1);
		} else {
			if(gid == data){
				found = 1;
				inport = i+1;
				break;
			}
		}
	}
	if (found) {
#ifdef FW_USER_LOGS
printf("getInputPortForGid: gid:%d => inport:%d \n", (int) gid, inport);
#endif
		return inport;
	} else
		return -1;

}

/* Function to obtain map of group names (equivalently clinics,
 * and indirectly doctors) to NoC output ports (BRAMS)
 * for access via FW.
 * This function can be called both from system Admin and clinics.
 */
int getOutputPortForGid(gid_t gid){
	int found = -1;
	int inport;
	int outport;
	int i;
	unsigned int data;
	volatile int rc;

	for(i=0; i<USER_NOC_OUTPUT_PORTS; i++){
		inport = ADMIN_INPUT_PORT; 
		outport = ADMIN_INPUT_PORT; 
		rc = accessBramFW(0x1, inport, outport, OFFSET_OUTPUT_PORTS_BRAM4 + (i*0x4), &data);
		if(rc < 0){
			printf("getOutputPortForGid: ioctl failed - (rc:%d, meaning:%s) \n", rc, strerror(errno));
			exit(1);
		} else {
			if(gid == data){
				found = 1;
				outport = i+1;
				break;
			}
		}
	}
	if (found) {
#ifdef FW_USER_LOGS
printf("getOutputPortForGid: gid:%d => outport:%d \n", (int) gid, outport);
#endif
		return outport;
	} else
		return -1;
}

/* Function to perform ioctl call for simple FW setup, i.e setup
 * simple rule per input_port and range (no outport)
 * Perhaps also limit L/H_addr_reg to 0x40000100 for access via FW.
 * This function must be called from admin.
 */
void setupSFW(unsigned int inport, unsigned long L_addr_reg, unsigned long H_addr_reg, unsigned int rule, 
		unsigned int write_ops, unsigned int read_ops) { 
	int fd, rc;
	unsigned int Rule_reg = 0x0; 
	struct fw_ds my_fw_ds;		// firewall setup struct
	struct fw_ds check_my_fw_ds;	// firewall validate struct
	int ok_bram1 = 0, ok_bram2 = 0, ok_bram3 = 0, ok_bram4 = 0; 

	/* Set parameter restrictions */
        if (inport <= 0 || inport > 4){
                printf("setupSFW: Invalid value for input port:%d - must be 1, 2, 3 or 4\n", inport);
                exit(1);
        }
        if (rule < 0 || rule > 1){
                printf("setupSFW: Invalid value for rule:%d - must be 0 or 1.\n", rule);
                exit(1);
        }
        if (write_ops < 0 || write_ops > 1){
                printf("setupSFW: Invalid value for (deny) write operation:%d - must be 0 or 1.\n", write_ops);
                exit(1);
        }
        if (read_ops < 0 || read_ops > 1){
                printf("setupSFW: Invalid value for (deny) read operation:%d - must be 0 or 1.\n", read_ops);
                exit(1);
        }

#ifdef FW_USER_LOGS
printf("setupSFW: check offsets: L_addr_reg:%lx, H_addr_reg:%lx \n", L_addr_reg, H_addr_reg);
#endif

	ok_bram1 = ((L_addr_reg >= 0x40000000 || L_addr_reg < 0x40001000) || (H_addr_reg >= 0x40000000 || H_addr_reg < 0x40001000));
	if (!ok_bram1) {
		ok_bram2 = ((L_addr_reg >= 0x40001000 || L_addr_reg < 0x40002000) || (H_addr_reg >= 0x40001000 || H_addr_reg < 0x40002000));
		if (!ok_bram2) {
			ok_bram3 = ((L_addr_reg >= 0x40002000 || L_addr_reg < 0x40003000) || (H_addr_reg >= 0x40002000 || H_addr_reg < 0x40003000));
			if (!ok_bram3) {
				ok_bram4 = ((L_addr_reg >= 0x40003000 || L_addr_reg < 0x40004000) || (H_addr_reg >= 0x40003000 || H_addr_reg < 0x40004000));
				if (ok_bram4) {
#ifdef FW_USER_LOGS
printf("setupSFW: ok BRAM4 \n");
#endif
;
				}
			} else
#ifdef FW_USER_LOGS
printf("setupSFW: ok BRAM3 \n");
#endif
;
		} else
#ifdef FW_USER_LOGS
printf("setupSFW: ok BRAM2 \n");
#endif
;
	} else {

#ifdef FW_USER_LOGS
printf("setupSFW: ok BRAM1 \n");
#endif
;
	}

#ifdef FW_USER_LOGS
	if (!ok_bram1 && !ok_bram2 && !ok_bram3 && !ok_bram4) {
		printf("setupSFW: ERROR in offsets above \n");
		exit(1);
	}
#endif

	/* prepare rule register for simple setup */
	Rule_reg = rule << 7; // enable(1) or disable(0) rule
	Rule_reg += 0 << 6;   // set to 0 to operate in simple mode
	Rule_reg += write_ops << 5; //enable(1)/disable(0) rule for write operations
	Rule_reg += read_ops << 4; //enable(1)/disable(0) rule for read operations
	Rule_reg += 0x0; //unused

	/* fill setup data structure */
	my_fw_ds.input_port = inport;   
	my_fw_ds.L_addr_reg = L_addr_reg;
	my_fw_ds.H_addr_reg = H_addr_reg;
	my_fw_ds.Rule_reg = Rule_reg;

	fd = open(nodename, O_RDWR); //open firewall module 

#ifdef FW_USER_LOGS
printf("setupSFW: simple setup - input_port:%u, L/H_addr_reg:(%x %x) => Rule_reg:%x \n", my_fw_ds.input_port, my_fw_ds.L_addr_reg, my_fw_ds.H_addr_reg, my_fw_ds.Rule_reg);
#endif

	rc = ioctl(fd, IOCTL_SETUP_FW_PORT, &my_fw_ds); // ioctl call for simple fw setup (use iowrite32)
	if(rc < 0) {
                printf("setupSFW: ioctl failed - rc:%d meaning: %s \n", rc, strerror(errno));
		exit(1);
	} else {
#ifdef FW_USER_LOGS
                printf("setupSFW: ioctl passed - rc:%d \n", rc);
#endif
	}

#ifdef FW_USER_LOGS
	/* consider validating simple setup.
	 * Providing empty data (apart from inport) to read
`	*/
        check_my_fw_ds.input_port = inport; 
        check_my_fw_ds.L_addr_reg = 65536; // maximum is 65535
        check_my_fw_ds.H_addr_reg = 65536; // maximum is 65535
        check_my_fw_ds.Rule_reg = 0; // 0 is never used

        rc = ioctl(fd, IOCTL_CHECK_FW_PORT, &check_my_fw_ds);// new ioctl call for validation of setupSFW setup data 
	if(rc < 0){
                printf("setupSFW: ioctl failed - (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
	} else {
                printf("setupSFW: ioctl passed - rc:%d \n", rc);
printf("setupSFW: validating setup - input_port:%u, L/H_addr_reg:(%x %x) => Rule_reg:%x \n", my_fw_ds.input_port, my_fw_ds.L_addr_reg, my_fw_ds.H_addr_reg, my_fw_ds.Rule_reg);
	}
#endif

	close(fd);  //close firewall module
}

/* Function to read simple FW setup registers
 * This function can be called both from system Admin and clinics.
 */
void readSFW(gid_t gid) {
        int fd, rc;
	unsigned int inport;
	struct fw_ds my_fw_ds; // firewall setup struct 
	
	if (gid != groupIdFromName("root")){
		printf("readSFW: You are not authorized as system admin.\n");
		exit(1);
	}
	inport = ADMIN_INPUT_PORT;

        fd = open(nodename, O_RDWR); // open firewall module

        /* fill empty data structure apart from input_port to read */
        my_fw_ds.input_port = inport;
        my_fw_ds.L_addr_reg = 0x0;
        my_fw_ds.H_addr_reg = 0x0;
        my_fw_ds.Rule_reg = 0x0;

        rc = ioctl(fd, IOCTL_READ_FW_PORT, &my_fw_ds); // ioctl call to read simple fw setup data

	if(rc < 0){
                printf("readSFW: failed = %d, meaning: %s \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
printf("readSFW: checking simple setup - input_port:%u, L/H_addr_reg:(%x %x) => Rule_reg:%x (rc:%d) \n", my_fw_ds.input_port, my_fw_ds.L_addr_reg, my_fw_ds.H_addr_reg, my_fw_ds.Rule_reg, rc);
#endif

	close(fd); //close firewall module
}

/* Function to perform ioctl call for complex FW setup, i.e setup xomplex rule per input_port and range (outport)
 * Perhaps also limit L/H_addr_reg to 0x100 for access via FW.
 * This function must be called from admin
 */
void setupCFW(unsigned int inport, unsigned int L_addr_reg, unsigned int H_addr_reg, unsigned int rule, 
				unsigned int write_ops, unsigned int read_ops, unsigned int outport) {
	int fd, rc;
	unsigned int Rule_reg = 0x0; 
	struct fw_ds my_fw_ds;		// firewall setup struct
	struct fw_ds check_my_fw_ds;	// firewall check setup struct

	/* Set parameter restrictions */
	if (inport <= 0 || inport > 4){
		printf("setupCFW: Invalid value for input port:%d - must be 1, 2, 3 or 4\n", inport);
		exit(1);
	}
	if (outport <= 0 || outport > 4){
	  	printf("setupCFW: Invalid value for output port:%d - must be 1, 2, 3, or 4.\n", outport);
		exit(1);
	}
	if (rule < 0 || rule > 1){
	  	printf("setupCFW: Invalid value for rule:%d - must be 0 or 1.\n", rule);
		exit(1);
	}
	if (write_ops < 0 || write_ops > 1){
	  	printf("setupCFW: Invalid value for (deny) write operation:%d - must be 0 or 1.\n", write_ops);
		exit(1);
	}
	if (read_ops < 0 || read_ops > 1){
	  	printf("setupCFW: Invalid value for (deny) read operation:%d - must be 0 or 1.\n", read_ops);
		exit(1);
	}
	if((L_addr_reg >= H_addr_reg) || (L_addr_reg < 0x00000000 || L_addr_reg >= 0x00001000)
           || (H_addr_reg < 0x00000000 || H_addr_reg > 0x00001000)){
		printf("setupCFW: Invalid range [%x %x] - must be in [0x00000000, 0x00001000].\n", L_addr_reg, H_addr_reg);
		exit(1);
	}

	/* prepare rule register for complex setup */
	Rule_reg = rule << 7; //enable (1) or disable (0) rule
	Rule_reg += 1 << 6;   //set to 1 to operate in complex/ extended mode
	Rule_reg += write_ops << 5; //enable or disable rule for write operations
	Rule_reg += read_ops << 4; //enable or disable rule for read operations

        // last part setups output port
	if(outport == 4)
		Rule_reg += 1 << 3; //Value set to 1 to enable rule for output port 4 (BRAM 1)
        else 
	  	Rule_reg += 0 << 3;

	if(outport == 3)
        	  Rule_reg += 1 << 2; //Value set to 1 to enable rule for output port 3 (BRAM 2)
        else
          	Rule_reg += 0 << 2;

	if(outport == 2)
          	Rule_reg += 1 << 1; //Value set to 1 to enable rule for output port 2 (BRAM 3)
        else
          	Rule_reg += 0 << 1;

	if(outport == 1)
          	Rule_reg += 1; //Value set to 1 to enable rule for output port 1 (BRAM 4)
        else
          	Rule_reg += 0;

	/* fill up setup data structure */
	my_fw_ds.input_port = inport;   
	my_fw_ds.L_addr_reg = L_addr_reg;
	my_fw_ds.H_addr_reg = H_addr_reg;
	my_fw_ds.Rule_reg = Rule_reg;
	my_fw_ds.output_port = outport;

        fd = open(nodename,  O_RDWR, "u+rwx,g+rwx,o+rwx"); // open firewall module 

#ifdef FW_USER_LOGS
printf("setupCFW: input_port:%u, output_port:%u, L/H_addr_reg:(%x %x) => Rule_reg:%x \n", my_fw_ds.input_port, my_fw_ds.output_port, my_fw_ds.L_addr_reg, my_fw_ds.H_addr_reg, my_fw_ds.Rule_reg);
#endif
	
	rc = ioctl(fd, IOCTL_SETUP_FW_PORT, &my_fw_ds); // ioctl call for complex fw setup (use iowrite32)
	if(rc < 0) {
                printf("setupCFW: ioctl failed - rc:%d meaning: %s \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
	/*consider validating complex setup
	 * providing empty data (apart from inport, outport) to read
	 */
        check_my_fw_ds.input_port = inport; 
        check_my_fw_ds.output_port = outport; 
        check_my_fw_ds.L_addr_reg = 65536; // maximum is 65535
        check_my_fw_ds.H_addr_reg = 65536; // maximum is 65535
        check_my_fw_ds.Rule_reg = 0; 

        rc = ioctl(fd, IOCTL_CHECK_FW_PORT, &check_my_fw_ds);// new ioctl call for validation of setupCFW setup data 
	if(rc < 0){
                printf("setupCFW: ioctl failed - (rc:%d meaning:%s) \n", rc, strerror(errno));
		exit(1);
        } else {
                printf("setupCFW: ioctl passed (rc:%d) \n", rc);
printf("setupCFW: validating setup - input_port:%u, L/H_addr_reg:(%x %x) => Rule_reg:%x \n", my_fw_ds.input_port, my_fw_ds.L_addr_reg, my_fw_ds.H_addr_reg, my_fw_ds.Rule_reg);
	}
#endif

	close(fd);//close firewall module
}

/* Function to read complex FW setup registers
 * This function can be called both from system Admin and clinics.
 */
void readCFW(gid_t gid) { 
        int fd, rc;
	unsigned int inport;
	struct fw_ds my_fw_ds;
	
	if (gid != groupIdFromName("root")){
		printf("readCFW: You are not authorized as system admin.\n");
		exit(1);
	}

	inport = ADMIN_INPUT_PORT;

        fd = open(nodename, O_RDWR); //open firewall module 

        /* fill empty data structure apart from input_port to read */
        my_fw_ds.input_port = inport;
        my_fw_ds.L_addr_reg = 0x0;
        my_fw_ds.H_addr_reg = 0x0;
        my_fw_ds.Rule_reg = 0x0;

        rc = ioctl(fd, IOCTL_READ_FW_PORT, &my_fw_ds);  //ioctl call for read complex setup data. Use of ioread32 function

	if(rc < 0) {
                printf("readCFW Complex (note: same IOCTL as simple): failed = %d, meaning: %s \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
printf("readCFW: checking simple setup - input_port:%u, L/H_addr_reg:(%x %x) => Rule_reg:%x (rc:%d) \n", my_fw_ds.input_port, my_fw_ds.L_addr_reg, my_fw_ds.H_addr_reg, my_fw_ds.Rule_reg, rc);
#endif
	
	close(fd); //close firewall module
}


/* Function for direct read/write access to BRAM.
 * This function can be called only from system Admin
 */
int accessBram(unsigned int op_code, unsigned int bram_no, unsigned int addr_reg, unsigned int *data){
	int fd, rc;
	struct direct_access_ds my_direct_access_ds; // direct access struct

	/* Set parameter restrictions */
	if (bram_no <= 0 || bram_no > 4){
		printf("accessBram: Invalid value for output port.\n");
		exit(1);
	}
	if (op_code < 0 || op_code > 1){
		printf("accessBram: Invalid value for operation code.\n");
		exit(1);
	}

        fd = open(nodename,  O_RDWR, "u+rwx,g+rwx,o+rwx"); //open  firewallemodule

	/* fill up direct_access data structure */
        my_direct_access_ds.bram_no = bram_no;
        my_direct_access_ds.addr = addr_reg;
	my_direct_access_ds.op_code = op_code;
	if (op_code == 0x1) // read option
		my_direct_access_ds.data = 0; // always initialize data before read operation
        else
		my_direct_access_ds.data = *data; 

#ifdef FW_USER_LOGS
if (op_code == 0x0)
printf("accessBram: TRY_W - BRAM%d[%x)<=%x (in decimal %u) \n", bram_no, addr_reg, my_direct_access_ds.data, my_direct_access_ds.data);
else
printf("accessBram: TRY_R - BRAM%d[%x)=>%x (INIT in decimal %u) \n", bram_no, addr_reg, my_direct_access_ds.data, my_direct_access_ds.data);
#endif

        rc = ioctl(fd, IOCTL_DIRECT_ACCESS_BRAM, &my_direct_access_ds); // ioctl call for direct_access to BRAM (use iowrite32 & ioread32)


	if(rc < 0) {
                printf("accessBram: (note: same IOCTL as simple): failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

	if (op_code == 0x1) // read option
		*data = my_direct_access_ds.data;

#ifdef FW_USER_LOGS
if (op_code == 0x0)
printf("accessBram: W - BRAM%d[%x)<=%x (in decimal %u) - (rc:%d) \n", bram_no, addr_reg, *data, *data, rc);
else
printf("accessBram: R - BRAM%d[%x)=>%x (in decimal %u) - (rc:%d) \n", bram_no, addr_reg, *data, *data, rc);
#endif

        close(fd); //close firewall module

	return rc;

}

/* Function for read/write access to BRAM via FW.
   This function can be called both from sytem Admin and clinics when opcode=0x1 (read)
*/
int accessBramFW(unsigned int op_code, unsigned int inport, unsigned int outport, 
							unsigned int addr_reg, unsigned int *data){
	int fd, rc;
        struct access_ds my_access_ds;

	/* Set parameter restrictions */
	if (inport <= 0 || inport > 4){
		printf("accessBramFW: Invalid value for input port:%d - must be 1, 2, 3 or 4 \n", inport);
		exit(1);
	}
	if (outport <= 0 || outport > 4){
		printf("accessBramFW: Invalid value for output port.\n");
		exit(1);
	}
	if (op_code < 0 || op_code > 1){
		printf("accessBramFW: Invalid value for operation code.\n");
		exit(1);
	}

        fd = open(nodename,  O_RDWR, "u+rwx,g+rwx,o+rwx"); //open firewall module

	/* fill up firewall access data structure */
        my_access_ds.input_port = inport;
        my_access_ds.output_port = outport;
        my_access_ds.addr = addr_reg;
	my_access_ds.op_code = op_code;

	if (op_code == 0x1) // read option
		my_access_ds.data = 0; // always initialize data before read operation
        else
		my_access_ds.data = *data;

#ifdef FW_USER_LOGS
if (op_code == 0x0)
printf("accessBramFW: TRY_W - (input_port:%x) - BRAM%d[%x) <= %x (in decimal %u) \n", inport, outport, addr_reg, my_access_ds.data, my_access_ds.data);
else
printf("accessBramFW: TRY_R - (input_port:%x) - BRAM%d[%x) => %x (INIT in decimal %u) \n", inport, outport, addr_reg, my_access_ds.data, my_access_ds.data);
#endif

        rc = ioctl(fd, IOCTL_ACCESS_BRAM, &my_access_ds); // ioctl call for firewall access to BRAM (use iowrite32 & ioread32)

	if(rc < 0) {
                printf("accessBramFW: (note: same IOCTL as simple): failed = %d, meaning: %s \n", rc, strerror(errno));
		exit(1);
        }

	if (op_code == 0x1) // read option
		*data = my_access_ds.data;

#ifdef FW_USER_LOGS
if (op_code == 0x0)
printf("accessBramFW: W - (input_port:%d) - BRAM%d[%x) <= %x (in decimal %u) - (rc:%d)\n", inport, outport, addr_reg, my_access_ds.data, my_access_ds.data, rc);
else
printf("accessBramFW: R - (input_port:%d) - BRAM%d[%x) => %x (in decimal %u) - (rc:%d) \n", inport, outport, addr_reg, my_access_ds.data, my_access_ds.data, rc);
#endif

        close(fd); //close firewall module

	return rc;

}


/* Function for ioctl call to read the total packets passed per port
 * This function must be called from admin.
 */
unsigned int readStatsTotalPerPort(unsigned int inport){
	int fd, rc;
        struct stat_ds my_stat_ds; //firewall  statistics struct

	/* Set parameter restrictions */
	if (inport <= 0 || inport > 4){
		printf("readStatsTotalPerPort: Invalid value for input port:%d - must be 1, 2, 3 or 4\n", inport);
		exit(1);
	}
	
        fd = open(nodename, O_RDWR); //open firewall module

        /* fill up Total_passed field of statistics data structure */
	my_stat_ds.port = inport;
	my_stat_ds.Total_passed = 0x0;	

	rc = ioctl(fd, IOCTL_READ_STATS, &my_stat_ds); //ioctl call for reading total passed packets

        if(rc < 0) {
                printf("readStatsTotalPerPort: ioctl failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
        printf("readStatsTotalPerPort: (input_port:%d) - my_stat_ds.Total_passed = %d (rc:%d)\n", my_stat_ds.port, my_stat_ds.Total_passed, rc);
#endif

        close(fd); //close firewall module

        return my_stat_ds.Total_passed;

}

/* Function for ioctl call to read the dropped packets from port(due to Fifo full error)
 * This function must be called from admin.
 */
unsigned int readStatsFifoPerPort(unsigned int inport)
{
	int fd, rc;
        struct stat_ds my_stat_ds;

	/* Set parameter restrictions */
	if (inport <= 0 || inport > 4){
		printf("readStatsFifoPerPort: Invalid value for input port:%d - must be 1, 2, 3 or 4\n", inport);
		exit(1);
	}

        fd = open(nodename, O_RDWR); //open firewall module

        /* fill up Fifo_dropped field of statistics data structure */
        my_stat_ds.port = inport;
        my_stat_ds.Fifo_dropped = 0x0;

        rc = ioctl(fd, IOCTL_READ_STATS, &my_stat_ds); //ioctl call for read the dropped packets

        if(rc < 0) {
                printf("readStatsFifoPerPort: ioctl failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
        printf("readStatsFifoPerPort: (input_port:%d) - my_stat_ds.Fifo_dropped:%d (rc:%d)\n", my_stat_ds.port, my_stat_ds.Fifo_dropped, rc);
#endif

        close(fd); //close firewall module

        return my_stat_ds.Fifo_dropped;
}

/* Function for ioctl call to read the droped packets from port (due to firewall rule)
 * This function must be called from admin
 */
unsigned int readStatsFwPerPort(unsigned int inport) {
	 int fd, rc;
        struct stat_ds my_stat_ds;

	/* Set parameter restrictions */
	if (inport <= 0 || inport > 4){
		printf("readStatsFwPerPort: Invalid value for input port:%d - must be 1, 2, 3 or 4\n", inport);
		exit(1);
	}

        fd = open(nodename, O_RDWR); //open firewall module

        /* fill up Fw_dropped field of statistics data structure */
        my_stat_ds.port = inport;
        my_stat_ds.Fw_dropped = 0x0;

	//ioctl call for reading dropped firewall packets
        rc = ioctl(fd, IOCTL_READ_STATS, &my_stat_ds);

        if(rc < 0) {
                printf("readStatsFwPerPort: ioctl failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
        printf("readStatsFwPerPort: (input_port:%d) - my_stat_ds.Fw_dropped:%d (rc:%d) \n", my_stat_ds.port, my_stat_ds.Fw_dropped, rc);
#endif

        close(fd); //close firewall module

        return my_stat_ds.Fw_dropped;
}

/* Function for ioctl call to read the total packets passed from all ports (port 1-4) 
 * This function must be called from admin
 */
unsigned int readStatsTotal(){
	int fd, rc;
        struct stat_ds my_stat_ds; //firewall statistics stuct

        fd = open(nodename, O_RDWR); //open firewall module

        /* fill up Total_passed field of statistics data structure */
        my_stat_ds.Total_passed = 0x0;

           rc = ioctl(fd, IOCTL_READ_STATS_ALL, &my_stat_ds);

        if(rc < 0) {
                printf("readStatsTotal: ioctl failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
        printf("readStatsTotal: my_stat_ds.Total_passed:%d (rc:%d) \n", my_stat_ds.Total_passed, rc);
#endif

	close(fd); //close firewall module

        return my_stat_ds.Total_passed;
}

/* Function for ioctl call to read the dropped packets(due to Fifo full error) passed from all ports( port 1-4) 
 * This function must be called from admin
 */
unsigned int readStatsFifo(){
	int fd, rc;
        struct stat_ds my_stat_ds; //firewall statistics struct

        fd = open(nodename, O_RDWR); //open firewall module

        /* fill up Fifo_dropped field of statistics data structure */
        my_stat_ds.Fifo_dropped = 0x0;

        rc = ioctl(fd, IOCTL_READ_STATS_ALL, &my_stat_ds);

        if(rc < 0) {
                printf("readStatsFifo: ioctl failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
        printf("readStatsFifo: my_stat_ds.Fifo_dropped:%d (rc:%d) \n", my_stat_ds.Fifo_dropped, rc);
#endif

	close(fd); //close firewall module

        return my_stat_ds.Fifo_dropped;
}

/* Function for ioctl call to read the dropped packets (due to Firewall) from all ports (ports 1-4) 
 * This function must be called from admin
 */
unsigned int readStatsFw(){
	int fd, rc;
        struct stat_ds my_stat_ds;

        fd = open(nodename, O_RDWR); //open firewall module

        /* fill up Fw_dropped field of statistics data structure */
        my_stat_ds.Fw_dropped = 0x0;

        rc = ioctl(fd, IOCTL_READ_STATS_ALL, &my_stat_ds);

        if(rc < 0) {
                printf("readStatsFw: ioctl failed (rc:%d, meaning:%s) \n", rc, strerror(errno));
		exit(1);
        }

#ifdef FW_USER_LOGS
        printf("readStatsFw: my_stat_ds.Fw_dropped:%d (rc:%d) \n", my_stat_ds.Fw_dropped, rc);
#endif

	close(fd); //close firewall module

        return my_stat_ds.Fw_dropped;
}
