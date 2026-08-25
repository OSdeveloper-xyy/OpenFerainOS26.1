ENTRY(init_setup)
SECTIONS 
{
    . = 0x100000;
    .entry : {
        KEEP(*(.entry))
    }
    .text : {
        *(.text .text.*)
    }
    .rodata : {
        *(.rodata*)
    }
    .data : {
        *(.data*)
    }
    .bss (NOLOAD) : {
        *(.bss*)
        *(COMMON)
    }
    /DISCARD/ : {
        *(.comment*)
        *(.note*)
        *(.eh_frame*)
    }
    .pad : {
        FILL(0x00);
        BYTE(0);
        . = ALIGN(4096);
    }
}