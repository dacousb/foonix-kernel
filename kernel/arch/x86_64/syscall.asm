[bits 64]

extern syscall_handler

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

%macro pop_regs_syscall 0
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
%endmacro

global syscall_entry
syscall_entry:
    swapgs
    mov [gs:0x8], rsp ; save syscall_user_stack
    mov rsp, [gs:0x0] ; load syscall_kernel_stack

    push qword 3*8 | 3  ; push user data
    push qword [gs:0x8] ; push syscall_user_stack

    push r11           ; push rflags (saved by syscall)
    push qword 4*8 | 3 ; push user code
    push rcx           ; push rip (saved by syscall)

    push qword 0 ; push err_code
    push qword 0 ; push int_no

    push_regs

    mov rdi, rsp ; pass as argument
    call syscall_handler
    
    pop_regs_syscall ; rax keeps its value

    mov rsp, [gs:0x8] ; load syscall_user_stack

    swapgs
    o64 sysret ; set the operand size to 64 bits and return
