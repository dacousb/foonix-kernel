#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <lib/types.h>

typedef struct
{
    struct
    {
        u64 r15;
        u64 r14;
        u64 r13;
        u64 r12;
        u64 r11;
        u64 r10;
        u64 r9;
        u64 r8;
    } __attribute__((packed)) control_registers;

    struct
    {
        u64 rbp;
        u64 rdi;
        u64 rsi;
        u64 rdx;
        u64 rcx;
        u64 rbx;
        u64 rax;
    } __attribute__((packed)) general_registers;

    struct
    {
        u64 int_no;
        u64 err_code;

        u64 rip;
        u64 cs;
        u64 rflags;
        u64 rsp;
        u64 ss;
    } __attribute__((packed)) frame_registers;
} __attribute__((packed)) regs_t;

#endif
