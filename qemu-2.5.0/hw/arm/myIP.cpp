/*
 * Arm custom IP Interface
 *
 * Copyright (c) 2011-2012 NSYSU ESLAB
 * Written by GK mike
 *
 * This code is licenced under the GPL.
 */
 #include "tb_sobel.h"
 #include "sobel.h"

#define DRIVER_NAME "myIP"
#define TYPE_myIP "myIP"
#define myIP(obj) OBJECT_CHECK(myIP_state, (obj), TYPE_myIP)
 sobel s_obj("sobel");
  tb_sobel tb_obj("tb_sobel");
 sc_clock                clk("clk", 25, SC_NS, 0.5, 0, SC_NS, true);
  sc_signal<bool> rst;

  sc_signal<sc_uint<8> > input_row[3];
  sc_signal<sc_uint<8> > output_row;
  int i;
  char name[15];
//myIP = new SC_myIP("myIP");
extern "C"{
#include "hw/sysbus.h"
#include "qemu/timer.h"

typedef struct {
    SysBusDevice busdev;
    qemu_irq irq;
    int reg[4];
    MemoryRegion iomem;
} myIP_state;

#define	QEMU_DEBUG_RW	1
#if QEMU_DEBUG_RW
  #define dprintf(x...) printf(DRIVER_NAME ": " x)
#else
  #define dprintf(x...) 
#endif

static uint16_t myIP_read(void *opaque, hwaddr offset)
{
    myIP_state *s = (myIP_state *)opaque;
  // tb_sobelIP_state *s = (tb_sobelIP_state *)opaque;
	int offset_sh=offset >> 2;

	return 0;
}

static void myIP_write(void *opaque, hwaddr offset,
                          uint32_t value)
{
    myIP_state *s = (myIP_state *)opaque;
    int offset_sh=offset >> 2;

	printf("tb_soble_writing \n");
  
  s_obj.clk(clk);
    s_obj.rst(rst);
 
 
 
    for(i=0;i< 3; i++){
        s_obj.input_row[i](input_row[i]);
    }
 
    s_obj.output_row(output_row);

  tb_obj.clk(clk);
  tb_obj.rst(rst);

    for(i=0;i< 3; i++){
    tb_obj.input_row[i](input_row[i]);
    }

  tb_obj.output_row(output_row);




#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");

  // Top level signals
  sc_trace(trace_file, clk              , "clk");
  sc_trace(trace_file, rst              , "rst");
 
 for(i=0; i < SIZE_BUFFER; i++){
   sprintf(name, "input_row%d",i);   
   sc_trace(trace_file, input_row[i]     , name);
 }
 
 sc_trace(trace_file, output_row         , "output_row");
  

#endif  // End WAVE_DUMP

 
   sc_start(25, SC_NS );
   rst.write(0);
  
   sc_start(25, SC_NS );
   rst.write(1);


   sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif


	printf("tb_soble_writed \n");
    
}

static const MemoryRegionOps myIP_ops = {
    myIP_read,
    myIP_write,
    NULL,NULL,
    DEVICE_NATIVE_ENDIAN,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static int myIP_init(Object *obj)
{
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    myIP_state *s = myIP(obj);
    memory_region_init_io(&s->iomem, OBJECT(s), &myIP_ops, s, "myIP", 0x10000);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq); 
   // myIP = new  tb_sobel("myIP");
//tb_sobelIP = new tb_sobel("tb_sobelIP");
    return 0;
}

static int myIP_init_arm(ObjectClass *klass, void *data)
{
    DeviceClass *k = DEVICE_CLASS(klass);
    
}

static TypeInfo myIP_info = {
    "myIP",
    TYPE_SYS_BUS_DEVICE,
    sizeof(myIP_state),
    myIP_init,
    NULL,NULL,NULL,NULL,
    myIP_init_arm,
    NULL,NULL,NULL,NULL,
//myIP_init,
};


static void myIP_register_types(void)
{
     type_register_static(&myIP_info);
}
void sc_start_X_cycles(uint64_t cycles){
sc_start((int)cycles,SC_NS);
}
type_init(myIP_register_types)
}

