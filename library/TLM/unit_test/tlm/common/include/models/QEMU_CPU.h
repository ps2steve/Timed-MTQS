/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

//====================================================================
//  Nov 06, 2008
//
//  Updated by:
//    Xiaopeng Qiu, JEDA Technologies, Inc
//    Email:  qiuxp@jedatechnologies.net
//
//  To fix violations of TLM2.0 rules, which are detected by JEDA 
//  TLM2.0 checker.
//
//====================================================================

#ifndef __SIMPLE_LT_INITIATOR1_H__
#define __SIMPLE_LT_INITIATOR1_H__

#include "tlm.h"     /// TLM definitions
#include <cassert>   /// STD assert ()
#include <stdint.h>

extern sc_event rw_event;
extern "C"{
	extern void hw_log(char *name, char *state, uint64_t time_begin, uint64_t time_end);
	extern uint64_t total_cycle;
}


class QEMU_CPU :
  public sc_core::sc_module,
  public virtual tlm::tlm_bw_transport_if<>
{
public:
  typedef tlm::tlm_generic_payload        transaction_type;
  typedef tlm::tlm_phase                  phase_type;
  typedef tlm::tlm_sync_enum              sync_enum_type;
  typedef tlm::tlm_initiator_socket<32>   initiator_socket_type;

public:
  initiator_socket_type socket;

public:
  SC_HAS_PROCESS(QEMU_CPU);
  QEMU_CPU(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    socket("socket")
  {
    // Bind this initiator's interface to the initiator socket
    socket(*this);

    // Initiator thread
    SC_THREAD(run);
  }
/*
  bool initTransaction(transaction_type& trans)
  {
    if (mTransactionCount < mNrOfTransactions) {
      trans.set_address(mBaseAddress + 4*mTransactionCount);
      mData = mTransactionCount;
      trans.set_command(tlm::TLM_WRITE_COMMAND);

    } else if (mTransactionCount < 2 * mNrOfTransactions) {
      trans.set_address(mBaseAddress + 4*(mTransactionCount - mNrOfTransactions));
      mData = 0;
      trans.set_command(tlm::TLM_READ_COMMAND);

    } else {
      return false;
    }

    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&mData));
    trans.set_data_length(4);
    trans.set_streaming_width(4);
    trans.set_dmi_allowed(false);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    ++mTransactionCount;
    return true;
  }
*/
  void run()
  {
	sc_core::sc_time t(sc_core::SC_ZERO_TIME);
	transaction_type trans;
	trans.set_data_ptr(reinterpret_cast<unsigned char*>(&mData));
	trans.set_data_length(4);
	trans.set_streaming_width(4);
	trans.set_dmi_allowed(false);
	trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	extern bool cpu_ready;
	uint32_t Icache_miss_list[2][50], Dcache_r_miss_list[2][50], Dcache_w_miss_list[2][50];
	uint32_t Icache_miss_list_size, Dcache_r_miss_list_size, Dcache_w_miss_list_size;
#define PHY     1
#define VIR     0
	sc_time t1,t2;
	sc_time cycles;
	uint64_t time_begin,time_end;
	while(1){
		wait(rw_event);
		int i;
		for(i=0; i<Icache_miss_list_size; i++){
			trans.set_address(Icache_miss_list[PHY][i]);
			trans.set_command(tlm::TLM_BURST8_READ_COMMAND);
			socket->b_transport(trans, t);
			wait(t);
		}
		for(i=0; i<Dcache_r_miss_list_size; i++){
			trans.set_address(Dcache_r_miss_list[PHY][i]);
			trans.set_command(tlm::TLM_BURST8_READ_COMMAND);
			socket->b_transport(trans, t);
			wait(t);
		}
		for(i=0; i<Dcache_w_miss_list_size; i++){//write through
			trans.set_address(Dcache_w_miss_list[PHY][i]);
			trans.set_command(tlm::TLM_READ_COMMAND);
			socket->b_transport(trans, t);
			wait(t);
		}
		cpu_ready=true;
		/*
		if(Icache_ready == false){
			t1 = sc_time_stamp();
			time_begin=total_cycle;
			trans.set_command(tlm::TLM_BURST8_READ_COMMAND);
			socket->b_transport(trans, t);
			wait(t);
			t2 = sc_time_stamp();
			cycles = t2 - t1;
			time_end=time_begin+cycles.value();
			//hw_log("CPU", "I$ F", time_begin, time_end);
		}
		if(Dcache_w_ready == false){
			t1 = sc_time_stamp();
			time_begin=total_cycle;
			trans.set_command(tlm::TLM_BURST8_WRITE_COMMAND);
			socket->b_transport(trans, t);
			wait(t);
			t2 = sc_time_stamp();
			cycles = t2 - t1;
			time_end=time_begin+cycles.value();
			//hw_log("CPU", "D$ W", time_begin, time_end);
		}
		if(Dcache_r_ready == false){
			t1 = sc_time_stamp();
			time_begin=total_cycle;
			trans.set_command(tlm::TLM_BURST8_READ_COMMAND);
			socket->b_transport(trans, t);
			wait(t);
			t2 = sc_time_stamp();
			cycles = t2 - t1;
			time_end=time_begin+cycles.value();
			//hw_log("CPU", "D$ R", time_begin, time_end);
		}
		Icache_ready = Dcache_w_ready = Dcache_r_ready = true;
*/
	}
  }

  tlm::tlm_sync_enum nb_transport_bw(transaction_type &,phase_type &,sc_core::sc_time & )
  {
    assert(0);  // should never happen
    return tlm::TLM_COMPLETED;
  }

  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
  {
    // No DMI support: ignore
  }

private:
  unsigned int mData;
};

#endif
