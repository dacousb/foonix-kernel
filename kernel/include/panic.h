#ifndef __PANIC_H__
#define __PANIC_H__

#include <arch/x86_64/asm.h>
#include <printf.h>

#define panic(...)                                                            \
    {                                                                         \
        printf("------------------------------------------------\n");         \
        printf("PANIC (%s:%d `%s`) (message=", __FILE__, __LINE__, __func__); \
        printf(__VA_ARGS__);                                                  \
        printf(")\n------------------------------------------------\n");      \
        __hlt_for__();                                                        \
    }

#define TODO panic("TODO")

#endif