#include <iostream>
using namespace std;

#include "receiver.h"


void receiver:: entry()
{
  unsigned int new_address;
  sc_uint<32> temp_val1;
  sc_uint<32> temp_val2;
  sc_uint<32> temp_val3;
  sc_uint<32> temp_val4;
  sc_uint<32> temp_val5;
  sc_uint<32> temp_val6;
  bool indone = false;
  while(true)
    {
	   wait();
	   if (ready.read()) {
		   if (!indone){
			   new_address = out_addr_to_read.read();
			   temp_val1 = pkt_in->read(new_address);
			   temp_val2 = count.read();
			   temp_val3 = dropped.read();
			   temp_val4 = fw_hreg.read();
			   temp_val5 = fw_lreg.read();
			   temp_val6 = fw_rreg.read();
			   cout << "                                  ......."<< name() <<"....." << endl;
			   cout << "                                  New Packet Received" << endl;
			   cout << "                                  Receiver ID: " << (unsigned int)sink_id.read() + 1 << endl;
			   cout << "                                  Packet Address Offset: 0x" << hex << new_address << dec << endl;
			   cout << "                                  Packet Value: " << temp_val1.to_string(SC_HEX) << endl;
			   cout << "                                  Count: " << (unsigned int) temp_val2 << endl;
			   cout << "                                  Dropped: " << (unsigned int) temp_val3 << endl;
			   cout << "                                  FW H Reg: 0x" << hex << temp_val4 << dec << endl;
			   cout << "                                  FW L Reg: 0x" << hex <<  temp_val5 << dec << endl;
			   cout << "                                  FW R Reg: 0x" << hex <<  temp_val6 << dec << endl;
			   cout << "                                  .........................." << endl;
			   indone = true;
			   done.write(true);
		   }
	   }else{
		   indone = false;
		   done.write(false);
	   }
    }
}
