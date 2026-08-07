#ifndef      INTERRUPT_H
#define      INTERRUPT_H
#include <stdint.h>
#include <asm/basic.h>
#define VIDEO_MEMORY_BASE_ADDR    0x000B8000
__attribute__((noinline)) void video_isr_print_string(
    uint32_t    esi_reg,
    uint32_t    edi_reg,
    uint32_t    length,
    uint32_t     color){
    uint32_t video_memory_write_base = 
        VIDEO_MEMORY_BASE_ADDR                   + 
        (((edi_reg & 0x0000FF00) >> 8) * 0xA0)   +
        (edi_reg & 0x000000FF);    
    for(uint32_t i = 0;i < length;i++){
        volatile uint8_t *source_pointer = (volatile uint8_t *)(esi_reg + i);
        volatile uint16_t *destination_pointer = (volatile uint16_t *)(video_memory_write_base + i * 2);
        uint16_t write_word = 
            ((color & 0xFF) << 8)     |
            *source_pointer;
        *destination_pointer = write_word;
    }
    return;
}
__attribute__((noinline)) void cursor_set(uint32_t x,uint32_t y){
    uint32_t vm_offset = (x * 80 + y);
    uint8_t high = (uint8_t)((vm_offset & 0x0000FF00) >> 8);
    uint8_t low  = (uint8_t)(vm_offset & 0x000000FF);
    outpb(0x3D4,0x0E);
    outpb(0x3D5,high);
    outpb(0x3D4,0x0F);
    outpb(0x3D5,low);
    return;
}
#endif