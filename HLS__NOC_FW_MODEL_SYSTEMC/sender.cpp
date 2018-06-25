#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;

#include "sender.h"

void sender:: entry()
{
	pkt pkt_data;
	sc_uint<4> dest;
	int count = 0;
	int pkt_count = 0;
	int readcount = 0;
	wait(10);
	srand((100+10*source_id.read().to_uint()));
	while(true){
		wait(10);
		sc_uint<32> new_data;
		if (count < 2){
			/////generate an 16-bit random value for data//////////
			pkt_data.data = rand()%65535;
			/////generate an 12-bit random value for address offset//////////
			pkt_data.addr_offset = pkt_count*10;
			/////send it to 1 or more(<=4) destinations//////////////
			pkt_data.dest = source_id.read().to_uint();
			pkt_data.src = source_id.read().to_uint();
			pkt_data.opcode = 0x0; //write operation
			new_data.range(31,22) = pkt_data.addr_offset;
			new_data.range(21,20) = pkt_data.opcode.to_uint();
			new_data.range(19,18) = pkt_data.src.to_uint();
			new_data.range(17,16) = pkt_data.dest.to_uint();
			new_data.range(15,0) = pkt_data.data;
			pkt_out.write(new_data);
			cout << ".........................." << endl;
			cout << "New Packet Sent" << endl;
			cout << "OPCODE: " << pkt_data.opcode.to_string() << " ";
			cout << "Source Addresses: " << pkt_data.src.to_string() << " ";
			cout << "Destination Addresses: " << pkt_data.dest.to_string() << " ";
			cout << "Packet Address Offset: " << pkt_data.addr_offset.to_string(SC_HEX) << endl;
			cout << "Packet Value: " << pkt_data.data.to_string(SC_HEX) << endl;
			cout << "Sender ID: " << (unsigned int)source_id.read() << endl;
			cout << "UINT Packet: 0x" << hex << new_data << dec << endl;
			cout << ".........................." << endl;
		}else if ((count >= 2) && (count < 4)){
			/////generate an 16-bit random value for data//////////
			pkt_data.data = 0x0;
			/////generate an 12-bit random value for address offset//////////
			pkt_data.addr_offset = readcount*10;
			/////send it to 1 or more(<=4) destinations//////////////
			pkt_data.dest = source_id.read().to_uint();
			pkt_data.src = source_id.read().to_uint();
			pkt_data.opcode = 0x1; //write operation
			new_data.range(31,22) = pkt_data.addr_offset;
			new_data.range(21,20) = pkt_data.opcode.to_uint();
			new_data.range(19,18) = pkt_data.src.to_uint();
			new_data.range(17,16) = pkt_data.dest.to_uint();
			new_data.range(15,0) = pkt_data.data;
			pkt_out.write(new_data);
			cout << ".........................." << endl;
			cout << "New Packet Sent" << endl;
			cout << "OPCODE: " << pkt_data.opcode.to_string() << " ";
			cout << "Source Addresses: " << pkt_data.src.to_string() << " ";
			cout << "Destination Addresses: " << pkt_data.dest.to_string() << " ";
			cout << "Packet Address Offset: " << pkt_data.addr_offset.to_string(SC_HEX) << endl;
			cout << "Packet Value: " << pkt_data.data.to_string(SC_HEX) << endl;
			cout << "Sender ID: " << (unsigned int)source_id.read() << endl;
			cout << "UINT Packet: 0x" << hex << new_data << dec << endl;
			cout << ".........................." << endl;
			readcount++;
			wait(1000);
			sc_uint<32> res_data = pkt_in.read();
			while(res_data[31].to_bool()){
				wait(5);
				res_data = pkt_in.read();
			}
			cout << ".........................." << endl;
			cout << "New Packet Respond" << endl;
			cout << "UINT Respond: 0x" << hex << res_data.range(30,0).to_uint() << dec << endl;
			cout << "Reciever ID: " << (unsigned int)source_id.read() << endl;
			cout << ".........................." << endl;
		}else if (count == 4){
			//setup a comp firewall rule for write ops
			if (source_id.read().to_uint() == 0x3){
				cout << ".........................." << endl;
				//0xA0 only write
				//0xB0 write && read
				cout << "Setup ("<< source_id <<") Comp Firewall rule u: 0x40003FFF, l: 0x40003000, rule: 0xB0" << endl;
				setup_lower.write(0x40000000);
				setup_upper.write(0x40000FFF);
				setup_rule.write(0xB0);
			}
			pkt_count = 0;
			readcount = 0;
		}else if (count == 5){
			/////generate an 16-bit random value for data//////////
			pkt_data.data = rand()%65535;
			/////generate an 12-bit random value for address offset//////////
			pkt_data.addr_offset = pkt_count*10;
			/////send it to 1 or more(<=4) destinations//////////////
			pkt_data.dest = 3-source_id.read().to_uint();
			pkt_data.src = source_id.read().to_uint();
			pkt_data.opcode = 0x0; //write operation
			new_data.range(31,22) = pkt_data.addr_offset;
			new_data.range(21,20) = pkt_data.opcode.to_uint();
			new_data.range(19,18) = pkt_data.src.to_uint();
			new_data.range(17,16) = pkt_data.dest.to_uint();
			new_data.range(15,0) = pkt_data.data;
			pkt_out.write(new_data);
			cout << ".........................." << endl;
			cout << "New Packet Sent" << endl;
			cout << "OPCODE: " << pkt_data.opcode.to_string() << " ";
			cout << "Source Addresses: " << pkt_data.src.to_string() << " ";
			cout << "Destination Addresses: " << pkt_data.dest.to_string() << " ";
			cout << "Packet Address Offset: " << pkt_data.addr_offset.to_string(SC_HEX) << endl;
			cout << "Packet Value: " << pkt_data.data.to_string(SC_HEX) << endl;
			cout << "Sender ID: " << (unsigned int)source_id.read() << endl;
			cout << "UINT Packet: 0x" << hex << new_data << dec << endl;
			cout << ".........................." << endl;
		}else if (count == 6) {
			/////generate an 16-bit random value for data//////////
			pkt_data.data = 0x0;
			/////generate an 12-bit random value for address offset//////////
			pkt_data.addr_offset = readcount*10;
			/////send it to 1 or more(<=4) destinations//////////////
			pkt_data.dest = 3-source_id.read().to_uint();
			pkt_data.src = source_id.read().to_uint();
			pkt_data.opcode = 0x1; //write operation
			new_data.range(31,22) = pkt_data.addr_offset;
			new_data.range(21,20) = pkt_data.opcode.to_uint();
			new_data.range(19,18) = pkt_data.src.to_uint();
			new_data.range(17,16) = pkt_data.dest.to_uint();
			new_data.range(15,0) = pkt_data.data;
			pkt_out.write(new_data);
			cout << ".........................." << endl;
			cout << "New Packet Sent" << endl;
			cout << "OPCODE: " << pkt_data.opcode.to_string() << " ";
			cout << "Source Addresses: " << pkt_data.src.to_string() << " ";
			cout << "Destination Addresses: " << pkt_data.dest.to_string() << " ";
			cout << "Packet Address Offset: " << pkt_data.addr_offset.to_string(SC_HEX) << endl;
			cout << "Packet Value: " << pkt_data.data.to_string(SC_HEX) << endl;
			cout << "Sender ID: " << (unsigned int)source_id.read() << endl;
			cout << "UINT Packet: 0x" << hex << new_data << dec << endl;
			cout << ".........................." << endl;
			readcount++;
			wait(10);
			sc_uint<32> res_data = pkt_in.read();
			cout << ".........."<< res_data <<".........." << endl;
			while(res_data[31].to_bool()){
				wait(5);
				res_data = pkt_in.read();
				cout << ".........."<< res_data <<".........." << endl;
			}
			cout << ".........................." << endl;
			cout << "New Packet Respond" << endl;
			cout << "UINT Respond: 0x" << hex << res_data.range(30,0).to_uint() << dec << endl;
			cout << "Reciever ID: " << (unsigned int)source_id.read() << endl;
			cout << ".........................." << endl;
		}else if (count == 7){
			//clear  a firewall rule
			if (source_id.read().to_uint() == 0x3){
				cout << ".........................." << endl;
				cout << "Setup ("<< source_id <<") Firewall reset rule u: 0x0, l: 0x0, rule: 0x0" << endl;
				setup_lower.write(0x0);
				setup_upper.write(0x0);
				setup_rule.write(0x0);
			}
			pkt_count = 0;
			readcount = 0;
		}else if (count == 8){
			/////generate an 16-bit random value for data//////////
			pkt_data.data = rand()%65535;
			/////generate an 12-bit random value for address offset//////////
			pkt_data.addr_offset = pkt_count*10;
			/////send it to 1 or more(<=4) destinations//////////////
			pkt_data.dest = 3-source_id.read().to_uint();
			pkt_data.src = source_id.read().to_uint();
			pkt_data.opcode = 0x0; //write operation
			new_data.range(31,22) = pkt_data.addr_offset;
			new_data.range(21,20) = pkt_data.opcode.to_uint();
			new_data.range(19,18) = pkt_data.src.to_uint();
			new_data.range(17,16) = pkt_data.dest.to_uint();
			new_data.range(15,0) = pkt_data.data;
			pkt_out.write(new_data);
			cout << ".........................." << endl;
			cout << "New Packet Sent" << endl;
			cout << "OPCODE: " << pkt_data.opcode.to_string() << " ";
			cout << "Source Addresses: " << pkt_data.src.to_string() << " ";
			cout << "Destination Addresses: " << pkt_data.dest.to_string() << " ";
			cout << "Packet Address Offset: " << pkt_data.addr_offset.to_string(SC_HEX) << endl;
			cout << "Packet Value: " << pkt_data.data.to_string(SC_HEX) << endl;
			cout << "Sender ID: " << (unsigned int)source_id.read() << endl;
			cout << "UINT Packet: 0x" << hex << new_data << dec << endl;
			cout << ".........................." << endl;
		}else if (count == 9){
			//setup a firewall rule
			if (source_id.read().to_uint() == 0x3){
				cout << ".........................." << endl;
				cout << "Setup ("<< source_id <<") NoC Firewall rule u: 0x0, l: 0xFFF, rule: 0xF8" << endl;
				setup_lower.write(0x0);
				setup_upper.write(0xFFF);
				setup_rule.write(0xFF);
			}
			pkt_count = 0;
			readcount = 0;
		}else if (count == 10){
			/////generate an 16-bit random value for data//////////
			pkt_data.data = rand()%65535;
			/////generate an 12-bit random value for address offset//////////
			pkt_data.addr_offset = pkt_count*10;
			/////send it to 1 or more(<=4) destinations//////////////
			pkt_data.dest = 3-source_id.read().to_uint();
			pkt_data.src = source_id.read().to_uint();
			pkt_data.opcode = 0x0; //write operation
			new_data.range(31,22) = pkt_data.addr_offset;
			new_data.range(21,20) = pkt_data.opcode.to_uint();
			new_data.range(19,18) = pkt_data.src.to_uint();
			new_data.range(17,16) = pkt_data.dest.to_uint();
			new_data.range(15,0) = pkt_data.data;
			pkt_out.write(new_data);
			cout << ".........................." << endl;
			cout << "New Packet Sent" << endl;
			cout << "OPCODE: " << pkt_data.opcode.to_string() << " ";
			cout << "Source Addresses: " << pkt_data.src.to_string() << " ";
			cout << "Destination Addresses: " << pkt_data.dest.to_string() << " ";
			cout << "Packet Address Offset: " << pkt_data.addr_offset.to_string(SC_HEX) << endl;
			cout << "Packet Value: " << pkt_data.data.to_string(SC_HEX) << endl;
			cout << "Sender ID: " << (unsigned int)source_id.read() << endl;
			cout << "UINT Packet: 0x" << hex << new_data << dec << endl;
			cout << ".........................." << endl;
		}else if (count == 11){
			break;
		}
		/////wait for 1 to 3 clock periods/////////////////////
		wait(10);
		count++;
		pkt_count++;
	}
	if (source_id.read().to_uint() == 0x0){
		wait(100000);
		sc_stop();
	}
}
