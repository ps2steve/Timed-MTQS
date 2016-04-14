/* 
* ARM custom IP interface with Sobel
 * Copyright (c) 2011-2012 NSYSU ESLAB
 * Written by CY Hsu
 *
 * This code is licenced under the GPL.
 */
 #include "sobel.h"

 #define DRIVER_NAME "sobelIP"
 #define TYPE_myIP "sobelIP"
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
 }sobel_state;

#define	QEMU_DEBUG_RW	1
#if QEMU_DEBUG_RW
  #define dprintf(x...) printf(DRIVER_NAME ": " x)
#else
  #define dprintf(x...) 
#endif

static uint8_t sobelIP_read(void *opaque, hwaddr offset)
{
	sobel_state *s = (sobel_state *)opaque;
	int offset_sh=offset >>2;
	uint8_t output_row;
		output_row= s->reg[0];
	return output_row;
}

static uint8_t sobelIP_write(void *opaque, hwaddr offset, uint8_t given_value)
{
	sobel_state *s = (sobel_state *)opaque;
	int offset_sh= offset >>2 ;
	if (offset_sh == 0)
	{
		sobelIP->input_row[SIZE_BUFFER]= given_value;
		sobelIP->flag_1.notify(0, SC_NS);
		s-> reg[offset_sh]=sobelIP->output_row;
	}
}
static const MemoryRegion sobelIP_ops = {
	sobelIP_read,
	sobelIP_write,
	NULL,NULL,
	DEVICE_NATIVE_ENDIAN,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static int sobelIP_init(Object *obj)
{
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);
    sobel_state *s = sobelIP(obj);
    memory_region_init_io(&s->iomem, OBJECT(s), &myIP_ops, s, "sobelIP", 0x10000);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq); 
    sobelIP = new SC_myIP("sobelIP");
    return 0;
}

static int sobelIP_init_arm(Object *obj)
{
	DeviceClass *k = DEVICE_CLAS(klass);
}

static TypeInfo sobleIP_info = {
	"sobelIP",
	TYPE_SYS_BUS_DEVICE,
	 sizeof(sobelIP_state),
    sobelIP_init,
    NULL,NULL,NULL,NULL,
    sobelIP_init_arm,
    NULL,NULL,NULL,NULL,
};

static void sobelIP_register_types(void)
{
	type_register_static(&sobleIP_info);
}
type_init(sobelIP_register_types)
}