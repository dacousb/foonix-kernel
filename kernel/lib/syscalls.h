#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include <lib/types.h>

u64 dispatch_syscall(u64 syscall, u64 a, u64 b, u64 c, u64 d, u64 e, u64 f);

#endif
