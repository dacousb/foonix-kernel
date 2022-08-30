#include <arch/x86_64/asm.h>
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/syscall.h>
#include <lib/printf.h>
#include <lib/syscalls.h>

void init_syscall()
{
    __wrmsr__(MSR_EFER, __rdmsr__(MSR_EFER) | 1);                         // enable system call extensions
    __wrmsr__(MSR_STAR, ((u64)(2 * 8 | 3) << 48) | ((u64)(1 * 8) << 32)); // set sysret/syscall selectors
    __wrmsr__(MSR_LSTAR, (u64)syscall_entry);

    printf("[OK] init syscalls\n");
}

u64 syscall_handler(regs_t *regs)
{
    u64 a = regs->general_registers.rdi;
    u64 b = regs->general_registers.rsi;
    u64 c = regs->general_registers.rdx;
    u64 d = regs->control_registers.r10;
    u64 e = regs->control_registers.r8;
    u64 f = regs->control_registers.r9;

    return dispatch_syscall(regs->general_registers.rax, a, b, c, d, e, f);
}
