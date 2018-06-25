#include "systemc.h"
#include "hls_bus_if.h"
#include "packet.h"
#include "sender.h"
#include "receiver.h"
#ifdef __RTL_SIMULATION__
#include "four_port_sw_rtl_wrapper.h"
#define four_port_sw four_port_sw_rtl_wrapper
#else
#include "four_port_sw.h"
#endif

#define DUMP_BUS_OP

int
sc_main(int, char *[])
{

  // Remove simulation warnings
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  sc_report_handler::set_actions( SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
  sc_report_handler::set_actions( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);
  sc_report_handler::set_actions( SC_ID_OBJECT_EXISTS_, SC_LOG);

  sc_signal<sc_uint<32> > pkt_in0;
  sc_signal<sc_uint<32> > pkt_in1;
  sc_signal<sc_uint<32> > pkt_in2;
  sc_signal<sc_uint<32> > pkt_in3;
  sc_signal<sc_uint<32> > pkt_in_in0;
  sc_signal<sc_uint<32> > pkt_in_in1;
  sc_signal<sc_uint<32> > pkt_in_in2;
  sc_signal<sc_uint<32> > pkt_in_in3;
  sc_signal<bool> pkt_out0_ready;
  sc_signal<bool> pkt_out1_ready;
  sc_signal<bool> pkt_out2_ready;
  sc_signal<bool> pkt_out3_ready;
  sc_signal<bool> pkt_out0_done;
  sc_signal<bool> pkt_out1_done;
  sc_signal<bool> pkt_out2_done;
  sc_signal<bool> pkt_out3_done;
  sc_signal<unsigned int> pkt_out_0_addr;
  sc_signal<unsigned int> pkt_out_1_addr;
  sc_signal<unsigned int> pkt_out_2_addr;
  sc_signal<unsigned int> pkt_out_3_addr;


  hls_bus_chn<sc_uint<32> > bus_mem0("bus_mem0",0x40000000,0x40000FFF);
  hls_bus_chn<sc_uint<32> > bus_mem1("bus_mem1",0x40001000,0x40001FFF);
  hls_bus_chn<sc_uint<32> > bus_mem2("bus_mem2",0x40002000,0x40002FFF);
  hls_bus_chn<sc_uint<32> > bus_mem3("bus_mem3",0x40003000,0x40003FFF);

  sc_signal<bool> pkt_sw_reset;

  sc_signal<unsigned int> pkt_out0p;
  sc_signal<unsigned int> pkt_out0d;
  sc_signal<unsigned int> pkt_out0f;
  sc_signal<unsigned int> pkt_out1p;
  sc_signal<unsigned int> pkt_out1d;
  sc_signal<unsigned int> pkt_out1f;
  sc_signal<unsigned int> pkt_out2p;
  sc_signal<unsigned int> pkt_out2d;
  sc_signal<unsigned int> pkt_out2f;
  sc_signal<unsigned int> pkt_out3p;
  sc_signal<unsigned int> pkt_out3d;
  sc_signal<unsigned int> pkt_out3f;
  sc_signal<sc_uint<32> > fw_debug_signal[12];

  sc_signal<sc_uint<32> > setup_upper_s[4];
  sc_signal<sc_uint<32> > setup_lower_s[4];
  sc_signal<sc_uint<32> > setup_rule_s[4];

  sc_signal<sc_int<4> > id0, id1, id2, id3;

  sc_clock clock1("CLOCK1", 75, SC_NS, 0.5, 0.0, SC_NS);

  sender sender0("SENDER0");
  sender0.pkt_out(pkt_in0);
  sender0.pkt_in(pkt_in_in0);
  sender0.source_id(id0);
  sender0.setup_upper(setup_upper_s[0]);
  sender0.setup_lower(setup_lower_s[0]);
  sender0.setup_rule(setup_rule_s[0]);
  sender0.CLK(clock1);

  sender sender1("SENDER1");
  sender1.pkt_out(pkt_in1);
  sender1.pkt_in(pkt_in_in1);
  sender1.source_id(id1);
  sender1.setup_upper(setup_upper_s[1]);
  sender1.setup_lower(setup_lower_s[1]);
  sender1.setup_rule(setup_rule_s[1]);
  sender1.CLK(clock1);

  sender sender2("SENDER2");
  sender2.pkt_out(pkt_in2);
  sender2.pkt_in(pkt_in_in2);
  sender2.source_id(id2);
  sender2.setup_upper(setup_upper_s[2]);
  sender2.setup_lower(setup_lower_s[2]);
  sender2.setup_rule(setup_rule_s[2]);
  sender2.CLK(clock1);

  sender sender3("SENDER3");
  sender3.pkt_out(pkt_in3);
  sender3.pkt_in(pkt_in_in3);
  sender3.source_id(id3);
  sender3.setup_upper(setup_upper_s[3]);
  sender3.setup_lower(setup_lower_s[3]);
  sender3.setup_rule(setup_rule_s[3]);
  sender3.CLK(clock1);

  four_port_sw switch1("SWITCH");
  switch1.CLK(clock1);
  switch1.in_0(pkt_in0);
  switch1.in_1(pkt_in1);
  switch1.in_2(pkt_in2);
  switch1.in_3(pkt_in3);
  switch1.in_responce_0(pkt_in_in0);
  switch1.in_responce_1(pkt_in_in1);
  switch1.in_responce_2(pkt_in_in2);
  switch1.in_responce_3(pkt_in_in3);
  switch1.out_0(bus_mem0);
  switch1.out_1(bus_mem1);
  switch1.out_2(bus_mem2);
  switch1.out_3(bus_mem3);
  switch1.out_addr_to_read_0(pkt_out_0_addr);
  switch1.out_addr_to_read_1(pkt_out_1_addr);
  switch1.out_addr_to_read_2(pkt_out_2_addr);
  switch1.out_addr_to_read_3(pkt_out_3_addr);
  switch1.done_0(pkt_out0_done);
  switch1.done_1(pkt_out1_done);
  switch1.done_2(pkt_out2_done);
  switch1.done_3(pkt_out3_done);
  switch1.ready_0(pkt_out0_ready);
  switch1.ready_1(pkt_out1_ready);
  switch1.ready_2(pkt_out2_ready);
  switch1.ready_3(pkt_out3_ready);
  switch1.total_0(pkt_out0p);
  switch1.total_1(pkt_out1p);
  switch1.total_2(pkt_out2p);
  switch1.total_3(pkt_out3p);
  switch1.droped_0(pkt_out0d);
  switch1.droped_1(pkt_out1d);
  switch1.droped_2(pkt_out2d);
  switch1.droped_3(pkt_out3d);
  switch1.fw_rule_hit_counter_0(pkt_out0f);
  switch1.fw_rule_hit_counter_1(pkt_out1f);
  switch1.fw_rule_hit_counter_2(pkt_out2f);
  switch1.fw_rule_hit_counter_3(pkt_out3f);
  switch1.setup_lower_range_0(setup_lower_s[0]);
  switch1.setup_lower_range_1(setup_lower_s[1]);
  switch1.setup_lower_range_2(setup_lower_s[2]);
  switch1.setup_lower_range_3(setup_lower_s[3]);
  switch1.setup_upper_range_0(setup_upper_s[0]);
  switch1.setup_upper_range_1(setup_upper_s[1]);
  switch1.setup_upper_range_2(setup_upper_s[2]);
  switch1.setup_upper_range_3(setup_upper_s[3]);
  switch1.setup_rule_range_0(setup_rule_s[0]);
  switch1.setup_rule_range_1(setup_rule_s[1]);
  switch1.setup_rule_range_2(setup_rule_s[2]);
  switch1.setup_rule_range_3(setup_rule_s[3]);
  switch1.fw_hreg_p_0(fw_debug_signal[0]);
  switch1.fw_hreg_p_1(fw_debug_signal[1]);
  switch1.fw_hreg_p_2(fw_debug_signal[2]);
  switch1.fw_hreg_p_3(fw_debug_signal[3]);
  switch1.fw_lreg_p_0(fw_debug_signal[4]);
  switch1.fw_lreg_p_1(fw_debug_signal[5]);
  switch1.fw_lreg_p_2(fw_debug_signal[6]);
  switch1.fw_lreg_p_3(fw_debug_signal[7]);
  switch1.fw_rreg_p_0(fw_debug_signal[8]);
  switch1.fw_rreg_p_1(fw_debug_signal[9]);
  switch1.fw_rreg_p_2(fw_debug_signal[10]);
  switch1.fw_rreg_p_3(fw_debug_signal[11]);
  switch1.reset(pkt_sw_reset);

  receiver receiver0("RECEIVER0");
  receiver0.pkt_in(bus_mem0);
  receiver0.out_addr_to_read(pkt_out_0_addr);
  receiver0.sink_id(id0);
  receiver0.count(pkt_out0p);
  receiver0.dropped(pkt_out0d);
  receiver0.firewalled(pkt_out0f);
  receiver0.ready(pkt_out0_ready);
  receiver0.done(pkt_out0_done);
  receiver0.fw_hreg(fw_debug_signal[0]);
  receiver0.fw_lreg(fw_debug_signal[4]);
  receiver0.fw_rreg(fw_debug_signal[8]);

  receiver0.CLK(clock1);

  receiver receiver1("RECEIVER1");
  receiver1.pkt_in(bus_mem1);
  receiver1.out_addr_to_read(pkt_out_1_addr);
  receiver1.sink_id(id1);
  receiver1.count(pkt_out1p);
  receiver1.dropped(pkt_out1d);
  receiver1.firewalled(pkt_out1f);
  receiver1.ready(pkt_out1_ready);
  receiver1.done(pkt_out1_done);
  receiver1.fw_hreg(fw_debug_signal[1]);
  receiver1.fw_lreg(fw_debug_signal[5]);
  receiver1.fw_rreg(fw_debug_signal[9]);
  receiver1.CLK(clock1);

  receiver receiver2("RECEIVER2");
  receiver2.pkt_in(bus_mem2);
  receiver2.out_addr_to_read(pkt_out_2_addr);
  receiver2.sink_id(id2);
  receiver2.count(pkt_out2p);
  receiver2.dropped(pkt_out2d);
  receiver2.firewalled(pkt_out2f);
  receiver2.ready(pkt_out2_ready);
  receiver2.done(pkt_out2_done);
  receiver2.fw_hreg(fw_debug_signal[2]);
  receiver2.fw_lreg(fw_debug_signal[6]);
  receiver2.fw_rreg(fw_debug_signal[10]);
  receiver2.CLK(clock1);

  receiver receiver3("RECEIVER3");
  // hooking up signals to ports by position
  receiver3.pkt_in(bus_mem3);
  receiver3.out_addr_to_read(pkt_out_3_addr);
  receiver3.sink_id(id3);
  receiver3.count(pkt_out3p);
  receiver3.dropped(pkt_out3d);
  receiver3.firewalled(pkt_out3f);
  receiver3.ready(pkt_out3_ready);
  receiver3.done(pkt_out3_done);
  receiver3.fw_hreg(fw_debug_signal[3]);
  receiver3.fw_lreg(fw_debug_signal[7]);
  receiver3.fw_rreg(fw_debug_signal[11]);
  receiver3.CLK(clock1);

  id0.write(0);
  id1.write(1);
  id2.write(2);
  id3.write(3);

  pkt_sw_reset.write(true);
  sc_start(1000, SC_NS);
  pkt_sw_reset.write(false);

  sc_start();
  return 0;

}
