#ifndef SENDER_H_INCLUDED
#define SENDER_H_INCLUDED

#include "systemc.h"
#include "packet.h"

struct sender: sc_module {
  sc_out<sc_uint<32> > pkt_out;
  sc_in<sc_uint<32> > pkt_in;
  sc_in<sc_int<4> > source_id;
  sc_in_clk CLK;
  sc_out<sc_uint<32> > setup_upper;
  sc_out<sc_uint<32> > setup_lower;
  sc_out<sc_uint<32> > setup_rule;

  SC_CTOR(sender)
    {
      SC_CTHREAD(entry, CLK.pos());
     }
  void entry();
};
#endif // SENDER_H_INCLUDED
