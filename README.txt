Xilinx - Open Hardware 2018 
Team number: xohw18-187
Project name: Hierarchical security primitives based on a Network-on-Chip
Date: June 2018
Version of uploaded archive: version 1

University name: Technological Educational Institute of Crete, Greece
Supervisor name: Miltos D. Grammatikakis
Supervisor e-mail: mdgramma@cs.teicrete.gr 
Participant(s): Voula Piperaki, Angelos Mouzakitis
Category: student
Email: dreams@cs.teicrete.gr

Board used: Zedboard Z7020 FPGA
Vivado Version: 2014.4.1

Brief description of project:
 We develop, implement and validate hierarchical GNU/Linux security primitives
 on top of a hardware Network-on-Chip (NoC) Firewall module embedded on the
 ARM-based Xilinx Zedboard FPGA. Our open source multi-layer design enables 
 modularity and reuse across different use cases, and interfaces to system 
 tools for high-level security visualization and notification services. 
 Focusing, data privacy and anonymity in patient data for ECG application.
 We demonstrate how mid-level driver layer can be extended to implement 
 high-level system security primitives for supporting data privacy and anonymity.
 Experimental results on Zedboard demonstrate low overhead of our security primitives.

 
Description of archive (explain directory structure, documents and source files):
	- DOC__USER_AND_REF_MANUAL/ : contains project report
	- HLS__NOC_FW_MODEL_SYSTEMC/ : systemC source files of the NoC firewall
	- hw_bitstream : include the final bitstream for the design
	- IP__FROM_VIVADO/ : Generated IP from HLS
	- SDCARD__HOWTO/SD_CARD_BOOT_PARTITION: includes all files required too boot from an SD card
	- SDCARD__HOWTO/ SD_CARD_FILE_SYSTEM_PARTITION: refers to the rootfs
	- SW__HIER_LINUX_DRIVER_AND_TESTBENCHES_REUSE: contains driver and application software source code

	
Instructions to build and test project

# Run the following steps as root
# -----   Step 1: Format the SD card, enumerated as /dev/sdc  -------------
fdisk /dev/sdc

# Delete all previous partitions
o

# Create the boot partition
n

# Create primary partition
p

# Select the partition number
1

# Select the first sector
8192

# Create a partition with 64M size
+64M

# Change the partition type to 'W95 FAT32'
t
c

# Create the second partition (rootfs)
n
p
2
139264
[ press enter ]

# Write and Quit
w


# Step 2: ---------  Format the partitions  ----------------

mkfs.vfat /dev/sdc1
mkfs.ext4 /dev/sdc2

# Step 3: --------- Mount and extract the root file-system -------------
# Export the 'SDCARD__HOWTO/SD_CARD_BOOT_PARTITION' and
#  'SW__HIER_LINUX_DRIVER_AND_TESTBENCHES_REUSE' directories environment variables
export APPL_DIR=</path/to/SW__HIER_LINUX_DRIVER_AND_TESTBENCHES_REUSE>
export SD_IMAGE=</path/to/SDCARD__HOWTO/SD_CARD_BOOT_PARTITION>

mkdir /mnt/rootfs
mount /dev/sdc2 /mnt/rootfs
cd /mnt/rootfs
wget http://releases.linaro.org/debian/images/alip-armhf/latest/linaro-jessie-alip-20161117-32.tar.gz
tar xf linaro-jessie-alip-20161117-32.tar.gz
mv binary/* .
rmdir binary
cp -r $APPL_DIR/fw_root root
mkdir home/clinic1
cp -r $APPL_DIR/fw_clinic1 home/clinic1
cd ..
umount /mnt/rootfs

# Mount and copy the boot files
mkdir /mnt/bootfs
mount /dev/sdc1 /mnt/bootfs
cp $SD_IMAGE/* /mnt/bootfs
umount /mnt/bootfs


# Step 4: Boot the board with the SD card and run the following commands as root.

# Create the groups
groupadd fwgroup
groupadd fwgroup1
groupadd fwgroup2
useradd -m clinic -g fwgroup -s /bin/bash 
useradd -m clinic2 -g fwgroup2 -s /bin/bash 
useradd clinic1 -g fwgroup1 -s /bin/bash 
chgrp fwgroup1 /home/clinic1
chown -R clinic1 /home/clinic1
---Give passowords to users----
passwd root
passwd clinic1
	
# Step 5: ---------  Allow root to ssh  --------------

sed -i 's/without.password/yes/g' /etc/ssh/sshd_config
sed -i 's/StrictModes.yes/StrictModes no/g' /etc/ssh/sshd_config
echo /usr/local/lib >> /etc/ld.so.conf
ldconfig


# Step 6: -------- Build the firewall Linux driver ------------

# Option 1: (faster): Use the precompiled firewall module.
# Option 2: Download and build the kernel, then build the firewall module

# Option 1:
mount /dev/mmcblk0p1 /boot
insmod  /boot/kernel_mode.ko


# Option 2:

# Clone the analogsources:
apt-get update
apt-get install device-tree-compiler u-boot-tools
cd root 
wget https://github.com/analogdevicesinc/linux/archive/2015_R1.zip
unzip 2015_R1.zip
cd linux-2015_R1
mount /dev/mmcblk0p1 /boot
cp /boot/analog-dotconfig .config
make oldconfig
make -j2

# Build the firewall kernel module
cd /root/fw_root/LINUX_KERNEL_MODULE
make
make install

# Step 7: ----------------------   run the application   -----------------------
cd /root/fw_root/TESTBENCHES/05_SYSTEMADMIN_SETUP
# see the README.txt


Link to YouTube Video(s): https://youtu.be/rYdyZacwYRw
