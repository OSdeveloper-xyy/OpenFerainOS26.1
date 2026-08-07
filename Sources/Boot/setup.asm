bits 16;实模式
org 0x7C00;0x7c00引导扇区的内存地址
section .text
    global _start
_start:
    mov ax,0x1000;设置堆栈段地址
    mov ss,ax
    mov sp,0x0000;设置堆栈顶地址
    mov ax,0x0003;设置为80x25文本模式
    int 0x10

    mov ax,0x0000;初始化段寄存器
    mov ds,ax
    mov es,ax
    call _move
    call _A20_init
    call _gdt_init
    cli
    mov eax, cr0
    or eax,0x00000001
    mov cr0,eax
    jmp 0x8:0x8000
_move:
    mov dx,0x0080
    mov cx,0x0002
    mov bx,0x8000
    mov ax,0x0220
    int 0x13
    ret
_A20_init:
    in al,0x92
    or al,0x02
    out 0x92,al
    ret
_gdt_init: 
    ;NULL
    mov dword [0x6000],0x00000000
    mov dword [0x6004],0x00000000
    ;CODE
    mov dword [0x6008],0x0000FFFF
    mov dword [0x600C],0x00CF9A00
    ;DATA
    mov dword [0x6010],0x0000FFFF
    mov dword [0x6014],0x00CF9200
    ;NULL
    mov dword [0x6018],0x00000000
    mov dword [0x601C],0x00000000
    lgdt [gdtr_inf]
    ret
gdtr_inf:
    dw 0x03FF
    dd 0x00006000
msg1 db 'Loaded error!'
times 0x1BE - ($ - $$) db 0
db 0x00,0x20,0x21,0x00,0x0C,0x5E,0x38,0x26,0x00,0x08,0x00,0x00,0x00,0x60,0x09,0x00;分区1,FAT32
times 510 - ($ - $$) db 0
dw 0xAA55;引导扇区标志