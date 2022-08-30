#ifndef __PANIC_H__
#define __PANIC_H__

#include <arch/x86_64/asm.h>
#include <lib/printf.h>

#define panic(reason)                                                 \
    {                                                                 \
        printf("------------------------------------------------\n"); \
        printf("PANIC (%s:%d `%s`)\n", __FILE__, __LINE__, __func__); \
        printf("      `%s`\n", reason);                               \
        printf("------------------------------------------------\n"); \
        __hlt_for__();                                                \
    }

#define assert(condition)                                                            \
    {                                                                                \
        if (!(condition))                                                            \
        {                                                                            \
            printf("------------------------------------------------\n");            \
            printf("FAILED ASSERTION (%s:%d `%s`)\n", __FILE__, __LINE__, __func__); \
            printf("                 `" #condition "`\n");                           \
            printf("------------------------------------------------\n");            \
            __hlt_for__();                                                           \
        }                                                                            \
    }

#define TODO panic("TODO")

#endif
