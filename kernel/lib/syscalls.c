#include <lib/printf.h>
#include <lib/syscalls.h>

u64 dispatch_syscall(u64 syscall, u64 a, u64 b, u64 c, u64 d, u64 e, u64 f)
{
    (void)d;
    (void)e;
    (void)f;

    printf("[SYSCALL] %lx (%lx %lx %lx)\n", syscall, a, b, c);
    return 0;
}
