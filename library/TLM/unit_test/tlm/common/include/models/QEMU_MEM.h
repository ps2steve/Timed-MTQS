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

#ifndef __QEMU_LT_TARGET1_H__
#define __QEMU_LT_TARGET1_H__

#include "tlm.h"
#include <cassert>
#include <vector>


class QEMU_MEM :
  public sc_core::sc_module,
  public virtual tlm::tlm_fw_transport_if<>
{
public:
  typedef tlm::tlm_phase                phase_type;
  typedef tlm::tlm_sync_enum            sync_enum_type;
  typedef tlm::tlm_generic_payload      transaction_type;
  typedef tlm::tlm_target_socket<32>    target_socket_type;

public:
  target_socket_type socket;

public:
  SC_HAS_PROCESS(QEMU_MEM);
  QEMU_MEM(sc_core::sc_module_name name) :
      sc_core::sc_module(name),
      socket("socket")
  {
    // Bind this target's interface to the target socket
    socket(*this);
  }

  sync_enum_type nb_transport_fw(transaction_type& trans, phase_type& phase, sc_core::sc_time& t)
  {
    //Target never calls wait, so we can do this
    b_transport(trans, t); 

    return tlm::TLM_COMPLETED;
  }
  
  unsigned int transport_dbg(transaction_type& r)
  {
	fprintf(stderr,"error in QEMU_MEM.h:transport_dbg\n");
    	return 0;
  }
  bool get_direct_mem_ptr(transaction_type& trans,
                          tlm::tlm_dmi&  dmi_data)
  {
	fprintf(stderr,"error in QEMU_MEM.h:get_direct_mem_ptr\n");
	return false;
  }

  void b_transport(transaction_type& trans, sc_core::sc_time &t)
  {
    sc_dt::uint64 address = trans.get_address();
    //assert(address < 400);
    //cout << "dummy_mem! " << sc_time_stamp() << endl;

    unsigned int& data = *reinterpret_cast<unsigned int*>(trans.get_data_ptr());
/*
    if(address>0){//not fake address
	if(trans.get_command() == tlm::TLM_READ_COMMAND)
	    cpu_physical_memory_read(addr  ,(uint8_t *)&data,4);
	if(trans.get_command() == tlm::TLM_WRITE_COMMAND)
	    cpu_physical_memory_write(addr  ,(uint8_t *)&data,2);//for framebuffer wrtie
    }
*/
    if (trans.get_command() == tlm::TLM_WRITE_COMMAND) 
/*      std::cout << name() << ": Received write request: A = 0x"
                << std::hex << (unsigned int)address
                << ", D = 0x" << data << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;
*/
//      *reinterpret_cast<unsigned int*>(&mMem[address]) = data;
      t =  sc_core::sc_time(2, sc_core::SC_NS);
    else if (trans.get_command() == tlm::TLM_READ_COMMAND) 
      t =  sc_core::sc_time(2, sc_core::SC_NS);
    else if (trans.get_command() == tlm::TLM_BURST8_WRITE_COMMAND) 
      t =  sc_core::sc_time(9, sc_core::SC_NS);
    else if (trans.get_command() == tlm::TLM_BURST8_READ_COMMAND) 
      t =  sc_core::sc_time(9, sc_core::SC_NS);
    else {
/*      std::cout << name() << ": Received read request: A = 0x"
                << std::hex << (unsigned int)address << std::dec
                << " @ " << sc_core::sc_time_stamp() << std::endl;
*/
//      data = *reinterpret_cast<unsigned int*>(&mMem[address]);
      //t = sc_core::sc_time(2, sc_core::SC_NS);
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);

//    trans.set_dmi_allowed(true);
  }

private:
};

#endif
