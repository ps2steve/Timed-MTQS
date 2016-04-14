/* 
* ARM custom IP interface with Sobel
 * Copyright (c) 2011-2012 NSYSU ESLAB
 * Written by CY Hsu
 *
 * This code is licenced under the GPL.
 */
 //#include "sobel.h"
 #include "tb_sobel.h"
 #include "sobel.h"
 #include  "stdio.h"
// #include "define.h"
 #define DRIVER_NAME "tb_sobelIP"
 #define TYPE_tb_sobelIP "tb_sobelIP"
 #define tb_sobelIP(obj) OBJECT_CHECK(tb_sobelIP_state, (obj), TYPE_tb_sobelIP)

//systemC defined
 sobel s_obj("sobel");
  tb_sobel tb_obj("tb_sobel");
 sc_clock                clk("clk", 25, SC_NS, 0.5, 0, SC_NS, true);
  sc_signal<bool> rst;

  sc_signal<sc_uint<8> > input_row[3];
  sc_signal<sc_uint<8> > output_row;
  int i;
  char name[15];


 extern "C"{
 #include "hw/sysbus.h"
 #include "qemu/timer.h"
 

 typedef struct{
 	SysBusDevice busdev;
 	qemu_irq irq;
 	sc_uint<8> reg[4];
 	MemoryRegion iomem;
 }tb_sobelIP_state;

#define	QEMU_DEBUG_RW	1
#if QEMU_DEBUG_RW
  #define dprintf(x...) printf(DRIVER_NAME ": " x)
#else
  #define dprintf(x...) 
#endif

static uint8_t tb_sobelIP_read(void *opaque, hwaddr offset)
{
	tb_sobelIP_state *s = (tb_sobelIP_state *)opaque;
	
  return 0;
}

static void tb_sobelIP_write(void *opaque, hwaddr offset, uint8_t given_value)
{
	tb_sobelIP_state *s = (tb_sobelIP_state *)opaque;
	
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
		cout << " write finished"<<endl;
	
}

static const MemoryRegionOps tb_sobelIP_ops = {
	tb_sobelIP_read,
	tb_sobelIP_write,
	NULL,
	NULL,
	DEVICE_NATIVE_ENDIAN,
 	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static int tb_sobelIP_init(Object *obj)
{
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    tb_sobelIP_state *s = tb_sobelIP(obj);
    memory_region_init_io(&s->iomem, OBJECT(s), &tb_sobelIP_ops, s, "tb_sobelIP", 0x10001);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq); 
    return 0;
}

static int tb_sobelIP_init_arm(Object *klass, void *data)
{
	DeviceClass *k = DEVICE_CLASS(klass);
}

static TypeInfo tb_sobelIP_info = {
	"tb_sobelIP",
	TYPE_SYS_BUS_DEVICE,
	sizeof(tb_sobelIP_state),
    tb_sobelIP_init,
    NULL,NULL,NULL,NULL,
    tb_sobelIP_init_arm,
    NULL,NULL,NULL,NULL,
};

static void tb_sobelIP_register_types(void)
{
	type_register_static(&tb_sobelIP_info);
}
type_init(tb_sobelIP_register_types)
}
