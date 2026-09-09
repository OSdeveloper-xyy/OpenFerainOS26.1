.code32
# Part of move Code
.section .entry,"ax"
.global init_setup
init_setup:
    # Set up the stack and segment registers
    movw $0x10,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%ss
    movl $0x003FFFF0,%esp
    # Clear the 1M to 4M section
    movl $0x00100000,%edi
    movl $0x00000000,%eax
    movl $0x000C0000,%ecx
    cld
    rep stosl
    # Copy the init and ISR code to 1MB
    movl $0x00008000,%esi
    movl $0x00100000,%edi
    movl $0x00001000,%ecx
    rep movsl
    # jmp to the init code at 1MB
    push $clear_section
    ret
clear_section:
    # Clear the Low 640KB section
    movl $0x00000000,%edi
    movl $0x00000000,%eax
    movl $0x00010000,%ecx
    cld
    rep stosl
    movl $0x00048000,%edi
    movl $0x00000000,%eax
    movl $0x00012000,%ecx
    cld
    rep stosl
    # jmp to init main
    push $init_main
    ret
# Part of ISR
.section .text
.global default_ISR
.global DE_ISR
.global SS_ISR
.global GP_ISR
.global PS2_ISR
.global SERVER_ISR
.global VIDEO_ISR
.global DEVICE_ISR
default_ISR:
    pusha
    popa
    iret
DE_ISR:
    pusha
    movl 0x20(%esp),%ebx
    call _DE_fix
    movl %ebx,0x20(%esp)
    popa
    iret
SS_ISR:
    pusha
    popa
    addl $4,%esp
    iret
GP_ISR:
    pusha
    popa
    addl $4,%esp
    iret
PS2_ISR:
    pusha
    pushl %edi
    call ISR_FOR_PS2
    addl $4,%esp
    movb $0x20,%al
    movw $0x20,%dx
    outb %al,%dx
    popa
    iret
SERVER_ISR:
    pusha
    push %ds
    push %es
    movw $0x10,%bx
    movw %bx,%ds
    movw %bx,%es
    call _SERVER_ISR_CMP
    push %es
    pop %ds
    popa
    iret
VIDEO_ISR:
    pusha
    push %ds
    push %es

    movw $0x10,%bx
    movw %bx,%ds
    movw %bx,%es
    call _VIDEO_ISR_CMP

    pop %es
    pop %ds
    popa
    iret
DEVICE_ISR:
    pusha
    push %ds
    push %es

    movw $0x10,%bx
    movw %bx,%ds
    movw %bx,%es
    call _DEVICE_ISR_CMP
    pop %es
    pop %ds
    popa
    iret
_DE_fix:
    movb (%ebx),%al
    cmpb $0x66,%al
    je _op_prefix
    incl %ebx
    movb (%ebx),%al
    shrb $6,%al
    cmpb $0x00,%al
    je _mod0
    cmpb $0x01,%al
    je _mod1
    cmpb $0x02,%al
    je _mod2
    cmpb $0x03,%al
    je _mod3
    ret
_op_prefix:
    incl %ebx
    jmp _DE_fix
_mod0:
    movb (%ebx),%al
    andb $0x07,%al
    cmpb $0x05,%al
    je _mod2
    cmpb $0x04,%al
    je _sib
    incl %ebx
    ret
_sib:
    incl %ebx
    movb (%ebx),%al
    cmpb $0x05, %al 
    je _mod2
    incl %ebx
    ret
_mod1:
    addl $2,%ebx
    ret
_mod2:
    addl $5,%ebx
    ret
_mod3:
    incl %ebx
    ret
_VIDEO_ISR_CMP:
    cmpb $0x00,%ah
    je _print_string
    cmpb $0x80,%ah
    je _set_cursor
    cmpb $0x81,%ah
    je _scolling
_print_string:
    pushl %eax
    pushl %ecx
    pushl %edi
    pushl %esi
    call video_isr_print_string
    addl $16,%esp
    ret
_set_cursor:
    pushl %edi
    pushl %esi
    call cursor_set
    addl $8,%esp
    ret
_scolling:
    call vm_scolling
    ret
_SERVER_ISR_CMP:
    ret
_DEVICE_ISR_CMP:
    ret
