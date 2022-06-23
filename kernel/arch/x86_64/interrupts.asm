[bits 64]

extern interrupt_handler

%macro push_regs 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro pop_regs 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

%macro interrupt_err 1
interrupt%1:
    push qword %1 ; int_no
    jmp interrupt_finish
%endmacro

%macro interrupt_no_err 1
interrupt%1:
    push qword 0  ; err_code
    push qword %1 ; int_no
    jmp interrupt_finish
%endmacro

interrupt_finish:
    push_regs

    mov rdi, rsp ; pass as argument
    call interrupt_handler
    mov rsp, rax ; get the return value

    pop_regs
    add rsp, 8 ; pop int_no
    add rsp, 8 ; pop err_code

    iretq

interrupt_no_err 0
interrupt_no_err 1
interrupt_no_err 2
interrupt_no_err 3
interrupt_no_err 4
interrupt_no_err 5
interrupt_no_err 6
interrupt_no_err 7
interrupt_err    8
interrupt_no_err 9
interrupt_err    10
interrupt_err    11
interrupt_err    12
interrupt_err    13
interrupt_err    14
interrupt_no_err 15
interrupt_no_err 16
interrupt_err    17
interrupt_no_err 18
interrupt_no_err 19
interrupt_no_err 20
interrupt_no_err 21
interrupt_no_err 22
interrupt_no_err 23
interrupt_no_err 24
interrupt_no_err 25
interrupt_no_err 26
interrupt_no_err 27
interrupt_no_err 28
interrupt_no_err 29
interrupt_err    30
interrupt_no_err 31

%assign n 32
%rep 256-32
    interrupt_no_err n
%assign n n+1
%endrep

section .data
global interrupts

interrupts:
    %assign n 0
    %rep 256
        dq interrupt%+n
        %assign n n+1
    %endrep