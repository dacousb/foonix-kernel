#include "request.h"

#include <arch/x86_64/acpi.h>
#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/hpet.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/syscall.h>
#include <lib/fb.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <lib/types.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <tasking/scheduler.h>

void kernel_hello()
{
    printf("     Hello\n");
    __hlt_for__();
}

void kernel_world()
{
    printf("     world!\n");
    __hlt_for__();
}

void _start(void)
{
    if (framebuffer_request.response == nil ||
        framebuffer_request.response->framebuffer_count < 1)
        __hlt_for__();
    init_fb(framebuffer_request.response->framebuffers[0]);

    assert(rsdp_request.response != nil);
    assert(memmap_request.response != nil);
    assert(kaddr_request.response != nil);

    printf("  __                   _      \n");
    printf(" / _| ___   ___  _ __ (_)_  __\n");
    printf("| |_ / _ \\ / _ \\| '_ \\| \\ \\/ /\n");
    printf("|  _| (_) | (_) | | | | |>  < \n");
    printf("|_|  \\___/ \\___/|_| |_|_/_/\\_\\\n\n");

    init_gdt();
    init_idt();

    acpi_t acpi = init_acpi((u64)rsdp_request.response->address);
    init_hpet(acpi);
    init_apic(acpi);

    init_pmm(memmap_request.response);
    init_vmm(kaddr_request.response, memmap_request.response);

    i32 task = new_kernel_task((u64)kernel_hello);
    i32 task_ = new_kernel_task((u64)kernel_world);
    set_task_status(task, TASK_ACTIVE);
    set_task_status(task_, TASK_ACTIVE);

    init_syscall();
    init_scheduler();

    __hlt_for__();
}
