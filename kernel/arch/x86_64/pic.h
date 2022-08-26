#ifndef __PIC_H__
#define __PIC_H__

#include <arch/x86_64/asm.h>

#define PIC1 0x20
#define PIC2 0xa0

#define PIC1_DATA (PIC1 + 1)
#define PIC2_DATA (PIC2 + 1)

static inline void disable_pic()
{
    out8(PIC1_DATA, 0xff);
    out8(PIC2_DATA, 0xff);
}

#endif