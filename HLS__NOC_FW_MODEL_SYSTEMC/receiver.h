#ifndef RECEIVER_H_INCLUDED
#define RECEIVER_H_INCLUDED

#ifndef SYSTEM_C_H_INCLUDED
#define SYSTEM_C_H_INCLUDED
#include "systemc.h"
#include "AXI4_if.h"
#include"tlm.h"
#endif // SYSTEM_C_H_INCLUDED
using namespace tlm;
#include "packet.h"

struct receiver: sc_module {
  AXI4M_bus_port<sc_uint<32> >  pkt_in;
  sc_in<sc_int<4> >   sink_id;
  sc_in<unsigned int> count;
  sc_in<unsigned int> dropped;
  sc_in<unsigned int> firewalled;
  sc_in<bool>         ready;
  sc_out<bool>        done;
  sc_in<unsigned int> out_addr_to_read;
  sc_in<sc_uint<32> > fw_hreg;
  sc_in<sc_uint<32> > fw_lreg;
  sc_in<sc_uint<32> > fw_rreg;
  sc_in_clk           CLK;

  SC_CTOR(receiver):pkt_in ("pkt_in") {
      SC_CTHREAD(entry, CLK.pos());
    }

 void entry();

};
#endif // RECEIVER_H_INCLUDED
