
/* The header file that contains ioctl macros and the data structures.
 * Used both in Firewall's Driver (kernel space) and in Firewall's API (user space).
 */

#include <linux/ioctl.h>
/* Define simple commands with _IO. Returned an integer e.g 1, 2, 3 that be 
 * unique for _IOR & _IOW
 */
#define MYIOC_TYPE 'k'
#define MYIOC_WRITEONE    _IO(MYIOC_TYPE, 1)    //command one (WRITEONE)
#define MYIOC_WRITETWO    _IO(MYIOC_TYPE, 2)    //command two (WRITETWO)
#define MYIOC_WRITETHREE  _IO(MYIOC_TYPE, 3)    //command three (WRITETHREE)

#define MYIOC_READONE     _IO(MYIOC_TYPE, 1)    //command one (READONE)
#define MYIOC_READTWO     _IO(MYIOC_TYPE, 2)    //command two (READTWO)
#define MYIOC_READTHREE   _IO(MYIOC_TYPE, 3)    //command three (READTHREE) 
#define MYIOC_READFOUR    _IO(MYIOC_TYPE, 4)    //command four (READFOUR)

/* Define commands or macros for generating request code that work from user space. This
 * works by putting a pointer to the appropriate data-structure in the IOCTL call
 * e.g in user_mode file -->rc = ioctl(fd, IOCTL_READ_SFW_PORT, &my_fw_ds); 
 * _IOW = creates an ioctl code to write data (passing data from user space to kernel space)
 * _IOR = creates an ioctl code to read data (return data to user space)
 */

/* Arguments in ioctl call:
 * First argument: is a magic number that tells to kernel which driver to call
 * Second argument: is an integer with unique value called command id that tells the driver
 * 		    the choosen command in order to execute the corresponding action
 * Third argument: is the data type we want to pass(usually structures to group many data)
 * more informations: https://static.lwn.net/images/pdf/LDD3/ch06.pdf
 */
/* compose a unique ioctl number to setup firewall in specific port */
#define IOCTL_SETUP_FW_PORT _IOW(MYIOC_TYPE, MYIOC_WRITEONE, struct fw_ds)

/* compose a unique ioctl number to check the setup of firewall in specific port */
#define IOCTL_CHECK_FW_PORT _IOR(MYIOC_TYPE, MYIOC_READTWO, struct fw_ds)

/* compose a unique ioctl number to read the setup of firewall in specific port */
#define IOCTL_READ_FW_PORT _IOR(MYIOC_TYPE, MYIOC_READONE, struct fw_ds)

/* compose a unique ioctl number to read firewall statistics in specific port */
#define IOCTL_READ_STATS _IOR(MYIOC_TYPE, MYIOC_READTHREE, struct stat_ds)

/* compose a unique ioctl number to read firewall statistics on all ports totally */
#define IOCTL_READ_STATS_ALL _IOR(MYIOC_TYPE, MYIOC_READFOUR, struct stat_ds)

/* compose a unique ioctl number to access the bram via FW (switch) */
#define IOCTL_ACCESS_BRAM _IOW(MYIOC_TYPE, MYIOC_WRITETWO, struct access_ds)

/* compose a unique ioctl number to direclty access bram */
#define IOCTL_DIRECT_ACCESS_BRAM _IOW(MYIOC_TYPE, MYIOC_WRITETHREE, struct direct_access_ds)
          
/* define firewall's setup struct */
struct fw_ds {
	unsigned int op_code;
        unsigned int input_port;
	unsigned int output_port;
	unsigned int L_addr_reg;
        unsigned int H_addr_reg;
        unsigned int Rule_reg;
};
/* define firewall's statistics structure */
struct stat_ds {
	unsigned int port;
        unsigned int Total_passed;
        unsigned int Fifo_dropped;
        unsigned int Fw_dropped;
};
/* define firewall's access structurei */
struct access_ds {
	unsigned int op_code;
	unsigned int input_port;
	unsigned int output_port;
	unsigned int addr;
	unsigned int data;
};
/* define firewall's direct read/write access to BRAM */
struct direct_access_ds {
	unsigned int op_code;
	unsigned int bram_no;
	unsigned int addr;
	unsigned int data;
};
