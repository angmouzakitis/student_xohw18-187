#ifndef FOUR_PORT_SW_H_INCLUDED
#define FOUR_PORT_SW_H_INCLUDED

#ifndef SYSTEM_C_H_INCLUDED
#define SYSTEM_C_H_INCLUDED
#define SC_INCLUDE_FX
#include "systemc.h"
#include "AXI4_if.h"
#include"tlm.h"
using namespace tlm;
#endif // SYSTEM_C_H_INCLUDED

#include "packet.h"

#define INTERNAL_FIFO_SIZE 10

SC_MODULE (four_port_sw) {
  //SW Outside Ports
  sc_in<sc_uint<32> >   in_0;
  sc_in<sc_uint<32> >   in_1;
  sc_in<sc_uint<32> >   in_2;
  sc_in<sc_uint<32> >   in_3;
  sc_out<sc_uint<32> >  in_responce_0;
  sc_out<sc_uint<32> >  in_responce_1;
  sc_out<sc_uint<32> >  in_responce_2;
  sc_out<sc_uint<32> >  in_responce_3;
  AXI4M_bus_port<sc_uint<32> >  out_0;
  AXI4M_bus_port<sc_uint<32> >  out_1;
  AXI4M_bus_port<sc_uint<32> >  out_2;
  AXI4M_bus_port<sc_uint<32> >  out_3;
  //firewall setup interface
  sc_in<sc_uint<32> >   setup_lower_range_0;
  sc_in<sc_uint<32> >   setup_lower_range_1;
  sc_in<sc_uint<32> >   setup_lower_range_2;
  sc_in<sc_uint<32> >   setup_lower_range_3;
  sc_in<sc_uint<32> >   setup_upper_range_0;
  sc_in<sc_uint<32> >   setup_upper_range_1;
  sc_in<sc_uint<32> >   setup_upper_range_2;
  sc_in<sc_uint<32> >   setup_upper_range_3;
  sc_in<sc_uint<32> >   setup_rule_range_0;
  sc_in<sc_uint<32> >   setup_rule_range_1;
  sc_in<sc_uint<32> >   setup_rule_range_2;
  sc_in<sc_uint<32> >   setup_rule_range_3;
  //stats interface
  sc_out<unsigned int>   total_0;
  sc_out<unsigned int>   total_1;
  sc_out<unsigned int>   total_2;
  sc_out<unsigned int>   total_3;
  sc_out<unsigned int>   droped_0;
  sc_out<unsigned int>   droped_1;
  sc_out<unsigned int>   droped_2;
  sc_out<unsigned int>   droped_3;
  sc_out<unsigned int>   fw_rule_hit_counter_0;
  sc_out<unsigned int>   fw_rule_hit_counter_1;
  sc_out<unsigned int>   fw_rule_hit_counter_2;
  sc_out<unsigned int>   fw_rule_hit_counter_3;
  sc_out<sc_uint<32> >   fw_hreg_p_0;
  sc_out<sc_uint<32> >   fw_hreg_p_1;
  sc_out<sc_uint<32> >   fw_hreg_p_2;
  sc_out<sc_uint<32> >   fw_hreg_p_3;
  sc_out<sc_uint<32> >   fw_lreg_p_0;
  sc_out<sc_uint<32> >   fw_lreg_p_1;
  sc_out<sc_uint<32> >   fw_lreg_p_2;
  sc_out<sc_uint<32> >   fw_lreg_p_3;
  sc_out<sc_uint<32> >   fw_rreg_p_0;
  sc_out<sc_uint<32> >   fw_rreg_p_1;
  sc_out<sc_uint<32> >   fw_rreg_p_2;
  sc_out<sc_uint<32> >   fw_rreg_p_3;
  //Clock
  sc_in <bool>      CLK;
  //reset
  sc_in<bool>       reset;


  //SW Inside Fifos/ports
  sc_fifo<sc_uint<32> > in_fifo_0;
  sc_fifo<sc_uint<32> > in_fifo_1;
  sc_fifo<sc_uint<32> > in_fifo_2;
  sc_fifo<sc_uint<32> > in_fifo_3;
  sc_fifo<sc_uint<32> > out_fifo_s1_0;
  sc_fifo<sc_uint<32> > out_fifo_s1_1;
  sc_fifo<sc_uint<32> > out_fifo_s2_0;
  sc_fifo<sc_uint<32> > out_fifo_s2_1;
  sc_fifo<sc_uint<32> > out_fifo_s3_0;
  sc_fifo<sc_uint<32> > out_fifo_s3_1;
  sc_fifo<sc_uint<32> > out_fifo_s4_0;
  sc_fifo<sc_uint<32> > out_fifo_s4_1;
  sc_fifo<sc_uint<32> > out_fifo_0;
  sc_fifo<sc_uint<32> > out_fifo_1;
  sc_fifo<sc_uint<32> > out_fifo_2;
  sc_fifo<sc_uint<32> > out_fifo_3;
  sc_fifo<sc_uint<32> > responce_fifo_0;
  sc_fifo<sc_uint<32> > responce_fifo_1;
  sc_fifo<sc_uint<32> > responce_fifo_2;
  sc_fifo<sc_uint<32> > responce_fifo_3;
  sc_fifo<sc_uint<32> > init_responce_fifo_0;
  sc_fifo<sc_uint<32> > init_responce_fifo_1;
  sc_fifo<sc_uint<32> > init_responce_fifo_2;
  sc_fifo<sc_uint<32> > init_responce_fifo_3;

#ifndef __SYNTHESIS__
  sc_out<unsigned int>  out_addr_to_read_0;
  sc_out<unsigned int>  out_addr_to_read_1;
  sc_out<unsigned int>  out_addr_to_read_2;
  sc_out<unsigned int>  out_addr_to_read_3;
  sc_in<bool>       done_0;
  sc_in<bool>       done_1;
  sc_in<bool>       done_2;
  sc_in<bool>       done_3;
  sc_out<bool>      ready_0;
  sc_out<bool>      ready_1;
  sc_out<bool>      ready_2;
  sc_out<bool>      ready_3;
  bool preready[4];
#endif
  //internal used variables
  sc_signal<sc_uint<32> > responce_s0;
  sc_signal<sc_uint<32> > responce_s1;
  sc_signal<sc_uint<32> > responce_s2;
  sc_signal<sc_uint<32> > responce_s3;
  sc_uint<32> responce0;
  sc_uint<32> responce1;
  sc_uint<32> responce2;
  sc_uint<32> responce3;
  sc_signal<sc_uint<32> > fw_lower_range_0;
  sc_signal<sc_uint<32> > fw_lower_range_1;
  sc_signal<sc_uint<32> > fw_lower_range_2;
  sc_signal<sc_uint<32> > fw_lower_range_3;
  sc_signal<sc_uint<32> > fw_upper_range_0;
  sc_signal<sc_uint<32> > fw_upper_range_1;
  sc_signal<sc_uint<32> > fw_upper_range_2;
  sc_signal<sc_uint<32> > fw_upper_range_3;
  sc_signal<sc_uint<32> > fw_rule_range_0;
  sc_signal<sc_uint<32> > fw_rule_range_1;
  sc_signal<sc_uint<32> > fw_rule_range_2;
  sc_signal<sc_uint<32> > fw_rule_range_3;
  sc_uint<32> count_total_0;
  sc_uint<32> count_total_1;
  sc_uint<32> count_total_2;
  sc_uint<32> count_total_3;
  sc_uint<32> count_droped_0;
  sc_uint<32> count_droped_1;
  sc_uint<32> count_droped_2;
  sc_uint<32> count_droped_3;
  sc_uint<32> rule_hit_counter_0;
  sc_uint<32> rule_hit_counter_1;
  sc_uint<32> rule_hit_counter_2;
  sc_uint<32> rule_hit_counter_3;

  //input handler method
  void interface_in_0();
  void interface_in_1();
  void interface_in_2();
  void interface_in_3();
  void interface_out_0();
  void interface_out_1();
  void interface_out_2();
  void interface_out_3();
  void entry_sw_1();
  void entry_sw_2();
  void entry_sw_3();
  void entry_sw_4();
  void entry_response();
  void setupfw_rule();

  //Constructor
  SC_CTOR(four_port_sw)
  :out_0 ("out_0"), out_1 ("out_1"), out_2 ("out_2"), out_3 ("out_3"),
   in_fifo_0(INTERNAL_FIFO_SIZE),in_fifo_1(INTERNAL_FIFO_SIZE),
   in_fifo_2(INTERNAL_FIFO_SIZE),in_fifo_3(INTERNAL_FIFO_SIZE),
   out_fifo_s1_0(INTERNAL_FIFO_SIZE),out_fifo_s1_1(INTERNAL_FIFO_SIZE),
   out_fifo_s2_0(INTERNAL_FIFO_SIZE),out_fifo_s2_1(INTERNAL_FIFO_SIZE),
   out_fifo_s3_0(INTERNAL_FIFO_SIZE),out_fifo_s3_1(INTERNAL_FIFO_SIZE),
   out_fifo_s4_0(INTERNAL_FIFO_SIZE),out_fifo_s4_1(INTERNAL_FIFO_SIZE),
   out_fifo_0(INTERNAL_FIFO_SIZE),out_fifo_1(INTERNAL_FIFO_SIZE),
   out_fifo_2(INTERNAL_FIFO_SIZE),out_fifo_3(INTERNAL_FIFO_SIZE),
   responce_fifo_0(INTERNAL_FIFO_SIZE),responce_fifo_1(INTERNAL_FIFO_SIZE),
   responce_fifo_2(INTERNAL_FIFO_SIZE),responce_fifo_3(INTERNAL_FIFO_SIZE),
   init_responce_fifo_0(INTERNAL_FIFO_SIZE),init_responce_fifo_1(INTERNAL_FIFO_SIZE),
   init_responce_fifo_2(INTERNAL_FIFO_SIZE),init_responce_fifo_3(INTERNAL_FIFO_SIZE)
  {
#pragma HLS RESOURCE variable=setup_lower_range_0 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_upper_range_0 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_rule_range_0 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_lower_range_1 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_upper_range_1 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_rule_range_1 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_lower_range_2 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_upper_range_2 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_rule_range_2 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_lower_range_3 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_upper_range_3 core=AXI4LiteS metadata="-bus_bundle fw_setup"
#pragma HLS RESOURCE variable=setup_rule_range_3 core=AXI4LiteS metadata="-bus_bundle fw_setup"      
#pragma HLS RESOURCE variable=in_0 core=AXI4LiteS metadata="-bus_bundle slv0"
#pragma HLS RESOURCE variable=in_responce_0 core=AXI4LiteS metadata="-bus_bundle slv0"
#pragma HLS RESOURCE variable=total_0 core=AXI4LiteS metadata="-bus_bundle slv0"
#pragma HLS RESOURCE variable=droped_0 core=AXI4LiteS metadata="-bus_bundle slv0"
#pragma HLS RESOURCE variable=fw_rule_hit_counter_0 core=AXI4LiteS metadata="-bus_bundle slv0"
#pragma HLS RESOURCE variable=in_1 core=AXI4LiteS metadata="-bus_bundle slv1"
#pragma HLS RESOURCE variable=in_responce_1 core=AXI4LiteS metadata="-bus_bundle slv1"
#pragma HLS RESOURCE variable=total_1 core=AXI4LiteS metadata="-bus_bundle slv1"
#pragma HLS RESOURCE variable=droped_1 core=AXI4LiteS metadata="-bus_bundle slv1"
#pragma HLS RESOURCE variable=fw_rule_hit_counter_1 core=AXI4LiteS metadata="-bus_bundle slv1"
#pragma HLS RESOURCE variable=in_2 core=AXI4LiteS metadata="-bus_bundle slv2"
#pragma HLS RESOURCE variable=in_responce_2 core=AXI4LiteS metadata="-bus_bundle slv2"
#pragma HLS RESOURCE variable=total_2 core=AXI4LiteS metadata="-bus_bundle slv2"
#pragma HLS RESOURCE variable=droped_2 core=AXI4LiteS metadata="-bus_bundle slv2"
#pragma HLS RESOURCE variable=fw_rule_hit_counter_2 core=AXI4LiteS metadata="-bus_bundle slv2"
#pragma HLS RESOURCE variable=in_3 core=AXI4LiteS metadata="-bus_bundle slv3"
#pragma HLS RESOURCE variable=in_responce_3 core=AXI4LiteS metadata="-bus_bundle slv3"
#pragma HLS RESOURCE variable=total_3 core=AXI4LiteS metadata="-bus_bundle slv3"
#pragma HLS RESOURCE variable=droped_3 core=AXI4LiteS metadata="-bus_bundle slv3"
#pragma HLS RESOURCE variable=fw_rule_hit_counter_3 core=AXI4LiteS metadata="-bus_bundle slv3"      
#pragma HLS RESOURCE variable=fw_hreg_p_0 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_hreg_p_1 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_hreg_p_2 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_hreg_p_3 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_lreg_p_0 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_lreg_p_1 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_lreg_p_2 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_lreg_p_3 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_rreg_p_0 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_rreg_p_1 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_rreg_p_2 core=AXI4LiteS metadata="-bus_bundle debugfw"
#pragma HLS RESOURCE variable=fw_rreg_p_3 core=AXI4LiteS metadata="-bus_bundle debugfw"      
#pragma HLS RESOURCE variable=out_0 core=AXI4M
#pragma HLS RESOURCE variable=out_1 core=AXI4M
#pragma HLS RESOURCE variable=out_2 core=AXI4M
#pragma HLS RESOURCE variable=out_3 core=AXI4M
#pragma HLS RESOURCE variable=out_fifo_3 core=FIFO
#pragma HLS RESOURCE variable=in_fifo_3 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_2 core=FIFO
#pragma HLS RESOURCE variable=in_fifo_2 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_1 core=FIFO
#pragma HLS RESOURCE variable=in_fifo_1 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_0 core=FIFO
#pragma HLS RESOURCE variable=in_fifo_0 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s1_0 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s1_1 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s2_0 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s2_1 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s3_0 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s3_1 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s4_0 core=FIFO
#pragma HLS RESOURCE variable=out_fifo_s4_1 core=FIFO
#pragma HLS RESOURCE variable=responce_fifo_0 core=FIFO
#pragma HLS RESOURCE variable=responce_fifo_1 core=FIFO
#pragma HLS RESOURCE variable=responce_fifo_2 core=FIFO
#pragma HLS RESOURCE variable=responce_fifo_3 core=FIFO
  SC_CTHREAD(interface_in_0, CLK.pos());
  SC_CTHREAD(interface_in_1, CLK.pos());
  SC_CTHREAD(interface_in_2, CLK.pos());
  SC_CTHREAD(interface_in_3, CLK.pos());
  SC_CTHREAD(setupfw_rule, CLK.pos());
  SC_CTHREAD(interface_out_0, CLK.pos());
  SC_CTHREAD(interface_out_1, CLK.pos());
  SC_CTHREAD(interface_out_2, CLK.pos());
  SC_CTHREAD(interface_out_3, CLK.pos());
  SC_CTHREAD(entry_sw_1, CLK.pos());
  SC_CTHREAD(entry_sw_2, CLK.pos());
  SC_CTHREAD(entry_sw_3, CLK.pos());
  SC_CTHREAD(entry_sw_4, CLK.pos());
  SC_CTHREAD(entry_response, CLK.pos());
  }

};

#endif //FOUR_PORT_SW_H_INCLUDED
