/* ------------------------------------------------------------
 * Low & Mid Level driver API.
 *
 * Created by V. Piperaki, A. Mouzakitis, and M.D Grammatikakis
 *
 * TEI of Crete <dreams@cs.teicrete.gr>
 *
 * 2017 - 2018 
* ---------------------------------------------------------- */

//#define FW_KERN_LOGS
//#define FW_KERN_LOGS2

#ifdef FW_KERN_LOGS
#define fw_pr_info(fmt, ...)   pr_info(fmt, ##__VA_ARGS__)
#else
#define fw_pr_info(fmt, ...)
#endif

#include "init_modules.h" //firewall device driver header file
#include "ioctl_fw.h"    // ioctl macros and data structures header file
#include <linux/cred.h>

/* From hardware_info.pdf
 * G. Firewall Setup address Mapping for switch ports
 * The offsets 5, 13, 21  (below) relate to the address of NoC Firewall,
 * i.e. address mapping for switch ports (last two bits),
 * (14hex-->20dec/4=5, 34hex-->52dec/4=13, 54hex-->84dec/4=21)
 */
static int startingLowerReg = 5; // lower range register
static int startingHighReg = 13; // high range register
static int startingRuleReg = 21; // rule register

/* Function to setup a firewall rule.
 * This function called from user space to define a 
 * simple firewall rule (e.g setup an input port,the absolute range
 * (L_addr_reg- H_addr_reg etc) or *  - complex firewall rule (e.g setup
 * input port, an outport, the relative range( L_addr_reg - H_addr_reg etc). 
 */
static void setupFW(struct fw_ds *my_fw_ds_ptr)
{
	int offset = 2;
	unsigned int inport = my_fw_ds_ptr->input_port;
	unsigned int L_addr_reg = my_fw_ds_ptr->L_addr_reg;
	unsigned int H_addr_reg = my_fw_ds_ptr->H_addr_reg;
	unsigned int Rule_reg = my_fw_ds_ptr->Rule_reg;
	int gid;

	gid = current_gid().val;
	if (gid == 0) {
		fw_pr_info("setupFW:GO_ON  UID:%u GID:%u my_fw_ds_ptr->input_port: %d  my_fw_ds_ptr->L_addr_reg:0x%x  my_fw_ds_ptr->H_addr_reg:0x%x   my_fw_ds_ptr->Rule_reg:0x%x (w=0x90, r=0xA0, wr=0x80, none=0xB0)\n", current_uid().val, current_gid().val, my_fw_ds_ptr->input_port, my_fw_ds_ptr->L_addr_reg, my_fw_ds_ptr->H_addr_reg, my_fw_ds_ptr->Rule_reg);

		/* write above data to kernel space 
		 * written by A. Papagrigoriou 
		 */
		iowrite32(0x00000000, noc_setup_registers + startingRuleReg + ((inport-1)*offset) ); //rule register
		iowrite32(L_addr_reg, noc_setup_registers + startingLowerReg + ((inport-1)*offset) ); //lower register
		iowrite32(H_addr_reg, noc_setup_registers + startingHighReg + ((inport-1)*offset) ); //high register
		iowrite32(Rule_reg, noc_setup_registers + startingRuleReg + ((inport-1)*offset) ); //rule register
	} else {
		fw_pr_info("setupFW:STOP!  UID:%u GID:%u my_fw_ds_ptr->input_port: %d  my_fw_ds_ptr->L_addr_reg:0x%x  my_fw_ds_ptr->H_addr_reg:0x%x   my_fw_ds_ptr->Rule_reg:0x%x (w=0x90, r=0xA0, wr=0x80, none=0xB0)\n", current_uid().val, current_gid().val, my_fw_ds_ptr->input_port, my_fw_ds_ptr->L_addr_reg, my_fw_ds_ptr->H_addr_reg, my_fw_ds_ptr->Rule_reg);
		fw_pr_info("setupFW: gid:%d", (int)gid);

		pr_info("NON-ROOT TRIED TO PERFORM setupFW");
		BUG_ON((gid != 0));
	}
}

/* Function to read firewall setup data.
 * This function called from user space to return 
 *  - the simple firewall rule (e.g setup an input port,the absolute range(L_addr_reg- H_addr_reg etc) or 
 *  - or complex firewall rule (e.g setup an input port, an outport, the relative range( L_addr_reg - H_addr_reg etc). 
 */
static void readFWRegs(struct fw_ds *my_fw_ds_ptr)
{
	int offset = 2;
	int inport = my_fw_ds_ptr->input_port;
	int gid;

	gid = current_gid().val;
	if (gid == 0) {
		my_fw_ds_ptr->L_addr_reg = 0x0;
		my_fw_ds_ptr->H_addr_reg = 0x0;
		my_fw_ds_ptr->Rule_reg = 0x0;
		/* Retrieve data from kernel space and eventually to user space. */ 
		my_fw_ds_ptr->L_addr_reg = ioread32(noc_setup_registers + startingLowerReg + ((inport-1)*offset) );
		my_fw_ds_ptr->H_addr_reg = ioread32(noc_setup_registers + startingHighReg + ((inport-1)*offset) );
		my_fw_ds_ptr->Rule_reg = ioread32(noc_setup_registers + startingRuleReg + ((inport-1)*offset) );

		fw_pr_info("readFWRegs:GO_ON  my_fw_ds_ptr->input_port: %d\n   my_fw_ds_ptr->L_addr_reg: 0x%x\n   my_fw_ds_ptr->H_addr_reg: 0x%x\n   my_fw_ds_ptr->Rule_reg: 0x%x\n",  my_fw_ds_ptr->input_port, my_fw_ds_ptr->L_addr_reg, my_fw_ds_ptr->H_addr_reg, my_fw_ds_ptr->Rule_reg);

	} else {
		fw_pr_info("readFWRegs:STOP!  my_fw_ds_ptr->input_port: %d\n   my_fw_ds_ptr->L_addr_reg: 0x%x\n   my_fw_ds_ptr->H_addr_reg: 0x%x\n   my_fw_ds_ptr->Rule_reg: 0x%x\n",  my_fw_ds_ptr->input_port, my_fw_ds_ptr->L_addr_reg, my_fw_ds_ptr->H_addr_reg, my_fw_ds_ptr->Rule_reg);
		fw_pr_info("readFWRegs: gid:%d", (int)gid);

		pr_info("NON-ROOT TRIED TO PERFORM readFWRegs");
		BUG_ON((gid != 0));
	}
}

/* 
 * Function to  validate a firewall rule
 * Compares the initial data with these after call read method and return the outcome.
 */
static int checkFWRegs(struct fw_ds *my_fw_ds_ptr)
{
        struct fw_ds my_fw_ds_temp;
	int gid;
	int flag = 0;

	gid = current_gid().val;
	if (gid == 0) {
		my_fw_ds_temp.input_port = my_fw_ds_ptr->input_port;
		my_fw_ds_temp.L_addr_reg = my_fw_ds_ptr->L_addr_reg;
		my_fw_ds_temp.H_addr_reg = my_fw_ds_ptr->H_addr_reg;
		my_fw_ds_temp.Rule_reg = my_fw_ds_ptr->Rule_reg;

		fw_pr_info("checkFWRegs:GO_ON (input_port:%d) given_values - L_addr_reg:%x H_addr_reg:%x Rule_reg:%x\n", my_fw_ds_temp.input_port, my_fw_ds_temp.L_addr_reg, my_fw_ds_temp.H_addr_reg, my_fw_ds_temp.Rule_reg);
//
		// check initial data with data from readFWRegs method
		readFWRegs(my_fw_ds_ptr);

		fw_pr_info("checkFWRegs:GO_ON (input_port:%d)  actual_values - L_addr_reg:%x H_addr_reg:%x Rule_reg:%x\n", my_fw_ds_ptr->input_port, my_fw_ds_ptr->L_addr_reg, my_fw_ds_ptr->H_addr_reg, my_fw_ds_ptr->Rule_reg);

		if ((my_fw_ds_temp.L_addr_reg==my_fw_ds_ptr->L_addr_reg) 
		&&  (my_fw_ds_temp.H_addr_reg==my_fw_ds_ptr->H_addr_reg) 
		&&  (my_fw_ds_temp.Rule_reg==my_fw_ds_ptr->Rule_reg) ) {
        		flag = 1;
			return flag;
		}
        	else {
        		flag = -1;
			return flag;
		}
	} else {

		fw_pr_info("checkFWRegs:STOP! (input_port:%d) given_values - L_addr_reg:%x H_addr_reg:%x Rule_reg:%x\n", my_fw_ds_temp.input_port, my_fw_ds_temp.L_addr_reg, my_fw_ds_temp.H_addr_reg, my_fw_ds_temp.Rule_reg);
		fw_pr_info("checkFWRegs: gid:%d", (int)gid);

		pr_info("NON-ROOT TRIED TO PERFORM checkFWRegs");
		BUG_ON((gid != 0));
	}
	return 0;
}
/* Function to write or read data to a block (specific range) directly to BRAM.   
 * This function called from user space to write (op_code 0) data to kernel space
 * or read (op_code 1) data from kernel space.
 * Written by Antonis Papagrigoriou 
 */
static int accessBram(struct direct_access_ds *my_direct_access_ds_ptr)
{
	unsigned int test_data;
	unsigned int op_code = my_direct_access_ds_ptr->op_code;
	unsigned int bram_no = my_direct_access_ds_ptr->bram_no;
	unsigned int addr_reg = my_direct_access_ds_ptr->addr;
	unsigned int data = my_direct_access_ds_ptr->data;
	int gid;

	gid = current_gid().val;
	if ((gid != 0) &&  ((op_code == 0x0) || (op_code == 0x1))) { // only root group can perform direct read/write
		fw_pr_info("accessBram: gid:%d", (int)gid);

		pr_info("accessBram: NON-ROOT TRIED TO PERFORM direct access (r/w)\n");
		BUG_ON((gid != 0) &&  (op_code == 0x0));
	}

	if (op_code == 0x0){ // write data to kernel space directly
		switch(bram_no) {
	  	case 1:
             		iowrite32(data,iobram1_direct + addr_reg);

			fw_pr_info("accessBram: W - BRAM1[%x]<-%x\n", addr_reg, data);
			fw_pr_info("accessBram: R - BRAM1[%x]<-%x (for checking with W) \n", addr_reg, ioread32(iobram1_direct + addr_reg));

             		return 0; 

	  	case 2:	
             		iowrite32(data,iobram2_direct + addr_reg);

			fw_pr_info("accessBram: W - BRAM2[%x]<-%x\n", addr_reg, data);
			fw_pr_info("accessBram: R - BRAM2[%x]<-%x (for checking with W) \n", addr_reg, ioread32(iobram2_direct + addr_reg));

             		return 0;

	   	case 3:
             		iowrite32(data,iobram3_direct + addr_reg);
			fw_pr_info("accessBram: W - BRAM3[%x]<-%x\n", addr_reg, data);
			fw_pr_info("accessBram: R - BRAM3[%x]<-%x (for checking with W) \n", addr_reg, ioread32(iobram3_direct + addr_reg));
	     		return 0;

	  	case 4:
             		iowrite32(data,iobram4_direct + addr_reg);

			fw_pr_info("accessBram: W - BRAM4[%x]<-%x\n", addr_reg, data);
			fw_pr_info("accessBram: R - BRAM4[%x]<-%x (for checking with W) \n", addr_reg, ioread32(iobram4_direct + addr_reg));

	     		return 0;

		default:
			return -1;
		} 
	} else { // read data from kernel space directly
		switch(bram_no) {
	  	case 1:
	     		test_data = ioread32(iobram1_direct + addr_reg);
			fw_pr_info("accessBram: R - BRAM1[%x]->%x \n", addr_reg, test_data);

	     		my_direct_access_ds_ptr->data = test_data;
	     		return 0; 

	  	case 2:	
	     		test_data = ioread32(iobram2_direct + addr_reg);
			fw_pr_info(KERN_INFO "accessBram: R - BRAM2[%x]->%x \n", addr_reg, test_data);

	     		my_direct_access_ds_ptr->data = test_data;
             		return 0;

	   	case 3:
	     		test_data = ioread32(iobram3_direct + addr_reg);
			fw_pr_info(KERN_INFO "accessBram: R - BRAM3[%x]->%x \n", addr_reg, test_data);

	     		my_direct_access_ds_ptr->data = test_data;
	     		return 0;

	  	case 4:
	     		test_data = ioread32(iobram4_direct + addr_reg);
			fw_pr_info(KERN_INFO "accessBram: R - BRAM4[%x]->%x \n", addr_reg, test_data);

	     		my_direct_access_ds_ptr->data = test_data;
	     		return 0;

		default:
			return -1; // maybe already checked in regard to ioread/iowrite!
		} 
	}
}

/* Function to write or read data to a block (specific range) to BRAM via FW.   
 * This function called from user space to write (op_code 0) data to kernel space
 * or read (op_code 1) data from kernel  space.
 * Written by Antonis Papagrigoriou 
 */
static int accessBramFW(struct access_ds *my_access_ds_ptr)
{
	unsigned int write_value = 0x0; //hold validation data (check with ioread32 method) 
	volatile unsigned int test_data = 0;
	unsigned int print_data;
        unsigned int op_code = my_access_ds_ptr->op_code;
	unsigned int input_port = my_access_ds_ptr->input_port;
        unsigned int output_port = my_access_ds_ptr->output_port;
        unsigned int addr_reg = my_access_ds_ptr->addr;
        unsigned int data = my_access_ds_ptr->data;
	int gid;

#ifdef FW_KERN_LOGS2
	/* From hardware_info.pdf
         * 11 (below) comes from address of E. Read the dropped packets (due to Fifo full error) from port (last two bits),
         * 13 (below) comes from address of F. Read the dropped packets (due firewall rule) from port (last two bits),
 	 * (2Chex-->44dec/4=11, 34hex-->52dec/4=13)
         */
        int startingStatisticsFifo = 11; 
	int startingStatisticsFw = 13;  
	unsigned int Fifo_dropped = 0;
	unsigned int Fw_dropped = 0;
#endif

	gid = current_gid().val;
	if ((gid != 0) &&  (op_code == 0x0)) { // only root group can perform write via firewall
		fw_pr_info("accessBramFW: gid:%d", (int)gid);

		pr_info("accessBramFW: NON-ROOT TRIED TO PERFORM write access via FW \n");
		BUG_ON((gid != 0) &&  (op_code == 0x0));
	}

	if (op_code == 0x0){ // write data to kernel space via FW

		write_value = (addr_reg) << 24; // write address offset
		write_value += ((input_port - 1) << 18); // write source port
		write_value += ((output_port - 1) << 16); // write destination port
		write_value += data; // write command payload
		switch (input_port) {
		/* + 5 (below) comes from address of B.write packet register for each switch port (last two bits)
		 */
		  case 1:
                    iowrite32(write_value, (iobram1 + 5));
                    break;
                  case 2:
                    iowrite32(write_value, (iobram2 + 5));
                    break;
                  case 3:
                    iowrite32(write_value, (iobram3 + 5));
                    break;
                  case 4:
                    iowrite32(write_value, (iobram4 + 5));
                    break;
		}

	fw_pr_info("accessBramFW: W - BRAM%d[%x]<-%x input_port:%d\n", output_port, addr_reg, data, input_port);


#ifdef FW_KERN_LOGS2
Fw_dropped = 0x0;
Fifo_dropped = 0x0;
//read statistics  per port
if(input_port == 1){
Fifo_dropped = ioread32(iobram1 + startingStatisticsFifo) - init_stat_fifo1;
Fw_dropped = ioread32(iobram1 + startingStatisticsFw) - init_stat_fw1;
}else if(input_port == 2){
Fifo_dropped = ioread32(iobram2 + startingStatisticsFifo) - init_stat_fifo2;
Fw_dropped = ioread32(iobram2 + startingStatisticsFw) - init_stat_fw2;
}else if(input_port == 3){
Fifo_dropped = ioread32(iobram3 + startingStatisticsFifo) - init_stat_fifo3;
Fw_dropped = ioread32(iobram3 + startingStatisticsFw) - init_stat_fw3;
}else if(input_port == 4){
Fifo_dropped = ioread32(iobram4 + startingStatisticsFifo) - init_stat_fifo4;
Fw_dropped = ioread32(iobram4 + startingStatisticsFw) - init_stat_fw4;
}
pr_info("accessBramFW: W - readStatsPerPort: Fifo_dropped[%d]:%d Fw_dropped[%d]:%d \n",  input_port, Fifo_dropped, input_port, Fw_dropped);
#endif
	     	return 0;

		
	} else { // read data from kernel space via FW

		fw_pr_info("accessBramFW: TRY_R - BRAM%d[%x]->(data TBD) input_port:%d\n", output_port, addr_reg, input_port);


		write_value = (addr_reg) << 24; // write address offset
          	write_value += (0x1 << 20); // write the read opcode
          	write_value += ((input_port - 1) << 18); // write the source port
          	write_value += ((output_port - 1) << 16); // write the destination port
          	write_value += 0x0; // no  payload for read
		/* From hardware_info.pdf
		 * + 5 (below) comes from address of write packet register for each switch port (last two bits)
         	* (14hex-->20dec/4=5)
		 */
		switch (input_port) {
                  case 1:
                    	iowrite32(write_value, (iobram1 + 5));
                  	break;
                  case 2:
                    	iowrite32(write_value, (iobram2 + 5));
                  	break;
                  case 3:
                    	iowrite32(write_value, (iobram3 + 5));
                  	break;
                  case 4:
pr_info("accessBramFW: preparing packet - iowrite \n");
                    	iowrite32(write_value, (iobram4 + 5));
                  	break;
		}

		test_data = 0x80000000;
		while (test_data == 0x80000000) {
			switch (input_port) {
		/* From hardware_info.pdf
		 * + 7 (below) comes from address of read response register for each switch port
         	 * (1Chex-->28dec/4=7)
		 */
				case 1:
					test_data = ioread32((iobram1 + 7));
					break;
				case 2:
					test_data = ioread32((iobram2 + 7));
					break;
				case 3:
					test_data = ioread32((iobram3 + 7));
					break;
				case 4:
pr_info("accessBramFW: reading data - ioread \n");
					test_data = ioread32((iobram4 + 7));
					break;
			}
			set_current_state(TASK_INTERRUPTIBLE); // scheduler won't run same process again for CMD_DELAY
			schedule_timeout(CMD_DELAY);
		}

		print_data = (unsigned int) ((test_data << 16) | (test_data >> 16)); // chop last 16 bits for final 16-bit value
		print_data = (print_data) >> 16; //write address offset

		fw_pr_info("accessBramFW: R - BRAM%d[%x]->%x input_port:%d\n", output_port, addr_reg, test_data, input_port);
		fw_pr_info("accessBramFW: R - BRAM%d[%x]->%x (final 16-bit) input_port:%d \n", output_port, addr_reg, print_data, input_port);


		//save print_data to pass this value back to user_mode
		my_access_ds_ptr->data = print_data;

		return 0;

	}

}

/* Function to read/show statistics per port (e.g read total packets passed from port) 
 * This function called from user space to read specific statistics per port, i.e.
 * total packets passed, dropped packets (due to fifo full error) and dropped packets (due to firewall rule) per port. 
 */
static void readStatsPerPort(struct stat_ds *my_stat_ds_ptr)
{ 

	/* From hardware_info.pdf
	 * 9 (below) comes from address of D.Read the total packets passed from port (last two bits),
         * 11 (below) comes from address of E. Read the dropped packets (due to Fifo full error) from port (last two bits),
         * 13 (below) comes from address of F. Read the dropped packets (due firewall rule) from port (last two bits),
 	 * (24hex-->36dec/4=9, 2Chex-->44dec/4=11, 34hex-->52dec/4=13)
         */
        int startingStatisticsTotal = 9; 
        int startingStatisticsFifo = 11; 
        int startingStatisticsFw = 13;  

	int inport = my_stat_ds_ptr->port;
	int gid;

	gid = current_gid().val;
	if ((gid != 0)) { // only root group can access statistics
		fw_pr_info("readStatsPerPort: gid:%d", (int)gid);

		pr_info("NON-ROOT TRIED TO PERFORM readStatsPerPort");
		BUG_ON(gid != 0);
	}

	my_stat_ds_ptr-> Total_passed = 0x0;
	my_stat_ds_ptr-> Fifo_dropped = 0x0;
	my_stat_ds_ptr-> Fw_dropped = 0x0;
	//read statistics  per port
	if(inport == 1){
          my_stat_ds_ptr-> Total_passed = ioread32(iobram1 + startingStatisticsTotal) - init_stat_total1;
	  my_stat_ds_ptr-> Fifo_dropped = ioread32(iobram1 + startingStatisticsFifo) - init_stat_fifo1;
	  my_stat_ds_ptr-> Fw_dropped = ioread32(iobram1 + startingStatisticsFw) - init_stat_fw1;
	}else if(inport == 2){
	  my_stat_ds_ptr-> Total_passed = ioread32(iobram2 + startingStatisticsTotal) - init_stat_total2;
          my_stat_ds_ptr-> Fifo_dropped = ioread32(iobram2 + startingStatisticsFifo) - init_stat_fifo2;
          my_stat_ds_ptr-> Fw_dropped = ioread32(iobram2 + startingStatisticsFw) - init_stat_fw2;
	}else if(inport == 3){
          my_stat_ds_ptr-> Total_passed = ioread32(iobram3 + startingStatisticsTotal) - init_stat_total3;
          my_stat_ds_ptr-> Fifo_dropped = ioread32(iobram3 + startingStatisticsFifo) - init_stat_fifo3;
          my_stat_ds_ptr-> Fw_dropped = ioread32(iobram3 + startingStatisticsFw) - init_stat_fw3;
        }else if(inport == 4){
          my_stat_ds_ptr-> Total_passed = ioread32(iobram4 + startingStatisticsTotal) - init_stat_total4;
          my_stat_ds_ptr-> Fifo_dropped = ioread32(iobram4 + startingStatisticsFifo) - init_stat_fifo4;
          my_stat_ds_ptr-> Fw_dropped = ioread32(iobram4 + startingStatisticsFw) - init_stat_fw4;
        }

	fw_pr_info("readStatsPerPort: (input_port:%d) Total_passed:%d - Fifo_dropped:%d Fw_dropped:%d\n",  my_stat_ds_ptr->port, my_stat_ds_ptr->Total_passed, my_stat_ds_ptr->Fifo_dropped, my_stat_ds_ptr->Fw_dropped);

}

/* Function to read/show statistics for all ports totally (e.g read total packets passed from all ports) 
 * This function called from user space to read specific statistics for all ports, i.e.
 * total packets passed, dropped packets (due to fifo full error) and dropped packets (due to firewall rule) for all port. 
 */
static void readStatsPerAllPorts(struct stat_ds *my_stat_ds_ptr)
{
	int gid;
	 /* From hardware_info.pdf
         * 9 (below) comes from address of D.Read the total packets passed from port (last two bits),
         * 11 (below) comes from address of E. Read the dropped packets (due to Fifo full error) from port (last two bits),
         * 13 (below) comes from address of F. Read the dropped packets (due firewall rule) from port (last two bits),
         * (24hex-->36dec/4=9, 2Chex-->44dec/4=11, 34hex-->52dec/4=13)
         */
        int startingStatisticsTotal = 9; 
        int startingStatisticsFifo = 11; 
        int startingStatisticsFw = 13;  

	int totalPort1 = 0;
	int totalPort2 = 0;
	int totalPort3 = 0;
	int totalPort4 = 0;
	int fifoPort1 = 0;
	int fifoPort2 = 0;
	int fifoPort3 = 0;
	int fifoPort4 = 0;
	int fwPort1 = 0;
	int fwPort2 = 0;
	int fwPort3 = 0;
	int fwPort4 = 0;

        my_stat_ds_ptr-> Total_passed = 0x0;
        my_stat_ds_ptr-> Fifo_dropped = 0x0;
        my_stat_ds_ptr-> Fw_dropped = 0x0;

	gid = current_gid().val;
	if ((gid != 0)) { // only root group can access statistics
		fw_pr_info("readStatsPerAllPorts: gid:%d", (int)gid);

		pr_info("NON-ROOT TRIED TO PERFORM readStatsPerPort");
		BUG_ON(gid != 0);
	}

        /* read statistics per all ports (totally) */
        totalPort1 = ioread32(iobram1 + startingStatisticsTotal) - init_stat_total1;
        fifoPort1 = ioread32(iobram1 + startingStatisticsFifo) - init_stat_fifo1;
        fwPort1 = ioread32(iobram1 + startingStatisticsFw) - init_stat_fw1;

	totalPort2 = ioread32(iobram2 + startingStatisticsTotal) - init_stat_total2;
        fifoPort2 = ioread32(iobram2 + startingStatisticsFifo) - init_stat_fifo2;
        fwPort2 = ioread32(iobram2 + startingStatisticsFw) - init_stat_fw2;    

	totalPort3 = ioread32(iobram3 + startingStatisticsTotal) - init_stat_total3;
        fifoPort3 = ioread32(iobram3 + startingStatisticsFifo) - init_stat_fifo3;
        fwPort3 = ioread32(iobram3 + startingStatisticsFw) - init_stat_fw3; 

	totalPort4 = ioread32(iobram4 + startingStatisticsTotal) - init_stat_total4;
        fifoPort4 = ioread32(iobram4 + startingStatisticsFifo) - init_stat_fifo4;
        fwPort4 = ioread32(iobram4 + startingStatisticsFw) - init_stat_fw4; 

	my_stat_ds_ptr->Total_passed = totalPort1 + totalPort2 + totalPort3 + totalPort4;
	my_stat_ds_ptr->Fifo_dropped = fifoPort1 + fifoPort2 + fifoPort3 + fifoPort4;
	my_stat_ds_ptr->Fw_dropped = fwPort1 + fwPort2 + fwPort3 + fwPort4;

	fw_pr_info("readStatsPerAllPorts: Total_passed:%d - Fifo_dropped:%d Fw_dropped:%d\n",  my_stat_ds_ptr->Total_passed, my_stat_ds_ptr->Fifo_dropped, my_stat_ds_ptr->Fw_dropped);
}

/*
 * This function called by user to setup, read, write brams or stats
 * The kernel or driver device allows the userspace to send commandsi to kernel space.
 */
static long my_unlocked_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct fw_ds my_fw_ds;
	struct stat_ds my_stat_ds;
	struct access_ds my_access_ds;
	struct direct_access_ds my_direct_access_ds;
	
	int size, rc, rc1, rc2;
	void __user *ioargp = (void __user *)arg;

	if (_IOC_TYPE(cmd) != MYIOC_TYPE) {
		fw_pr_info("Got invalid case, CMD=%d\n", cmd);

		return -EINVAL;
	}

	size = _IOC_SIZE(cmd);
	
	switch (cmd) {
		case IOCTL_SETUP_FW_PORT:
			fw_pr_info("\n\nCALL IOCTL_SETUP_FW_PORT\n");

		   rc = copy_from_user(&my_fw_ds, ioargp, size);
		   setupFW(&my_fw_ds);
		   fw_pr_info("EXTT IOCTL_SETUP_FW_PORT\n\n");

		   return rc;
		case IOCTL_CHECK_FW_PORT:
		   fw_pr_info("\n\nCALL IOCTL_CHECK_FW_PORT\n");

		   rc = copy_from_user(&my_fw_ds, ioargp, size);
		   rc = checkFWRegs(&my_fw_ds);
		   rc = copy_to_user(ioargp, &my_fw_ds, size);
		   fw_pr_info("EXIT IOCTL_CHECK_FW_PORT\n\n");

		   return rc; // return code 1 -> check ok
		case IOCTL_DIRECT_ACCESS_BRAM:
		   fw_pr_info("\n\nCALL IOCTL_DIRECT_ACCESS_BRAM\n");

                  rc1 = copy_from_user(&my_direct_access_ds, ioargp, size);
                  accessBram(&my_direct_access_ds);
                  rc2 = copy_to_user(ioargp, &my_direct_access_ds, size);
		  fw_pr_info("EXIT IOCTL_DIRECT_ACCESS_BRAM\n\n");

		  return rc2; //return ((rc1 == 0) && (rc2 ==0));
		case IOCTL_ACCESS_BRAM:
		  fw_pr_info("\n\nCALL IOCTL_ACCESS_BRAM\n\n");

                  rc1 = copy_from_user(&my_access_ds, ioargp, size);
                  accessBramFW(&my_access_ds);
                  rc2 = copy_to_user(ioargp, &my_access_ds, size);
		  fw_pr_info("EXIT IOCTL_ACCESS_RAM\n\n");

		  return rc2; 
		case IOCTL_READ_FW_PORT:
		  fw_pr_info("\n\nCALL IOCTL_READ_FW_PORT\n");

		  rc = copy_from_user(&my_fw_ds, ioargp, size);
		  readFWRegs(&my_fw_ds);
		  rc = copy_to_user(ioargp, &my_fw_ds, size);
		  fw_pr_info("EXIT IOCTL_READ_FW_PORT\n\n");

		  return rc;
		case IOCTL_READ_STATS:
		  fw_pr_info("\n\nCALL IOCTL_READ_STATS\n");

		  rc = copy_from_user(&my_stat_ds, ioargp, size);
                  readStatsPerPort(&my_stat_ds);
                  rc = copy_to_user(ioargp, &my_stat_ds, size);
		  fw_pr_info("EXIT IOCTL_READ_STATS\n\n");

		  return rc;
		case IOCTL_READ_STATS_ALL:
		  fw_pr_info("\n\nCALL IOCTL_READ_STATS_ALL\n");

                  rc = copy_from_user(&my_stat_ds, ioargp, size);
		  readStatsPerAllPorts(&my_stat_ds);
		  rc = copy_to_user(ioargp, &my_stat_ds, size);
		  fw_pr_info("EXIT IOCTL_READ_STATS_ALL\n\n");

                  return rc;
		default:
		  fw_pr_info("Got invalid case, CMD=%d\n", cmd);

                  return -EINVAL;
	}
}

static struct file_operations pugs_fops = // file ops struct
{
	.owner = THIS_MODULE,
	.unlocked_ioctl = my_unlocked_ioctl,
	.open = my_open,
	.release = my_close
};

static struct miscdevice firewall_miscdevice = {
	.name = "sw_firewall",
	.fops = &pugs_fops,
	.nodename = "sw_firewall",
	.mode = 0777,
};

module_init(mfd_init);
module_exit(mfd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEI of Crete <dreams@cs.teicrete.gr>,  V. Piperaki, A.Mouzakitis  and M.D Grammatikakis");
MODULE_DESCRIPTION("Dreams Access BRAM memory module");
