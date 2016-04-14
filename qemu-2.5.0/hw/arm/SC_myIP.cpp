//#include <systemc.h>
#include "SC_myIP.h"
#include "stdio.h"

extern SC_myIP *myIP;

void SC_myIP::draw_LCD()
{
	while(1)
	{
		wait(myIP->flag_1);
		tmp_16 = ((tmp_32 & 0xff) >> 3) |   /* red */
                    ((tmp_32 & 0xfc00) >> 5) |      /* green */
                    ((tmp_32 & 0xf80000) >> 8);     /* blue */
	}
}
