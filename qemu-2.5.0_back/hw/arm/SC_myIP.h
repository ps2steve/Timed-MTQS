#ifdef QEMU_SC
#include "SC_myIP.h"
#endif
 
#include "systemc.h"
#include <stdio.h>
#include <stdlib.h>
 
//#ifndef myIP_H
//#define myIP_H
 
class SC_myIP : public sc_module
{
    public:
    int tmp_32;
    int tmp_16;
    void draw_LCD();
 
   SC_CTOR(SC_myIP)
   {
    SC_THREAD(draw_LCD);
   }
 
//   private:
    sc_event flag_1;
};
 
//#endif
