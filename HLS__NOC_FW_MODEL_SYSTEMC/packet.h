#ifndef PACKET_H_INCLUDED
#define PACKET_H_INCLUDED

#include "systemc.h"

struct pkt {
       sc_uint<10> addr_offset;
       sc_bv<2> opcode;
       sc_bv<2> src;
       sc_bv<2> dest;
       sc_int<16> data;

       inline bool operator == (const pkt& rhs) const
   {
     return (rhs.data == data && rhs.addr_offset == addr_offset &&
    		 rhs.opcode[0] == opcode[0] && rhs.opcode[1] == opcode[1] &&
    		 rhs.src[0] == src[0] && rhs.src[1] == src[1] &&
    		 rhs.dest[0] == dest[0] && rhs.dest[1] == dest[1]);
   }

};

inline
ostream&
operator << ( ostream& os, const pkt& a)
{
    os << "PKT " << " addr_offset " << a.addr_offset.to_string(SC_HEX)
    		<< " data " << a.data.to_string(SC_HEX)
    		<< " opcode0 " << a.opcode[0] << " opcode1 " << a.opcode[1]
    		<< " src0 " << a.src[0] << " src1 " << a.src[1]
    		<< " dest0 " << a.dest[0] << " dest1 " << a.dest[1];
    return os;
}

inline
void
    sc_trace( sc_trace_file* tf, const pkt& a, const std::string& name )
{
  sc_trace( tf, a.addr_offset, name + ".addr_offset" );
  sc_trace( tf, a.data, name + ".data" );
  sc_trace( tf, a.opcode[0], name + ".opcode0" );
  sc_trace( tf, a.opcode[1], name + ".opcode1" );
  sc_trace( tf, a.src[0], name + ".src0" );
  sc_trace( tf, a.src[1], name + ".src1" );
  sc_trace( tf, a.dest[0], name + ".dest0" );
  sc_trace( tf, a.dest[1], name + ".dest1" );
}
#endif // PACKET_H_INCLUDED
