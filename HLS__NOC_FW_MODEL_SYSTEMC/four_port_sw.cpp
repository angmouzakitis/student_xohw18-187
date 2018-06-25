#ifndef SYSTEM_C_H_INCLUDED
#define SYSTEM_C_H_INCLUDED
#define SC_INCLUDE_FX
#include "systemc.h"
#include "AXI4_if.h"
#include"tlm.h"
using namespace tlm;
#endif // SYSTEM_C_H_INCLUDED

#include "packet.h"
#include "four_port_sw.h"

void four_port_sw::setupfw_rule()
{
  unsigned int precommand_0 = 0x0;
  unsigned int precommand_1 = 0x0;
  unsigned int precommand_2 = 0x0;
  unsigned int precommand_3 = 0x0;
  fw_rule_range_0.write(0x0);
  fw_rule_range_1.write(0x0);
  fw_rule_range_2.write(0x0);
  fw_rule_range_3.write(0x0);
  fw_upper_range_0.write(0x0);
  fw_upper_range_1.write(0x0);
  fw_upper_range_2.write(0x0);
  fw_upper_range_3.write(0x0);
  fw_lower_range_0.write(0x0);
  fw_lower_range_1.write(0x0);
  fw_lower_range_2.write(0x0);
  fw_lower_range_3.write(0x0);

  wait();
  while(true)
  {
    wait();
    sc_uint<32> tmp_read_p0_0 = setup_rule_range_0.read();
    if (tmp_read_p0_0.to_uint() != precommand_0){
      sc_uint<32> tmp_read_p0_1 = setup_lower_range_0.read();  
      sc_uint<32> tmp_read_p0_2 = setup_upper_range_0.read();  
      precommand_0 = tmp_read_p0_0.to_uint();
      fw_rule_range_0.write(tmp_read_p0_0);
      fw_lower_range_0.write(tmp_read_p0_1);
      fw_upper_range_0.write(tmp_read_p0_2);
#ifndef __SYNTHESIS__
      cout << name() << ".p0: Setting Up FW rule: upper: 0x" << hex << tmp_read_p0_2.to_uint() << dec
           << " lower: 0x" << hex << tmp_read_p0_1.to_uint() << dec
           << " rule: 0x" << hex << tmp_read_p0_0.to_uint() << dec << endl;
#endif
    }
    sc_uint<32> tmp_read_p1_0 = setup_rule_range_1.read();
    if (tmp_read_p1_0.to_uint() != precommand_1){
      sc_uint<32> tmp_read_p1_1 = setup_lower_range_1.read();  
      sc_uint<32> tmp_read_p1_2 = setup_upper_range_1.read();  
      precommand_1 = tmp_read_p1_0.to_uint();
      fw_rule_range_1.write(tmp_read_p1_0);
      fw_lower_range_1.write(tmp_read_p1_1);
      fw_upper_range_1.write(tmp_read_p1_2);
#ifndef __SYNTHESIS__
      cout << name() << ".p1: Setting Up FW rule: upper: 0x" << hex << tmp_read_p1_2.to_uint() << dec
           << " lower: 0x" << hex << tmp_read_p1_1.to_uint() << dec
           << " rule: 0x" << hex << tmp_read_p1_0.to_uint() << dec << endl;
#endif
    }
    sc_uint<32> tmp_read_p2_0 = setup_rule_range_2.read();
    if (tmp_read_p2_0.to_uint() != precommand_2){
      sc_uint<32> tmp_read_p2_1 = setup_lower_range_2.read();  
      sc_uint<32> tmp_read_p2_2 = setup_upper_range_2.read();  
      precommand_2 = tmp_read_p2_0.to_uint();
      fw_rule_range_2.write(tmp_read_p2_0);
      fw_lower_range_2.write(tmp_read_p2_1);
      fw_upper_range_2.write(tmp_read_p2_2);
#ifndef __SYNTHESIS__
      cout << name() << ".p2: Setting Up FW rule: upper: 0x" << hex << tmp_read_p2_2.to_uint() << dec
           << " lower: 0x" << hex << tmp_read_p2_1.to_uint() << dec
           << " rule: 0x" << hex << tmp_read_p2_0.to_uint() << dec << endl;
#endif
    }
    sc_uint<32> tmp_read_p3_0 = setup_rule_range_3.read();
    if (tmp_read_p3_0.to_uint() != precommand_3){
      sc_uint<32> tmp_read_p3_1 = setup_lower_range_3.read();  
      sc_uint<32> tmp_read_p3_2 = setup_upper_range_3.read();  
      precommand_3 = tmp_read_p3_0.to_uint();
      fw_rule_range_3.write(tmp_read_p3_0);
      fw_lower_range_3.write(tmp_read_p3_1);
      fw_upper_range_3.write(tmp_read_p3_2);
#ifndef __SYNTHESIS__
      cout << name() << ".p3: Setting Up FW rule: upper: 0x" << hex << tmp_read_p3_2.to_uint() << dec
           << " lower: 0x" << hex << tmp_read_p3_1.to_uint() << dec
           << " rule: 0x" << hex << tmp_read_p3_0.to_uint() << dec << endl;
#endif
    }
  }
}

void four_port_sw::interface_in_0()
{
  unsigned int preval0 = 0x0;
  unsigned int prevalf = 0x0;
  unsigned int pre_read_l = 0x0;
  unsigned int pre_read_h = 0x0;
  unsigned int pre_read_r = 0x0;

  unsigned int fw_read_l = 0x0;
  unsigned int fw_read_h = 0x0;
  sc_uint<32> fw_read_r = 0x0;

  fw_hreg_p_0.write(0x0);
  fw_lreg_p_0.write(0x0);
  fw_rreg_p_0.write(0x0);

  count_total_0 = 0x0;
  rule_hit_counter_0 = 0x0;
  count_droped_0 = 0x0;
  total_0.write(0x0);
  droped_0.write(0x0);
  fw_rule_hit_counter_0.write(0x0);
  wait();
  while(true)
  {
    wait();
    sc_uint<32> new_fw0 = fw_rule_range_0.read();
    if (new_fw0.to_uint() != pre_read_r){
    	pre_read_r = new_fw0.to_uint();
    	fw_read_r = new_fw0;
    	fw_read_l = fw_lower_range_0.read().to_uint();
    	fw_read_h = fw_upper_range_0.read().to_uint();
    	//this is debug for verify
        fw_hreg_p_0.write(fw_read_h);
        fw_lreg_p_0.write(fw_read_l);
        fw_rreg_p_0.write(fw_read_r);
        continue;
    }
    sc_uint<32> new_data0 = in_0.read();
    if (new_data0.to_uint() != preval0){
      preval0 = new_data0.to_uint();
      count_total_0 = count_total_0 + 1;
      total_0.write(count_total_0);
#ifndef __SYNTHESIS__
      cout << name() << ".SW1.p0: Received data " << count_total_0 << ": 0x" << hex << new_data0.to_uint() << dec << endl;
#endif
      //check for firewall rule
      //Rule register (bit active - stands for):
      //7 -> 1: Enable Rule
      //6 -> 1: Enabled NoC Mode // 0: Enable Compatibility Mode
      //5 -> 1: Enable Rule for Write OpCode
      //4 -> 1: Enable Rule for Read OpCode
      //3 -> 1: Enable Rule for Destination Port 3
      //2 -> 1: Enable Rule for Destination Port 2
      //1 -> 1: Enable Rule for Destination Port 1
      //0 -> 1: Enable Rule for Destination Port 0
      bool fw_hit = false;
      if (fw_read_r[7].to_bool()){
    	  if ((fw_read_r[5].to_bool() && (new_data0.range(21,20).to_uint() == 0x0)) || //firewall for writeOp
      		  (fw_read_r[4].to_bool() && (new_data0.range(21,20).to_uint() == 0x1))){ //firewall for ReadOp
    		  sc_uint<32> my_address = new_data0.range(31,22).to_uint();
    		  if (fw_read_r[6].to_bool()){ //NoC Mod
    			  if ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >=  fw_read_l)){ //the range of address is between
    				  if ((fw_read_r[3].to_bool() && new_data0.range(17,16).to_uint() == 0x3) ||
    					  (fw_read_r[2].to_bool() && new_data0.range(17,16).to_uint() == 0x2) ||
      					  (fw_read_r[1].to_bool() && new_data0.range(17,16).to_uint() == 0x1) ||
      					  (fw_read_r[0].to_bool() && new_data0.range(17,16).to_uint() == 0x0)){
      							fw_hit = true;
    				  }
    			  }
    		  }else{ //compatibility mode
    			  //physical address mode
    			  if (new_data0.range(17,16).to_uint() == 0x0){ //go to port 00 -> 0
    				  my_address += 0x40000000;
    			  }else if (new_data0.range(17,16).to_uint() == 0x1){ //go to port 01 -> 1
    				  my_address += 0x40001000;
    			  }else if (new_data0.range(17,16).to_uint() == 0x2){ //go to port 10 -> 2
    				  my_address += 0x40002000;
    			  }else if (new_data0.range(17,16).to_uint() == 0x3){ //go to port 11 -> 3
    				  my_address += 0x40003000;
    			  }else{
    				  fw_hit = true;
#ifndef __SYNTHESIS__
    				  cout << name() << ".SW1.p0: While Working in Physical Address mode no valid input address "<< endl;
#endif
    			  }
#ifndef __SYNTHESIS__
    			  cout << name() << ".SW1.p0: Checking if address: 0x"<< hex << my_address.to_uint() << dec << " is between 0x"
    				   << hex << fw_read_l << dec << " and 0x" << hex << fw_read_h << dec << endl;
#endif
    		  if ((!fw_hit) && ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >= fw_read_l))){
    			  fw_hit = true;
    		  }
            }
    	  }
      }
      if (fw_hit){
    	  if(new_data0.range(21,20).to_uint() == 0x1){ //if read operation set response to 0x0
    		  init_responce_fifo_0.write(0x00000000);
    	  }
    	  rule_hit_counter_0++;
    	  fw_rule_hit_counter_0.write(rule_hit_counter_0);
#ifndef __SYNTHESIS__
    	  cout << name() << ".SW1.p0: Requested Packet " << hex << new_data0.to_uint() << dec  << " Matched FW Rule [" << rule_hit_counter_0 << "]" << endl;
#endif
    	  continue;
      }
      if ((responce_s0.read()[31].to_bool()) && (new_data0.range(21,20).to_uint() == 0x1)){
            count_droped_0 = count_droped_0 + 1;
            droped_0.write(count_droped_0);
#ifndef __SYNTHESIS__
            cout << name() << ".SW1.p0: Requested Write Op while pending result. Packet Dropped [" << count_droped_0 << "]: 0x" << hex << new_data0.to_uint() << dec << endl;
#endif
      }else{
            if (in_fifo_0.num_free() > 0){
                if (new_data0.range(21,20).to_uint() == 0x1){ //for read operations
                    new_data0.range(19,18) = 0x0; //set source port
                    init_responce_fifo_0.write(0x80000000); //set 31 bit to 1 while the rest to 0 this mean expecting result
                }
                in_fifo_0.write(new_data0);
#ifndef __SYNTHESIS__
                cout << name() << ".SW1.p0: Packet accepted in in FIFO 0 [" << in_fifo_0.num_available() << "]: 0x" << hex << new_data0.to_uint() << dec << endl;
#endif
            }else{
                count_droped_0 = count_droped_0 + 1;
                droped_0.write(count_droped_0);
#ifndef __SYNTHESIS__
                cout << name() << ".SW1.p0: Packet Dropped [" << count_droped_0 << "]: 0x" << hex << new_data0.to_uint() << dec << endl;
#endif
            }
      }
    }

  }
}

void four_port_sw::interface_in_1()
{
  unsigned int preval = 0x0;
  unsigned int prevalf = 0x0;
  unsigned int pre_read_l = 0x0;
  unsigned int pre_read_h = 0x0;
  unsigned int pre_read_r = 0x0;
  unsigned int fw_read_l = 0x0;
  unsigned int fw_read_h = 0x0;
  sc_uint<32> fw_read_r = 0x0;
  fw_hreg_p_1.write(0x0);
  fw_lreg_p_1.write(0x0);
  fw_rreg_p_1.write(0x0);

  count_total_1 = 0x0;
  rule_hit_counter_1 = 0x0;
  count_droped_1 = 0x0;
  total_1.write(0x0);
  droped_1.write(0x0);
  fw_rule_hit_counter_1.write(0x0);
  wait();
  while(true)
  {
    wait();
	sc_uint<32> new_fw = fw_rule_range_1.read();
	if (new_fw.to_uint() != pre_read_r){
	  pre_read_r = new_fw.to_uint();
	  fw_read_r = new_fw;
	  fw_read_l = fw_lower_range_1.read().to_uint();
	  fw_read_h = fw_upper_range_1.read().to_uint();
	  //this is debug for verify
	  fw_hreg_p_1.write(fw_read_h);
	  fw_lreg_p_1.write(fw_read_l);
	  fw_rreg_p_1.write(fw_read_r);
	  continue;
	}
	sc_uint<32> new_data = in_1.read();
	if (new_data.to_uint() != preval){
	  preval = new_data.to_uint();
	  count_total_1 = count_total_1 + 1;
	  total_1.write(count_total_1);
#ifndef __SYNTHESIS__
      cout << name() << ".SW1.p1: Received data " << count_total_1 << ": 0x" << hex << new_data.to_uint() << dec << endl;
#endif
      //check for firewall rule
	  //Rule register (bit active - stands for):
	  //7 -> 1: Enable Rule
	  //6 -> 1: Enabled NoC Mode // 0: Enable Compatibility Mode
	  //5 -> 1: Enable Rule for Write OpCode
	  //4 -> 1: Enable Rule for Read OpCode
	  //3 -> 1: Enable Rule for Destination Port 3
	  //2 -> 1: Enable Rule for Destination Port 2
	  //1 -> 1: Enable Rule for Destination Port 1
	  //0 -> 1: Enable Rule for Destination Port 0
	  bool fw_hit = false;
	  if (fw_read_r[7].to_bool()){
	    if ((fw_read_r[5].to_bool() && (new_data.range(21,20).to_uint() == 0x0)) || //firewall for writeOp
	        (fw_read_r[4].to_bool() && (new_data.range(21,20).to_uint() == 0x1))){ //firewall for ReadOp
	      sc_uint<32> my_address = new_data.range(31,22).to_uint();
	      if (fw_read_r[6].to_bool()){ //NoC Mod
	        if ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >=  fw_read_l)){ //the range of address is between
	    	  if ((fw_read_r[3].to_bool() && new_data.range(17,16).to_uint() == 0x3) ||
	    		  (fw_read_r[2].to_bool() && new_data.range(17,16).to_uint() == 0x2) ||
	      		  (fw_read_r[1].to_bool() && new_data.range(17,16).to_uint() == 0x1) ||
	      		  (fw_read_r[0].to_bool() && new_data.range(17,16).to_uint() == 0x0)){
	      	    fw_hit = true;
	    	  }
	    	}
	      }else{ //compatibility mode
            //physical address mode
	    	if (new_data.range(17,16).to_uint() == 0x0){ //go to port 00 -> 0
	    	  my_address += 0x40000000;
	    	}else if (new_data.range(17,16).to_uint() == 0x1){ //go to port 01 -> 1
	    	  my_address += 0x40001000;
	    	}else if (new_data.range(17,16).to_uint() == 0x2){ //go to port 10 -> 2
	    	  my_address += 0x40002000;
	    	}else if (new_data.range(17,16).to_uint() == 0x3){ //go to port 11 -> 3
	    	  my_address += 0x40003000;
	    	}else{
	    	  fw_hit = true;
#ifndef __SYNTHESIS__
              cout << name() << ".SW1.p1: While Working in Physical Address mode no valid input address "<< endl;
#endif
	    	}
#ifndef __SYNTHESIS__
              cout << name() << ".SW1.p1: Checking if address: 0x"<< hex << my_address.to_uint() << dec << " is between 0x"
	    		   << hex << fw_read_l << dec << " and 0x" << hex << fw_read_h << dec << endl;
#endif
	      if ((!fw_hit) && ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >= fw_read_l))){
	        fw_hit = true;
	      }
	    }
          }
	  }
      if (fw_hit){
	    if(new_data.range(21,20).to_uint() == 0x1){ //if read operation set response to 0x0
	      init_responce_fifo_1.write(0x00000000);
	    }
	    rule_hit_counter_1++;
	    fw_rule_hit_counter_1.write(rule_hit_counter_1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p1: Requested Packet " << hex << new_data.to_uint() << dec  << " Matched FW Rule [" << rule_hit_counter_1 << "]" << endl;
#endif
	    continue;
	 }
	 if ((responce_s1.read()[31].to_bool()) && (new_data.range(21,20).to_uint() == 0x1)){
	   count_droped_1 = count_droped_1 + 1;
	   droped_1.write(count_droped_1);
#ifndef __SYNTHESIS__
       cout << name() << ".SW1.p1: Requested Write Op while pending result. Packet Dropped [" << count_droped_1 << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
     }else{
	   if (in_fifo_1.num_free() > 0){
	     if (new_data.range(21,20).to_uint() == 0x1){ //for read operations
	       new_data.range(19,18) = 0x1; //set source port
	       init_responce_fifo_1.write(0x80000000); //set 31 bit to 1 while the rest to 0 this mean expecting result
	     }
	      in_fifo_1.write(new_data);
#ifndef __SYNTHESIS__
	      cout << name() << ".SW1.p1: Packet accepted in in FIFO 1 [" << in_fifo_1.num_available() << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
	   }else{
	     count_droped_1 = count_droped_1 + 1;
	     droped_1.write(count_droped_1);
#ifndef __SYNTHESIS__
	     cout << name() << ".SW1.p1: Packet Dropped [" << count_droped_1 << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
	    }
	  }
    }
  }
}

void four_port_sw::interface_in_2()
{
  unsigned int preval = 0x0;
  unsigned int prevalf = 0x0;
  unsigned int pre_read_l = 0x0;
  unsigned int pre_read_h = 0x0;
  unsigned int pre_read_r = 0x0;
  unsigned int fw_read_l = 0x0;
  unsigned int fw_read_h = 0x0;
  sc_uint<32> fw_read_r = 0x0;
  fw_hreg_p_2.write(0x0);
  fw_lreg_p_2.write(0x0);
  fw_rreg_p_2.write(0x0);

  count_total_2 = 0x0;
  rule_hit_counter_2 = 0x0;
  count_droped_2 = 0x0;
  total_2.write(0x0);
  droped_2.write(0x0);
  fw_rule_hit_counter_2.write(0x0);
  wait();
  while(true)
  {
    wait();
	sc_uint<32> new_fw = fw_rule_range_2.read();
	if (new_fw.to_uint() != pre_read_r){
	  pre_read_r = new_fw.to_uint();
	  fw_read_r = new_fw;
	  fw_read_l = fw_lower_range_2.read().to_uint();
	  fw_read_h = fw_upper_range_2.read().to_uint();
	  //this is debug for verify
	  fw_hreg_p_2.write(fw_read_h);
	  fw_lreg_p_2.write(fw_read_l);
	  fw_rreg_p_2.write(fw_read_r);
	  continue;
	}
	sc_uint<32> new_data = in_2.read();
	if (new_data.to_uint() != preval){
	  preval = new_data.to_uint();
	  count_total_2 = count_total_2 + 1;
	  total_2.write(count_total_2);
#ifndef __SYNTHESIS__
      cout << name() << ".SW2.p0: Received data " << count_total_2 << ": 0x" << hex << new_data.to_uint() << dec << endl;
#endif
      //check for firewall rule
	  //Rule register (bit active - stands for):
	  //7 -> 1: Enable Rule
	  //6 -> 1: Enabled NoC Mode // 0: Enable Compatibility Mode
	  //5 -> 1: Enable Rule for Write OpCode
	  //4 -> 1: Enable Rule for Read OpCode
	  //3 -> 1: Enable Rule for Destination Port 3
	  //2 -> 1: Enable Rule for Destination Port 2
	  //1 -> 1: Enable Rule for Destination Port 1
	  //0 -> 1: Enable Rule for Destination Port 0
	  bool fw_hit = false;
	  if (fw_read_r[7].to_bool()){
	    if ((fw_read_r[5].to_bool() && (new_data.range(21,20).to_uint() == 0x0)) || //firewall for writeOp
	        (fw_read_r[4].to_bool() && (new_data.range(21,20).to_uint() == 0x1))){ //firewall for ReadOp
	      sc_uint<32> my_address = new_data.range(31,22).to_uint();
	      if (fw_read_r[6].to_bool()){ //NoC Mod
	        if ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >=  fw_read_l)){ //the range of address is between
	    	  if ((fw_read_r[3].to_bool() && new_data.range(17,16).to_uint() == 0x3) ||
	    		  (fw_read_r[2].to_bool() && new_data.range(17,16).to_uint() == 0x2) ||
	      		  (fw_read_r[1].to_bool() && new_data.range(17,16).to_uint() == 0x1) ||
	      		  (fw_read_r[0].to_bool() && new_data.range(17,16).to_uint() == 0x0)){
	      	    fw_hit = true;
	    	  }
	    	}
	      }else{ //compatibility mode
            //physical address mode
	    	if (new_data.range(17,16).to_uint() == 0x0){ //go to port 00 -> 0
	    	  my_address += 0x40000000;
	    	}else if (new_data.range(17,16).to_uint() == 0x1){ //go to port 01 -> 1
	    	  my_address += 0x40001000;
	    	}else if (new_data.range(17,16).to_uint() == 0x2){ //go to port 10 -> 2
	    	  my_address += 0x40002000;
	    	}else if (new_data.range(17,16).to_uint() == 0x3){ //go to port 11 -> 3
	    	  my_address += 0x40003000;
	    	}else{
	    	  fw_hit = true;
#ifndef __SYNTHESIS__
              cout << name() << ".SW2.p0: While Working in Physical Address mode no valid input address "<< endl;
#endif
	    	}
#ifndef __SYNTHESIS__
              cout << name() << ".SW2.p0: Checking if address: 0x"<< hex << my_address.to_uint() << dec << " is between 0x"
	    		   << hex << fw_read_l << dec << " and 0x" << hex << fw_read_h << dec << endl;
#endif
	      if ((!fw_hit) && ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >= fw_read_l))){
	        fw_hit = true;
	      }
	    }
           }
	  }
      if (fw_hit){
	    if(new_data.range(21,20).to_uint() == 0x1){ //if read operation set response to 0x0
	      init_responce_fifo_2.write(0x00000000);
	    }
	    rule_hit_counter_2++;
	    fw_rule_hit_counter_2.write(rule_hit_counter_2);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p0: Requested Packet " << hex << new_data.to_uint() << dec  << " Matched FW Rule [" << rule_hit_counter_2 << "]" << endl;
#endif
	    continue;
	 }
	 if ((responce_s2.read()[31].to_bool()) && (new_data.range(21,20).to_uint() == 0x1)){
	   count_droped_2 = count_droped_2 + 1;
	   droped_2.write(count_droped_2);
#ifndef __SYNTHESIS__
       cout << name() << ".SW2.p0: Requested Write Op while pending result. Packet Dropped [" << count_droped_2 << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
     }else{
	   if (in_fifo_2.num_free() > 0){
	     if (new_data.range(21,20).to_uint() == 0x1){ //for read operations
	       new_data.range(19,18) = 0x2; //set source port
	       init_responce_fifo_2.write(0x80000000); //set 31 bit to 1 while the rest to 0 this mean expecting result
	     }
	      in_fifo_2.write(new_data);
#ifndef __SYNTHESIS__
	      cout << name() << ".SW2.p0: Packet accepted in in FIFO 1 [" << in_fifo_2.num_available() << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
	   }else{
	     count_droped_2 = count_droped_2 + 1;
	     droped_2.write(count_droped_2);
#ifndef __SYNTHESIS__
	     cout << name() << ".SW2.p0: Packet Dropped [" << count_droped_2 << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
	    }
	  }
    }
  }
}

void four_port_sw::interface_in_3()
{
  unsigned int preval = 0x0;
  unsigned int prevalf = 0x0;
  unsigned int pre_read_l = 0x0;
  unsigned int pre_read_h = 0x0;
  unsigned int pre_read_r = 0x0;
  unsigned int fw_read_l = 0x0;
  unsigned int fw_read_h = 0x0;
  sc_uint<32> fw_read_r = 0x0;
  fw_hreg_p_3.write(0x0);
  fw_lreg_p_3.write(0x0);
  fw_rreg_p_3.write(0x0);

  count_total_3 = 0x0;
  rule_hit_counter_3 = 0x0;
  count_droped_3 = 0x0;
  total_3.write(0x0);
  droped_3.write(0x0);
  fw_rule_hit_counter_3.write(0x0);
  wait();
  while(true)
  {
    wait();
	sc_uint<32> new_fw = fw_rule_range_3.read();
	if (new_fw.to_uint() != pre_read_r){
	  pre_read_r = new_fw.to_uint();
	  fw_read_r = new_fw;
	  fw_read_l = fw_lower_range_3.read().to_uint();
	  fw_read_h = fw_upper_range_3.read().to_uint();
	  //this is debug for verify
	  fw_hreg_p_3.write(fw_read_h);
	  fw_lreg_p_3.write(fw_read_l);
	  fw_rreg_p_3.write(fw_read_r);
	  continue;
	}
	sc_uint<32> new_data = in_3.read();
	if (new_data.to_uint() != preval){
	  preval = new_data.to_uint();
	  count_total_3 = count_total_3 + 1;
	  total_3.write(count_total_3);
#ifndef __SYNTHESIS__
      cout << name() << ".SW2.p1: Received data " << count_total_3 << ": 0x" << hex << new_data.to_uint() << dec << endl;
#endif
      //check for firewall rule
	  //Rule register (bit active - stands for):
	  //7 -> 1: Enable Rule
	  //6 -> 1: Enabled NoC Mode // 0: Enable Compatibility Mode
	  //5 -> 1: Enable Rule for Write OpCode
	  //4 -> 1: Enable Rule for Read OpCode
	  //3 -> 1: Enable Rule for Destination Port 3
	  //2 -> 1: Enable Rule for Destination Port 2
	  //1 -> 1: Enable Rule for Destination Port 1
	  //0 -> 1: Enable Rule for Destination Port 0
	  bool fw_hit = false;
	  if (fw_read_r[7].to_bool()){
	    if ((fw_read_r[5].to_bool() && (new_data.range(21,20).to_uint() == 0x0)) || //firewall for writeOp
	        (fw_read_r[4].to_bool() && (new_data.range(21,20).to_uint() == 0x1))){ //firewall for ReadOp
	      sc_uint<32> my_address = new_data.range(31,22).to_uint();
	      if (fw_read_r[6].to_bool()){ //NoC Mod
	        if ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >=  fw_read_l)){ //the range of address is between
	    	  if ((fw_read_r[3].to_bool() && new_data.range(17,16).to_uint() == 0x3) ||
	    		  (fw_read_r[2].to_bool() && new_data.range(17,16).to_uint() == 0x2) ||
	      		  (fw_read_r[1].to_bool() && new_data.range(17,16).to_uint() == 0x1) ||
	      		  (fw_read_r[0].to_bool() && new_data.range(17,16).to_uint() == 0x0)){
	      	    fw_hit = true;
	    	  }
	    	}
	      }else{ //compatibility mode
            //physical address mode
	    	if (new_data.range(17,16).to_uint() == 0x0){ //go to port 00 -> 0
	    	  my_address += 0x40000000;
	    	}else if (new_data.range(17,16).to_uint() == 0x1){ //go to port 01 -> 1
	    	  my_address += 0x40001000;
	    	}else if (new_data.range(17,16).to_uint() == 0x2){ //go to port 10 -> 2
	    	  my_address += 0x40002000;
	    	}else if (new_data.range(17,16).to_uint() == 0x3){ //go to port 11 -> 3
	    	  my_address += 0x40003000;
	    	}else{
	    	  fw_hit = true;
#ifndef __SYNTHESIS__
              cout << name() << ".SW2.p1: While Working in Physical Address mode no valid input address "<< endl;
#endif
	    	}
#ifndef __SYNTHESIS__
              cout << name() << ".SW2.p1: Checking if address: 0x"<< hex << my_address.to_uint() << dec << " is between 0x"
	    		   << hex << fw_read_l << dec << " and 0x" << hex << fw_read_h << dec << endl;
#endif
	      if ((!fw_hit) && ((fw_read_h >= my_address.to_uint()) && (my_address.to_uint() >= fw_read_l))){
	        fw_hit = true;
	      }
	    }
          }
        }
      if (fw_hit){
	    if(new_data.range(21,20).to_uint() == 0x1){ //if read operation set response to 0x0
	      init_responce_fifo_3.write(0x00000000);
	    }
	    rule_hit_counter_3++;
	    fw_rule_hit_counter_3.write(rule_hit_counter_3);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p1: Requested Packet " << hex << new_data.to_uint() << dec  << " Matched FW Rule [" << rule_hit_counter_3 << "]" << endl;
#endif
	    continue;
	 }
	 if ((responce_s3.read()[31].to_bool()) && (new_data.range(21,20).to_uint() == 0x1)){
	   count_droped_3 = count_droped_3 + 1;
	   droped_3.write(count_droped_3);
#ifndef __SYNTHESIS__
       cout << name() << ".SW2.p1: Requested Write Op while pending result. Packet Dropped [" << count_droped_3 << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
     }else{
	   if (in_fifo_3.num_free() > 0){
	     if (new_data.range(21,20).to_uint() == 0x1){ //for read operations
	       new_data.range(19,18) = 0x3; //set source port
	       init_responce_fifo_3.write(0x80000000); //set 31 bit to 1 while the rest to 0 this mean expecting result
	     }
	      in_fifo_3.write(new_data);
#ifndef __SYNTHESIS__
	      cout << name() << ".SW2.p1: Packet accepted in in FIFO 1 [" << in_fifo_3.num_available() << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
	   }else{
	     count_droped_3 = count_droped_3 + 1;
	     droped_3.write(count_droped_3);
#ifndef __SYNTHESIS__
	     cout << name() << ".SW2.p1: Packet Dropped [" << count_droped_3 << "]: 0x" << hex << new_data.to_uint() << dec << endl;
#endif
	    }
	  }
    }
  }
}


void four_port_sw::interface_out_0()
{
#ifndef __SYNTHESIS__
  preready[0] = false;
#endif
  wait();
  while(true)
  {
    wait();
    sc_uint<32> out_data0;
#ifndef __SYNTHESIS__
    if (!done_0.read() && !preready[0]){
#endif
      if (out_fifo_0.nb_read(out_data0)){
              unsigned int new_address = 0x40000000;
              new_address += out_data0.range(31,22).to_uint();
              sc_uint<32> new_data = 0x0;
              if (out_data0.range(21,20).to_uint() == 0x1){
                new_data = out_0->read(new_address);
                new_data.range(31,22) = out_data0.range(31,22).to_uint();
                new_data.range(21,20) = 0x2;
                new_data.range(19,18) = out_data0.range(17,16);
                new_data.range(17,16) = out_data0.range(19,18);
                responce_fifo_0.write(new_data);
              }else{
                  new_data.range(15,0) = out_data0.range(15,0);
                out_0->write(new_address, &new_data);
#ifndef __SYNTHESIS__
                cout << name() << "PORT 0 PCKT OUT: 0x" << hex <<  out_data0 << dec << endl;
#endif
#ifndef __SYNTHESIS__
                out_addr_to_read_0.write(new_address);
                ready_0.write(true);
                preready[0] = true;
#endif
              }
#ifndef __SYNTHESIS__
      }
    }else if (done_0.read() && preready[0]){
              ready_0.write(false);
              preready[0] = false;
#endif
    }

  }
}

void four_port_sw::interface_out_1()
{
#ifndef __SYNTHESIS__
  preready[1] = false;
#endif
  wait();
  while(true)
  {
    wait();
    sc_uint<32> out_data1;
#ifndef __SYNTHESIS__
    if (!done_1.read() && !preready[1]){
#endif
      if (out_fifo_1.nb_read(out_data1)){
              unsigned int new_address = 0x40001000;
              new_address += out_data1.range(31,22).to_uint();
              sc_uint<32> new_data = 0x0;
              if (out_data1.range(21,20).to_uint() == 0x1){
                new_data = out_1->read(new_address);
                new_data.range(31,22) = out_data1.range(31,22).to_uint();
                new_data.range(21,20) = 0x2;
                new_data.range(19,18) = out_data1.range(17,16);
                new_data.range(17,16) = out_data1.range(19,18);
                responce_fifo_1.write(new_data);
              }else{
                new_data.range(15,0) = out_data1.range(15,0);
                out_1->write(new_address, &new_data);
#ifndef __SYNTHESIS__
                cout << name() << "PORT 1 PCKT OUT: 0x" << hex <<  out_data1 << dec << endl;
#endif
#ifndef __SYNTHESIS__
                out_addr_to_read_1.write(new_address);
                ready_1.write(true);
                preready[1] = true;
#endif
              }
#ifndef __SYNTHESIS__
      }
    }else if (done_1.read() && preready[1]){
              ready_1.write(false);
              preready[1] = false;
#endif
    }
  }
}

void four_port_sw::interface_out_2()
{
#ifndef __SYNTHESIS__
  preready[2] = false;
#endif
  wait();
  while(true)
  {
    wait();
    sc_uint<32> out_data2;
#ifndef __SYNTHESIS__
    if (!done_2.read() && !preready[2]){
#endif
      if (out_fifo_2.nb_read(out_data2)){
              unsigned int new_address = 0x40002000;
              new_address += out_data2.range(31,22).to_uint();
              sc_uint<32> new_data = 0x0;
              if (out_data2.range(21,20).to_uint() == 0x1){
                new_data = out_2->read(new_address);
                new_data.range(31,22) = out_data2.range(31,22).to_uint();
                new_data.range(21,20) = 0x2;
                new_data.range(19,18) = out_data2.range(17,16);
                new_data.range(17,16) = out_data2.range(19,18);
                responce_fifo_2.write(new_data);
              }else{
                new_data.range(15,0) = out_data2.range(15,0);
                out_2->write(new_address, &new_data);
#ifndef __SYNTHESIS__
                cout << name() << "PORT 2 PCKT OUT: 0x" << hex <<  out_data2 << dec << endl;
#endif
#ifndef __SYNTHESIS__
                out_addr_to_read_2.write(new_address);
                ready_2.write(true);
                preready[2] = true;
#endif
              }
#ifndef __SYNTHESIS__
      }
    }else if (done_2.read() && preready[2]){
              ready_2.write(false);
              preready[2] = false;
#endif
    }
  }
}

void four_port_sw::interface_out_3()
{
#ifndef __SYNTHESIS__
  preready[3] = false;
#endif
  wait();
  while(true)
  {
    wait();
    sc_uint<32> out_data3;
#ifndef __SYNTHESIS__
    if (!done_3.read() && !preready[3]){
#endif
      if (out_fifo_3.nb_read(out_data3)){
              unsigned int new_address = 0x40003000;
              new_address += out_data3.range(31,22).to_uint();
              sc_uint<32> new_data = 0x0;
              if (out_data3.range(21,20).to_uint() == 0x1){
                new_data = out_3->read(new_address);
                new_data.range(31,22) = out_data3.range(31,22).to_uint();
                new_data.range(21,20) = 0x2;
                new_data.range(19,18) = out_data3.range(17,16);
                new_data.range(17,16) = out_data3.range(19,18);
                responce_fifo_3.write(new_data);
              }else{
                new_data.range(15,0) = out_data3.range(15,0);
                out_3->write(new_address, &new_data);
#ifndef __SYNTHESIS__
                cout << name() << "PORT 3 PCKT OUT: 0x" << hex <<  out_data3 << dec << endl;
#endif
#ifndef __SYNTHESIS__
                out_addr_to_read_3.write(new_address);
                ready_3.write(true);
                preready[3] = true;
#endif
              }
#ifndef __SYNTHESIS__
      }
    }else if (done_3.read() && preready[3]){
              ready_3.write(false);
              preready[3] = false;
#endif
    }
  }
}

void four_port_sw::entry_sw_1(){
  wait();
  while(true)
  {
    wait();
    sc_uint<32> new_data0;
    if (in_fifo_0.nb_read(new_data0)){
      pkt new_packet;
      new_packet.addr_offset = new_data0.range(31,22).to_uint();
      new_packet.opcode = new_data0.range(21,20).to_uint();
      new_packet.src = new_data0.range(19,18).to_uint();
      new_packet.dest = new_data0.range(17,16).to_uint();
      new_packet.data = new_data0.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data0[17] = new_data0[16];
      new_data0[16] = next_position;
      if (!next_position){
        out_fifo_s1_0.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_s1_1.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data1;
    if (in_fifo_1.nb_read(new_data1)){
      pkt new_packet;
      new_packet.addr_offset = new_data1.range(31,22).to_uint();
      new_packet.opcode = new_data1.range(21,20).to_uint();
      new_packet.src = new_data1.range(19,18).to_uint();
      new_packet.dest = new_data1.range(17,16).to_uint();
      new_packet.data = new_data1.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data1[17] = new_data1[16];
      new_data1[16] = next_position;
      if (!next_position){
        out_fifo_s1_0.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_s1_1.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
    }
  }
}

void four_port_sw::entry_sw_2(){
  wait();
  while(true)
  {
    wait();
    sc_uint<32> new_data0;
    if (in_fifo_2.nb_read(new_data0)){
      pkt new_packet;
      new_packet.addr_offset = new_data0.range(31,22).to_uint();
      new_packet.opcode = new_data0.range(21,20).to_uint();
      new_packet.src = new_data0.range(19,18).to_uint();
      new_packet.dest = new_data0.range(17,16).to_uint();
      new_packet.data = new_data0.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data0[17] = new_data0[16];
      new_data0[16] = next_position;
      if (!next_position){
        out_fifo_s2_0.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_s2_1.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data1;
    if (in_fifo_3.nb_read(new_data1)){
      pkt new_packet;
      new_packet.addr_offset = new_data1.range(31,22).to_uint();
      new_packet.opcode = new_data1.range(21,20).to_uint();
      new_packet.src = new_data1.range(19,18).to_uint();
      new_packet.dest = new_data1.range(17,16).to_uint();
      new_packet.data = new_data1.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data1[17] = new_data1[16];
      new_data1[16] = next_position;
      if (!next_position){
        out_fifo_s2_0.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_s2_1.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
    }
  }
}

void four_port_sw::entry_sw_3(){
  wait();
  while(true)
  {
    wait();
    sc_uint<32> new_data0;
    if (out_fifo_s1_0.nb_read(new_data0)){
      pkt new_packet;
      new_packet.addr_offset = new_data0.range(31,22).to_uint();
      new_packet.opcode = new_data0.range(21,20).to_uint();
      new_packet.src = new_data0.range(19,18).to_uint();
      new_packet.dest = new_data0.range(17,16).to_uint();
      new_packet.data = new_data0.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data0[17] = new_data0[16];
      new_data0[16] = next_position;
      if (!next_position){
        out_fifo_0.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW3: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_1.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW3: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data1;
    if (out_fifo_s2_0.nb_read(new_data1)){
      pkt new_packet;
      new_packet.addr_offset = new_data1.range(31,22).to_uint();
      new_packet.opcode = new_data1.range(21,20).to_uint();
      new_packet.src = new_data1.range(19,18).to_uint();
      new_packet.dest = new_data1.range(17,16).to_uint();
      new_packet.data = new_data1.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data1[17] = new_data1[16];
      new_data1[16] = next_position;
      if (!next_position){
        out_fifo_0.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW3: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_1.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW3: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
    }
  }
}

void four_port_sw::entry_sw_4(){
  wait();
  while(true)
  {
    wait();
    sc_uint<32> new_data0;
    if (out_fifo_s1_1.nb_read(new_data0)){
      pkt new_packet;
      new_packet.addr_offset = new_data0.range(31,22).to_uint();
      new_packet.opcode = new_data0.range(21,20).to_uint();
      new_packet.src = new_data0.range(19,18).to_uint();
      new_packet.dest = new_data0.range(17,16).to_uint();
      new_packet.data = new_data0.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data0[17] = new_data0[16];
      new_data0[16] = next_position;
      if (!next_position){
        out_fifo_2.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW4: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_3.nb_write(new_data0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW4: Packet " << "[0x" << hex << new_data0.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data1;
    if (out_fifo_s2_1.nb_read(new_data1)){
      pkt new_packet;
      new_packet.addr_offset = new_data1.range(31,22).to_uint();
      new_packet.opcode = new_data1.range(21,20).to_uint();
      new_packet.src = new_data1.range(19,18).to_uint();
      new_packet.dest = new_data1.range(17,16).to_uint();
      new_packet.data = new_data1.range(15,0).to_uint();
      //find next exit port and rotate
      bool next_position = new_packet.dest[1].to_bool();
      new_data1[17] = new_data1[16];
      new_data1[16] = next_position;
      if (!next_position){
        out_fifo_2.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW4: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 0 [" << out_fifo_0.num_available() << "]" << endl;
#endif
      }else{
        out_fifo_3.nb_write(new_data1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW4: Packet " << "[0x" << hex << new_data1.to_uint() << dec << "] Moved from In fifo 0 to out fifo 1 [" << out_fifo_1.num_available() << "]" << endl;
#endif
      }
    }
  }
}

void four_port_sw::entry_response(){
  responce0 = 0x0;
  responce1 = 0x0;
  responce2 = 0x0;
  responce3 = 0x0;
  responce_s0.write(0x0);
  responce_s1.write(0x0);
  responce_s2.write(0x0);
  responce_s3.write(0x0);
  wait();
  while(true)
  {
    wait();
    sc_uint<32> new_data0;
    if (init_responce_fifo_0.nb_read(new_data0)){
      responce0 = new_data0.to_uint();
      responce_s0.write(responce0);
      in_responce_0.write(responce0);
#ifndef __SYNTHESIS__
      cout << name() << ".SW1.p0: Init Response Packet Was written" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce0.to_uint() << dec << endl;
#endif
      wait();
    }else if (responce_fifo_0.nb_read(new_data0)){
      responce0 = new_data0.to_uint();
      responce0[31] = false;
      if (responce0.range(17,16) == 0x0){
        responce_s0.write(responce0);
        in_responce_0.write(responce0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce0.to_uint() << dec << endl;
#endif
      }else if (responce0.range(17,16) == 0x1){
        responce_s1.write(responce0);
        in_responce_1.write(responce0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce0.to_uint() << dec << endl;
#endif
      }else if (responce0.range(17,16) == 0x2){
        responce_s2.write(responce0);
        in_responce_2.write(responce0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce0.to_uint() << dec << endl;
#endif
      }else if (responce0.range(17,16) == 0x3){
        responce_s3.write(responce0);
        in_responce_3.write(responce0);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce0.to_uint() << dec << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data1;
    if (init_responce_fifo_1.nb_read(new_data1)){
      responce1 = new_data1.to_uint();
      responce_s1.write(responce1);
      in_responce_1.write(responce1);
#ifndef __SYNTHESIS__
      cout << name() << ".SW1.p1: Init Response Packet Was written" << "[0x" << hex << new_data1.to_uint() << dec << "]: 0x" << hex << responce1.to_uint() << dec << endl;
#endif
      wait();
    }else if (responce_fifo_1.nb_read(new_data1)){
      responce1 = new_data1.to_uint();
      responce1[31] = false;
      if (responce1.range(17,16) == 0x0){
        responce_s0.write(responce1);
        in_responce_0.write(responce1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce1.to_uint() << dec << endl;
#endif
      }else if (responce1.range(17,16) == 0x1){
        responce_s1.write(responce1);
        in_responce_1.write(responce1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce1.to_uint() << dec << endl;
#endif
      }else if (responce1.range(17,16) == 0x2){
        responce_s2.write(responce1);
        in_responce_2.write(responce1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce1.to_uint() << dec << endl;
#endif
      }else if (responce1.range(17,16) == 0x3){
        responce_s3.write(responce1);
        in_responce_3.write(responce1);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce1.to_uint() << dec << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data2;
    if (init_responce_fifo_2.nb_read(new_data2)){
      responce2 = new_data2.to_uint();
      responce_s2.write(responce2);
      in_responce_2.write(responce2);
#ifndef __SYNTHESIS__
      cout << name() << ".SW2.p0: Init Response Packet Was written" << "[0x" << hex << new_data2.to_uint() << dec << "]: 0x" << hex << responce2.to_uint() << dec << endl;
#endif
      wait();
    }else if (responce_fifo_2.nb_read(new_data2)){
      responce2 = new_data2.to_uint();
      responce2[31] = false;
      if (responce2.range(17,16) == 0x0){
        responce_s0.write(responce2);
        in_responce_0.write(responce2);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce2.to_uint() << dec << endl;
#endif
      }else if (responce2.range(17,16) == 0x1){
        responce_s1.write(responce2);
        in_responce_1.write(responce2);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce2.to_uint() << dec << endl;
#endif
      }else if (responce2.range(17,16) == 0x2){
        responce_s2.write(responce2);
        in_responce_2.write(responce2);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce2.to_uint() << dec << endl;
#endif
      }else if (responce2.range(17,16) == 0x3){
        responce_s3.write(responce2);
        in_responce_3.write(responce2);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce2.to_uint() << dec << endl;
#endif
      }
      wait();
    }
    sc_uint<32> new_data3;
    if (init_responce_fifo_3.nb_read(new_data3)){
      responce3 = new_data3.to_uint();
      responce_s3.write(responce3);
      in_responce_3.write(responce3);
#ifndef __SYNTHESIS__
      cout << name() << ".SW2.p1: Init Response Packet Was written" << "[0x" << hex << new_data3.to_uint() << dec << "]: 0x" << hex << responce3.to_uint() << dec << endl;
#endif
      wait();
    }else if (responce_fifo_3.nb_read(new_data3)){
      responce3 = new_data3.to_uint() & 0x7FFFFFFF;
      responce3[31] = false;
      if (responce3.range(17,16) == 0x0){
        responce_s0.write(responce3);
        in_responce_0.write(responce3);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce3.to_uint() << dec << endl;
#endif
      }else if (responce3.range(17,16) == 0x1){
        responce_s1.write(responce3);
        in_responce_1.write(responce3);
#ifndef __SYNTHESIS__
        cout << name() << ".SW1.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce3.to_uint() << dec << endl;
#endif
      }else if (responce3.range(17,16) == 0x2){
        responce_s2.write(responce3);
        in_responce_2.write(responce3);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p0: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce3.to_uint() << dec << endl;
#endif
      }else if (responce3.range(17,16) == 0x3){
        responce_s3.write(responce3);
        in_responce_3.write(responce3);
#ifndef __SYNTHESIS__
        cout << name() << ".SW2.p1: Response Packet Was prepared" << "[0x" << hex << new_data0.to_uint() << dec << "]: 0x" << hex << responce3.to_uint() << dec << endl;
#endif
      }
      wait();
    }
  }
}

