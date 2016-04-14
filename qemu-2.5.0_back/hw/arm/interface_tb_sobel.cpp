/* 
* ARM custom IP interface with Sobel
 * Copyright (c) 2011-2012 NSYSU ESLAB
 * Written by CY Hsu
 *
 * This code is licenced under the GPL.
 */
 #include "sobel.h"
 #include "tb_sobel.cpp"

 #define DRIVER_NAME "tb_sobelIP"
 #define TYPE_myIP "tb_sobelIP"
 #define sobel(obj) OBJECT_CHECK(sobel_state, (obj), TYPE_sobel)

 soble * sobelIP;

 extern "C"{
 #include "hw/sysbus.h"
 #include "qemu/timer.h"
 

 typedef struct{
 	SysBusDevice busdev;
 	qemu_irq irq;
 	int reg[4];
 	MemoryRegion iomem;
 }tb_sobel_state;

#define	QEMU_DEBUG_RW	1
#if QEMU_DEBUG_RW
  #define dprintf(x...) printf(DRIVER_NAME ": " x)
#else
  #define dprintf(x...) 
#endif

static uint8_t tb_sobelIP_read(void *opaque, hwaddr offset)
{
	tb_sobel_state *s = (tb_sobel_state *)opaque;
	int offset_sh=offset >>2;
	uint8_t output_row;
		output_row= s->reg[0];
	return output_row;
}

static uint8_t tb_sobelIP_write(void *opaque, hwaddr offset, uint8_t given_value)
{
	tb_sobel_state *s = (tb_sobel_state *)opaque;
	int offset_sh= offset >>2 ;
	if (offset_sh == 0)
	{
		tb_sobelIP->input_row[SIZE_BUFFER]= given_value;
		tb_sobelIP->flag_1.notify(0, SC_NS);
		s-> reg[offset_sh]=tb_sobelIP->output_row;
	}
}
static const MemoryRegion tb_sobelIP_ops = {
	tb_sobelIP_read,
	tb_sobelIP_write,
	NULL,NULL,
	DEVICE_NATIVE_ENDIAN,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static int tb_sobelIP_init(Object *obj)
{
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    tb_sobel_state *s = tb_sobelIP(obj);
    memory_region_init_io(&s->iomem, OBJECT(s), &tb_soble_ops, s, "tb_sobelIP", 0x10000);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq); 
    tb_sobelIP = new tb_sobel("tb_sobelIP");
    return 0;
}

static int tb_sobelIP_init_arm(Object *obj)
{
	DeviceClass *k = DEVICE_CLAS(klass);
}

static TypeInfo tb_sobleIP_info = {
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
	type_register_static(&tb_sobleIP_info);
}
type_init(tb_sobelIP_register_types)
}