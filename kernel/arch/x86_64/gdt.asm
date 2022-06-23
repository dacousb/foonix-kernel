[bits 64]

global reload_gdt
reload_gdt:
    lgdt [rdi] ; load the gdtr

    mov ax, 8+8 ; reload segment registers
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    pop rdi
    mov rax, 8
    push rax
    push rdi
    
    retfq