/*
 * Arm custom IP Interface
 *
 * Copyright (c) 2011-2012 NSYSU ESLAB
 * Written by GK mike
 *
 * This code is licenced under the GPL.
 */
#include "SC_myIP.h"

#define DRIVER_NAME "myIP"
#define TYPE_myIP "myIP"
#define myIP(obj) OBJECT_CHECK(myIP_state, (obj), TYPE_myIP)
SC_myIP *myIP;
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
    int offset_sh=offset >> 2;
    uint16_t tmp;
           tmp = s->reg[0];
    return tmp;
}

static void myIP_write(void *opaque, hwaddr offset,
                          uint32_t value)
{
    myIP_state *s = (myIP_state *)opaque;
    int offset_sh=offset >> 2;

    if(offset_sh == 0)
    {
    myIP->tmp_32 = value;
    myIP->flag_1.notify(0,SC_NS);
    sc_start(100,SC_NS);
    s->reg[offset_sh]=myIP->tmp_16;
    }
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
    myIP = new SC_myIP("myIP");
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

