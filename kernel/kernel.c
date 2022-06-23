#include "request.h"

#include <arch/x86_64/acpi.h>
#include <arch/x86_64/apic.h>
#include <arch/x86_64/asm.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <fb.h>
#include <mem/pmm.h>
#include <panic.h>
#include <printf.h>
#include <types.h>

void _start(void)
{
    if (framebuffer_request.response == nil ||
        framebuffer_request.response->framebuffer_count < 1)
        __hlt_for__();
    if (rsdp_request.response == nil ||
        rsdp_request.response->address == nil)
        panic("rsdp request failed");
    if (memmap_request.response == nil ||
        memmap_request.response->entry_count < 1)
        panic("memmap request failed");

    init_fb(framebuffer_request.response->framebuffers[0]);
    printf("  __                   _      \n");
    printf(" / _| ___   ___  _ __ (_)_  __\n");
    printf("| |_ / _ \\ / _ \\| '_ \\| \\ \\/ /\n");
    printf("|  _| (_) | (_) | | | | |>  < \n");
    printf("|_|  \\___/ \\___/|_| |_|_/_/\\_\\\n\n");

    init_gdt();
    init_idt();

    acpi_t acpi = init_acpi((u64)rsdp_request.response->address);
    init_apic(acpi);

    init_pmm(memmap_request.response);

    __hlt_for__();
}
