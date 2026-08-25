bits 16
org 0x5000
_asm_main:
    call _kernel_move
    call _A20
    call _gdt_init
    call _idt_init
    mov eax, cr0
    or eax,0x00000001
    mov cr0,eax
    jmp 0x8:0x8000
_kernel_move:
    mov ax,0x1000
    mov es,ax
    mov dx,0x0080
    mov cx,0x0012
    mov bx,0x2000
    mov ax,0x0220
    int 0x13
    mov ax,0x0000
    mov es,ax
    mov dx,0x0080
    mov cx,0x000A
    mov bx,0x8000
    mov ax,0x0208
    int 0x13
    ret
_A20:
    in al,0x92
    or al,2
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
    ;STACK RING0
    mov dword [0x6018],0x0000FFFF
    mov dword [0x601C],0x3F4092FF
    ;VGA
    mov dword [0x6020],0x80000F9F
    mov dword [0x6024],0x0040920B
    ;STACK RING3
    mov dword [0x6028],0x0000FFFF
    mov dword [0x602C],0x3F40F2FE
    ;LDT
    mov dword [0x6030],0x1C0003FF
    mov dword [0x6034],0x00008200
    lgdt [gdtr_inf]
    ret
_idt_init:
    cli
    call _8529A
    mov di,0x6800
    mov ax,_default_int
    mov cx,128
    call _set_idt

    mov di,0x6900
    mov ax,_m_irq_int
    mov cx,8
    call _set_idt

    mov di,0x6940
    mov ax,_s_irq_int
    mov cx,8
    call _set_idt

    mov ax,_NM
    mov word [0x6838],ax
    mov word [0x683A],0x0008
    mov dword [0x683C],0x00008E00

    mov ax,_SS
    mov word [0x6860],ax
    mov word [0x6862],0x0008
    mov dword [0x6864],0x00008E00

    mov ax,_GP
    mov word [0x6868],ax
    mov word [0x686A],0x0008
    mov dword [0x686C],0x00008E00

    ;pit
    mov word [0x6900],0x0000
    mov word [0x6902],0x0040    ;tss
    mov dword [0x6904],0x0000E500    ;task gate

    mov ax,_KEYBOARD
    mov word [0x6908],ax
    mov word [0x690A],0x0008
    mov dword [0x690C],0x0000EE00

    mov ax,_RTC
    mov word [0x6940],ax
    mov word [0x6942],0x0008
    mov dword [0x6944],0x0000EE00

    mov ax,_VIDEO
    mov word [0x6980],ax
    mov word [0x6982],0x0008
    mov dword [0x6984],0x0000EE00

    mov ax,_SERVER
    mov word [0x6988],ax
    mov word [0x698A],0x0008
    mov dword [0x698C],0x0000EE00

    mov word [0x2020],0x6A00

    lidt [idtr_inf]
    ret
_8529A:
    mov al,0x11
    out 0x20,al
    mov al,0x20
    out 0x21,al
    mov al,0x04
    out 0x21,al
    mov al,0x01
    out 0x21,al
    mov al,0xFF
    out 0x21,al

    mov al,0x11
    out 0xA0,al
    mov al,0x28
    out 0xA1,al
    mov al,0x02
    out 0xA1,al
    mov al,0x01
    out 0xA1,al
    mov al,0xFF
    out 0xA1,al
    ret
_set_idt:
    mov word [di],ax
    add di,2
    mov word [di],0x0008
    add di,2
    mov dword [di],0x00008E00
    add di,4
    loop _set_idt
    ret

bits 32

_SERVER:
    cmp bh,0x00  ;load base
    je _LOAD_BASE
    cmp bh,0x01  ;creat task
    je _creat_task
_LOAD_BASE:
    mov eax,0x00000000
    mov ax,ds

    push ds
    mov bx,0x10
    mov ds,bx

    and ax,0xF8  ;Discard low 3 bit
    shr eax,4  ;task n(index 1)
    sub eax,1
    shl eax,2
    add eax,0x10000
    mov edi,eax
    mov dword eax,[edi]
    pop ds
    iret
_creat_task:
    pushad
    mov dword ebx,[0x2028]  ;save base table bp
    mov dword [ebx],eax ;save base
    add ebx,4
    mov dword [0x2028],ebx


    mov word bx,[0x2000]
    mov word [bx],0x0067
    add bx,2
    mov word dx,[0x2002]
    mov word [bx],dx
    add bx,2
    mov dword [bx],0x00008900
    add bx,4
    mov word [0x2000],bx
    add dx,0x80
    mov word [0x2002],dx

    call _ldt_creat
    call _tss_creat
    mov word bx,[0x2020]
    mov word [bx],0x0000
    add bx,2
    mov word dx,[0x2000]
    sub dx,0x6008
    mov word [bx],dx
    add bx,2
    mov dword [bx],0x0000E500
    add bx,4
    mov word [0x2020],bx
    popad
    iret
_tss_creat:
    mov word bx,[0x2002]
    sub bx,0x80
    mov dx,bx
    mov word [bx],0x0038 ;backtss
    mov word [bx+2],0x0000
    mov dword [bx+4],0x0000FFF0 ;esp0
    mov word [bx+8],0x0018 ;ss0
    mov word [bx+10],0x0000
    mov dword [bx+12],0x00000000 ;esp1
    mov word [bx+16],0x0000  ;ss1
    mov word [bx+18],0x0000
    mov dword [bx+20],0x00000000 ;esp2
    mov word [bx+24],0x0000  ;ss2
    mov word [bx+26],0x0000
    mov dword [bx+28],0x00000000 ;cr3
    mov dword [bx+32],0x00000000 ;eip
    mov dword [bx+36],0x00000202 ;eflags
    mov dword [bx+40],0x00000000 ;eax
    mov dword [bx+44],0x00000000 ;ecx
    mov dword [bx+48],0x00000000 ;edx
    mov dword [bx+52],0x00000000 ;ebx
    mov dword [bx+56],0x0000FFF0 ;esp
    mov dword [bx+60],0x00000000 ;ebp
    mov dword [bx+64],0x00000000 ;esi
    mov dword [bx+68],0x00000000 ;edi
    add cx,8
    mov word [bx+72],cx ;es
    mov word [bx+74],0x0000
    sub cx,8
    mov word [bx+76],cx ;cs
    mov word [bx+78],0x0000
    mov word [bx+80],0x002B ;ss
    mov word [bx+82],0x0000
    add cx,8
    mov word [bx+84],cx ;ds
    mov word [bx+86],0x0000
    mov word [bx+88],0x0000 ;fs
    mov word [bx+90],0x0000
    mov word [bx+92],0x0000 ;gs
    mov word [bx+94],0x0000
    mov word [bx+96],0x0030 ;ldt
    mov word [bx+98],0x0000
    mov word [bx+100],0x0000
    mov word [bx+102],0x0068 ;i/o_addr

    ret
_ldt_creat:
    mov word bx,[0x2010]
    mov cx,bx
    sub cx,0x1C00
    or cx,0x7 ;ti=1,dpl=3
    mov edx,eax
    shr edx,16

    sub esi,1
    mov word [bx],si
    add bx,2
    mov word [bx],ax
    add bx,2
    mov byte [bx],dl
    add bx,1
    mov byte [bx],0xFA
    add bx,1
    mov byte [bx],0x40
    add bx,1
    mov byte [bx],dh
    add bx,1

    sub edi,1
    mov word [bx],di
    add bx,2
    mov word [bx],ax
    add bx,2
    mov byte [bx],dl
    add bx,1
    mov byte [bx],0xF2
    add bx,1
    mov byte [bx],0x40
    add bx,1
    mov byte [bx],dh
    add bx,1

    mov word [0x2010],bx
    ret
_RTC:
    push ds
    mov bx,0x10
    mov ds,bx

    mov al,0x0C 
    out 0x70,al 
    in  al,0x71
    test al,0x10
    jnz _read_time
    jmp _rtc_exit

_read_time:
    mov al,0x0A
    out 0x70,al
    nop
    nop
    in al,0x71
    test al,0x80
    jnz _read_time

    mov byte [0x4000],0x32
    mov byte [0x4001],0x30

    nop
    nop
    mov al,0x09
    out 0x70,al
    in al,0x71
    mov bl,al
    shr bl,4
    shl bl,4
    sub al,bl
    shr bl,4
    add bl,0x30
    mov byte [0x4002],bl
    add al,0x30
    mov byte [0x4003],al

    mov byte [0x4004],0x2f

    nop
    nop
    mov al,0x08
    out 0x70,al
    in al,0x71
    mov bl,al
    shr bl,4
    shl bl,4
    sub al,bl
    shr bl,4
    add bl,0x30
    mov byte [0x4005],bl
    add al,0x30
    mov byte [0x4006],al

    mov byte [0x4007],0x2f

    nop
    nop
    mov al,0x07
    out 0x70,al
    in al,0x71
    mov bl,al
    shr bl,4
    shl bl,4
    sub al,bl
    shr bl,4
    add bl,0x30
    mov byte [0x4008],bl
    add al,0x30
    mov byte [0x4009],al

    mov byte [0x400A],0x20

    nop
    nop
    mov al,0x04
    out 0x70,al
    in al,0x71
    mov bl,al
    shr bl,4
    shl bl,4
    sub al,bl
    shr bl,4
    add bl,0x30
    mov byte [0x400B],bl
    add al,0x30
    mov byte [0x400C],al

    mov byte [0x400D],0x3a

    nop
    nop
    mov al,0x02
    out 0x70,al
    in al,0x71
    mov bl,al
    shr bl,4
    shl bl,4
    sub al,bl
    shr bl,4
    add bl,0x30
    mov byte [0x400E],bl
    add al,0x30
    mov byte [0x400F],al

    mov byte [0x4010],0x3a

    nop
    nop
    mov al,0x00
    out 0x70,al
    in al,0x71
    mov bl,al
    shr bl,4
    shl bl,4
    sub al,bl
    shr bl,4
    add bl,0x30
    mov byte [0x4011],bl
    add al,0x30
    mov byte [0x4012],al

    mov edi,[0x2008]
    push edi
    mov ax,0x0007
    mov esi,0x4000
    mov edi,0x183C
    mov ecx,19
    int 0x30
    pop edi
    mov [0x2008],edi
    jmp _rtc_exit

_rtc_exit:
    pop ds

    mov al,0x20 
    out 0xA0,al
    out 0x20,al
    iret
_m_irq_int:
    mov al,0x20
    out 0x20,al
    iret
_s_irq_int:
    mov al,0x20 
    out 0xA0,al
    out 0x20,al
    iret
_default_int:
    iret
_NM:
    clts
    iret
_GP:
    pop eax  ;GP error code
    iret
_SS:
    pop eax  ;SS error code
    iret
_VIDEO:
    push ds
    mov bx,0x10
    mov ds,bx

    cmp ah,0x00
    je _printf
    cmp ah,0x01
    je _one_char
    cmp ah,0x02
    je _scrolling
    cmp ah,0x03
    je _clean_char
    cmp ah,0x04
    je _clean_line
    cmp ah,0x05
    je _set_color
    cmp ah,0x80
    je _set_cursor
_set_color:
    mov ecx,2000
    mov ebx,0xB8001
    _set_color_loop:
        mov [ebx],al
        add ebx,2
        loop _set_color_loop
    pop ds
    iret
_clean_char:
    mov edi,[0x2008]
    sub edi,2
    mov [0x2008],edi
    mov byte [edi],0x20
    add edi,1
    mov byte [edi],al
    pop ds
    iret
_scrolling:
    push es
    mov bx,0x10
    mov es,bx
    mov esi,0xB80A0
    mov edi,0xB8000
    mov cx,0x370
    rep movsd
    mov edi,[0x2008]
    sub edi,0xA0
    mov [0x2008],edi
    pop es
    pop ds
    iret
_clean_line:
    push es
    mov bx,0x10
    mov es,bx

    imul edi,0xA0
    add edi,0xB8000
    mov ecx,80
    _clean_line_loop:
        mov byte [edi],0x00
        add edi,1
        mov byte [edi],al
        add edi,1
        loop _clean_line_loop
    rep stosw

    pop es
    pop ds
    iret
_set_cursor:
    mov edi,[0x2008]
    mov ebx,edi
    sub ebx,0xB8000
    shr ebx,1

    mov dx,0x3D4
    mov al,0x0E
    out dx,al
    mov dx,0x3D5
    mov al,bh
    out dx,al

    mov dx,0x3D4
    mov al,0x0F
    out dx,al
    mov dx,0x3D5
    mov al,bl
    out dx,al

    pop ds
    iret
_one_char:
    mov dword edi,[0x2008]
    mov byte [edi],cl
    add edi,1
    mov byte [edi],al
    add edi,1
    mov dword [0x2008],edi
    pop ds
    iret
_printf:
    mov dword [0x2004],edi
    mov edx,edi
    mov ebx,edi
    shr ebx,8
    mov edi,ebx
    imul edi,0xA0
    add edi,0xB8000
    shl ebx,8
    sub edx,ebx
    shl edx,1
    add edi,edx
_video_string_loop:
    mov byte dl,[esi]
    mov byte [edi],dl
    add edi,1
    mov byte [edi],al
    add esi,1
    add edi,1
    loop _video_string_loop
    mov dword [0x2008],edi
    pop ds
    iret
_KEYBOARD:
    push ds
    mov bx,0x0010
    mov ds,bx

    in al,0x60
    mov ch,al
    call _convert
    pop ds
    mov al,0x20
    out 0x20,al
    mov eax,0x00000001
    iret
_int_ret:
    iret
_convert:
    mov dword ebx,[KSR]
    and ebx,0x00000001
    cmp ebx,0x00000000
    jz _shift_false
    cmp ebx,0x00000001
    jz _shift_ture
    ret
_shift_ture:
    cmp ch,0x29
    je _tilde
    cmp ch,0x02
    je _exclamation_mark
    cmp ch,0x03
    je _at_sign
    cmp ch,0x04
    je _hash_sign
    cmp ch,0x05
    je _dollar_sign
    cmp ch,0x06
    je _percent_sign
    cmp ch,0x07
    je _caret_sign
    cmp ch,0x08
    je _ampersand_sign
    cmp ch,0x09
    je _asterisk_sign
    cmp ch,0x0A
    je _left_parenthesis
    cmp ch,0x0B
    je _right_parenthesis
    cmp ch,0x0C
    je _underscore
    cmp ch,0x0D
    je _plus_sign
    cmp ch,0x10
    je _Q
    cmp ch,0x11
    je _W
    cmp ch,0x12
    je _E
    cmp ch,0x13
    je _R
    cmp ch,0x14
    je _T
    cmp ch,0x15
    je _Y
    cmp ch,0x16
    je _U
    cmp ch,0x17
    je _I
    cmp ch,0x18
    je _O
    cmp ch,0x19
    je _P
    cmp ch,0x1A
    je _square_brackets_l
    cmp ch,0x1B
    je _square_brackets_r
    cmp ch,0x1C
    je _enter
    cmp ch,0x1E
    je _A
    cmp ch,0x1F
    je _S
    cmp ch,0x20
    je _D
    cmp ch,0x21
    je _F
    cmp ch,0x22
    je _G
    cmp ch,0x23
    je _H
    cmp ch,0x24
    je _J
    cmp ch,0x25
    je _K
    cmp ch,0x26
    je _L
    cmp ch,0x27
    je _colon
    cmp ch,0x28
    je _double_quotation_mark
    cmp ch,0x2B
    je _vertical_line
    cmp ch,0x2C
    je _Z
    cmp ch,0x2D
    je _X
    cmp ch,0x2E
    je _C
    cmp ch,0x2F
    je _V
    cmp ch,0x30
    je _B
    cmp ch,0x31
    je _N
    cmp ch,0x32
    je _M
    cmp ch,0x33
    je _less_sign
    cmp ch,0x34
    je _greater_sign
    cmp ch,0x35
    je _question_mark
    cmp ch,0xAA
    je _release_shift
    cmp ch,0x9D
    je _release_shift
    cmp ch,0x3A
    je _caps_lock_f
    jmp _force_ret
    ret
_shift_false:
    cmp ch,0x29
    je _back_tick
    cmp ch,0x02
    je _1
    cmp ch,0x03
    je _2
    cmp ch,0x04
    je _3
    cmp ch,0x05
    je _4
    cmp ch,0x06
    je _5
    cmp ch,0x07
    je _6
    cmp ch,0x08
    je _7
    cmp ch,0x09
    je _8
    cmp ch,0x0A
    je _9
    cmp ch,0x0B
    je _0
    cmp ch,0x0C
    je _minus_sign
    cmp ch,0x0D
    je _equal_sign
    cmp ch,0x0E
    je _backspace
    cmp ch,0x10
    je _q
    cmp ch,0x11
    je _w
    cmp ch,0x12
    je _e
    cmp ch,0x13
    je _r
    cmp ch,0x14
    je _t
    cmp ch,0x15
    je _y
    cmp ch,0x16
    je _u
    cmp ch,0x17
    je _i
    cmp ch,0x18
    je _o
    cmp ch,0x19
    je _p
    cmp ch,0x1A
    je _curly_brackets_l
    cmp ch,0x1B
    je _curly_brackets_r
    cmp ch,0x1C
    je _enter
    cmp ch,0x1E
    je _a
    cmp ch,0x1F
    je _s
    cmp ch,0x20
    je _d
    cmp ch,0x21
    je _f
    cmp ch,0x22
    je _g
    cmp ch,0x23
    je _h
    cmp ch,0x24
    je _j
    cmp ch,0x25
    je _k
    cmp ch,0x26
    je _l
    cmp ch,0x27
    je _semicolon
    cmp ch,0x28
    je _single_quotation_mark
    cmp ch,0x2B
    je _backslash
    cmp ch,0x2A
    je _press_shift
    cmp ch,0x1D
    je _press_shift
    cmp ch,0x2C
    je _z
    cmp ch,0x2D
    je _x
    cmp ch,0x2E
    je _c
    cmp ch,0x2F
    je _v
    cmp ch,0x30
    je _b
    cmp ch,0x31
    je _n
    cmp ch,0x32
    je _m
    cmp ch,0x33
    je _comma
    cmp ch,0x34
    je _period
    cmp ch,0x35
    je _slash
    cmp ch,0x39
    je _space
    cmp ch,0x3A
    je _caps_lock_t
    jmp _force_ret
    ret






_back_tick:
    mov cl,0x60
    ret
_1:
    mov cl,0x31
    ret
_2:
    mov cl,0x32
    ret
_3:
    mov cl,0x33
    ret
_4:
    mov cl,0x34
    ret
_5:
    mov cl,0x35
    ret
_6:
    mov cl,0x36
    ret
_7:
    mov cl,0x37
    ret
_8:
    mov cl,0x38
    ret
_9:
    mov cl,0x39
    ret
_0:
    mov cl,0x30
    ret
_minus_sign:
    mov cl,0x2D
    ret
_equal_sign:
    mov cl,0x3D
    ret
_backspace:
    mov cl,ch
    ret
_q:
    mov cl,0x71
    ret
_w:
    mov cl,0x77
    ret
_e:
    mov cl,0x65
    ret
_r:
    mov cl,0x72
    ret
_t:
    mov cl,0x74
    ret
_y:
    mov cl,0x79
    ret
_u:
    mov cl,0x75
    ret
_i:
    mov cl,0x69
    ret
_o:
    mov cl,0x6F
    ret
_p:
    mov cl,0x70
    ret 
_curly_brackets_l:
    mov cl,0x5B
    ret
_curly_brackets_r:
    mov cl,0x5D
    ret
_a:
    mov cl,0x61
    ret
_s:
    mov cl,0x73
    ret
_d:
    mov cl,0x64
    ret
_f:
    mov cl,0x66
    ret
_g:
    mov cl,0x67
    ret
_h:
    mov cl,0x68
    ret
_j:
    mov cl,0x6A
    ret
_k:
    mov cl,0x6B
    ret
_l:
    mov cl,0x6C
    ret
_semicolon:
    mov cl,0x3B
    ret
_single_quotation_mark:
    mov cl,0x27
    ret
_backslash:
    mov cl,0x5C
    ret
_enter:
    mov cl,ch
    ret
_press_shift:
    mov ebx,[KSR]
    or ebx,0x00000001
    mov [KSR],ebx
    mov cl,ch
    jmp _force_ret
    ret
_z:
    mov cl,0x7A
    ret
_x:
    mov cl,0x78
    ret
_c:
    mov cl,0x63
    ret
_v:
    mov cl,0x76
    ret
_b:
    mov cl,0x62
    ret
_n:
    mov cl,0x6E
    ret
_m:
    mov cl,0x6D
    ret
_comma:
    mov cl,0x2C
    ret
_period:
    mov cl,0x2E
    ret
_slash:
    mov cl,0x2F
    ret
_caps_lock_t:
    mov ebx,[KSR]
    or ebx,0x00000001
    mov [KSR],ebx
    mov cl,ch
    jmp _force_ret
    ret





_space:
    mov cl,0x20
    ret







_tilde:
    mov cl,0x7E
    ret
_exclamation_mark:
    mov cl,0x21
    ret
_at_sign:
    mov cl,0x40
    ret
_hash_sign:
    mov cl,0x23
    ret
_dollar_sign:
    mov cl,0x24
    ret
_percent_sign:
    mov cl,0x25
    ret
_caret_sign:
    mov cl,0x5E
    ret
_ampersand_sign:
    mov cl,0x26
    ret
_asterisk_sign:
    mov cl,0x2A
    ret
_left_parenthesis:
    mov cl,0x28
    ret
_right_parenthesis:
    mov cl,0x29
    ret
_underscore:
    mov cl,0x5F
    ret
_plus_sign:
    mov cl,0x2B
    ret
_Q:
    mov cl,0x51
    ret
_W:
    mov cl,0x57
    ret
_E:
    mov cl,0x45
    ret
_R:
    mov cl,0x52
    ret
_T:
    mov cl,0x54
    ret
_Y:
    mov cl,0x59
    ret
_U:
    mov cl,0x55
    ret
_I:
    mov cl,0x49
    ret
_O:
    mov cl,0x4F
    ret
_P:
    mov cl,0x50
    ret
_square_brackets_l:
    mov cl,0x7B
    ret
_square_brackets_r:
    mov cl,0x7D
    ret
_A:
    mov cl,0x41
    ret
_S:
    mov cl,0x53
    ret
_D:
    mov cl,0x44
    ret
_F:
    mov cl,0x46
    ret
_G:
    mov cl,0x47
    ret
_H:
    mov cl,0x48
    ret
_J:
    mov cl,0x4A
    ret
_K:
    mov cl,0x4B
    ret
_L:
    mov cl,0x4C
    ret
_colon:
    mov cl,0x3A
    ret
_double_quotation_mark:
    mov cl,0x22
    ret
_vertical_line:
    mov cl,0x7C
    ret
_Z:
    mov cl,0x5A
    ret
_X:
    mov cl,0x58
    ret
_C:
    mov cl,0x43
    ret
_V:
    mov cl,0x56
    ret
_B:
    mov cl,0x42
    ret
_N:
    mov cl,0x4E
    ret
_M:
    mov cl,0x4D
    ret
_less_sign:
    mov cl,0x3C
    ret
_greater_sign:
    mov cl,0x3E
    ret
_question_mark:
    mov cl,0x3F
    ret
_release_shift:
    mov ebx,[KSR]
    and ebx,0xFFFFFFFE
    mov [KSR],ebx
    mov cl,ch
    jmp _force_ret
    ret
_caps_lock_f:
    mov ebx,[KSR]
    and ebx,0xFFFFFFFE
    mov [KSR],ebx
    mov cl,ch
    jmp _force_ret
    ret
_force_ret:
    pop edx
    pop ds
    mov al,0x20
    out 0x20,al
    mov eax,0x00000000
    iret
idtr_inf:
    dw 0x03FF
    dd 0x00006800
gdtr_inf:
    dw 0x03FF
    dd 0x00006000
KSR dw 0x00000000
times 4096 - ($ - $$) db 0