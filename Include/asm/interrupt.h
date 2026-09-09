#ifndef      INTERRUPT_H
#define      INTERRUPT_H
#include <stdint.h>
#include <asm/basic.h>
#define VIDEO_MEMORY_BASE_ADDR    0x000B8000
uint8_t KEY_SHIFT_FLAG = 0,KEY_CAPSLOCK_FLAG = 0;
void KeyBack(){
    return;
}
void KeyEnter(){
    return;
}
void KeyShiftD(){
    KEY_SHIFT_FLAG    = 1;
    return;
}
void KeyShiftU(){
    KEY_SHIFT_FLAG    = 0;
    return;
}
void KeyCapsLk(){
    KEY_CAPSLOCK_FLAG ^= 1;
    return;
}
char PS2Expand(){
    uint8_t key = inpb(0x60);
    switch (key){
    }
    return key;
}
char PS2CLF(uint8_t key){
    switch (key){
    case 0x10:key = 'q'  ;break;
    case 0x11:key = 'w'  ;break;
    case 0x12:key = 'e'  ;break;
    case 0x13:key = 'r'  ;break;
    case 0x14:key = 't'  ;break;
    case 0x15:key = 'y'  ;break;
    case 0x16:key = 'u'  ;break;
    case 0x17:key = 'i'  ;break;
    case 0x18:key = 'o'  ;break;
    case 0x19:key = 'p'  ;break;
    case 0x1E:key = 'a'  ;break;
    case 0x1F:key = 's'  ;break;
    case 0x20:key = 'd'  ;break;
    case 0x21:key = 'f'  ;break;
    case 0x22:key = 'g'  ;break;
    case 0x23:key = 'h'  ;break;
    case 0x24:key = 'j'  ;break;
    case 0x25:key = 'k'  ;break;
    case 0x26:key = 'l'  ;break;
    case 0x2C:key = 'z'  ;break;
    case 0x2D:key = 'x'  ;break;
    case 0x2E:key = 'c'  ;break;
    case 0x2F:key = 'v'  ;break;
    case 0x30:key = 'b'  ;break;
    case 0x31:key = 'n'  ;break;
    case 0x32:key = 'm'  ;break;
    default:  key = 0xFF ;break;
    }
    return key;
}
char PS2CLT(uint8_t key){
    switch (key){
    case 0x10:key = 'Q'  ;break;
    case 0x11:key = 'W'  ;break;
    case 0x12:key = 'E'  ;break;
    case 0x13:key = 'R'  ;break;
    case 0x14:key = 'T'  ;break;
    case 0x15:key = 'Y'  ;break;
    case 0x16:key = 'U'  ;break;
    case 0x17:key = 'I'  ;break;
    case 0x18:key = 'O'  ;break;
    case 0x19:key = 'P'  ;break;
    case 0x1E:key = 'A'  ;break;
    case 0x1F:key = 'S'  ;break;
    case 0x20:key = 'D'  ;break;
    case 0x21:key = 'F'  ;break;
    case 0x22:key = 'G'  ;break;
    case 0x23:key = 'H'  ;break;
    case 0x24:key = 'J'  ;break;
    case 0x25:key = 'K'  ;break;
    case 0x26:key = 'L'  ;break;
    case 0x2C:key = 'Z'  ;break;
    case 0x2D:key = 'X'  ;break;
    case 0x2E:key = 'C'  ;break;
    case 0x2F:key = 'V'  ;break;
    case 0x30:key = 'B'  ;break;
    case 0x31:key = 'N'  ;break;
    case 0x32:key = 'M'  ;break;
    default:  key = 0xFF ;break;
    }
    return key;
}
__attribute__((noinline)) void ISR_FOR_PS2(uint32_t keybuf_ptr){
    uint8_t key = inpb(0x60);
    if((0x10 <= key <= 0x19) || (0x1E <= key <= 0x26) || (0x2C <= key <= 0x32)){
        if(KEY_CAPSLOCK_FLAG == 0){
            key = PS2CLF(key);
        }else if(KEY_CAPSLOCK_FLAG == 1){
            key = PS2CLT(key);
        }
    }else{
        switch(key){
            case 0x29:
                if(KEY_SHIFT_FLAG == 0)key = '`'  ;
                else                   key = '~'  ;
                break;
            case 0x02:
                if(KEY_SHIFT_FLAG == 0)key = '1'  ;
                else                   key = '!'  ;
                break;
            case 0x03:
                if(KEY_SHIFT_FLAG == 0)key = '2'  ;
                else                   key = '@'  ;
                break;
            case 0x04:
                if(KEY_SHIFT_FLAG == 0)key = '3'  ;
                else                   key = '#'  ;
                break;
            case 0x05:
                if(KEY_SHIFT_FLAG == 0)key = '4'  ;
                else                   key = '$'  ;
                break;
            case 0x06:
                if(KEY_SHIFT_FLAG == 0)key = '5'  ;
                else                   key = '%'  ;
                break;
            case 0x07:
                if(KEY_SHIFT_FLAG == 0)key = '6'  ;
                else                   key = '^'  ;
                break;
            case 0x08:
                if(KEY_SHIFT_FLAG == 0)key = '7'  ;
                else                   key = '&'  ;
                break;
            case 0x09:
                if(KEY_SHIFT_FLAG == 0)key = '8'  ;
                else                   key = '*'  ;
                break;
            case 0x0A:
                if(KEY_SHIFT_FLAG == 0)key = '9'  ;
                else                   key = '('  ;
                break;
            case 0x0B:
                if(KEY_SHIFT_FLAG == 0)key = '0'  ;
                else                   key = ')'  ;
                break;
            case 0x0C:
                if(KEY_SHIFT_FLAG == 0)key = '-'  ;
                else                   key = '='  ;
                break;
            case 0x0D:
                if(KEY_SHIFT_FLAG == 0)key = '='  ;
                else                   key = '+'  ;
                break;
            case 0x1A:
                if(KEY_SHIFT_FLAG == 0)key = '['  ;
                else                   key = '{'  ;
                break;
            case 0x1B:
                if(KEY_SHIFT_FLAG == 0)key = ']'  ;
                else                   key = '}'  ;
                break;
            case 0x27:
                if(KEY_SHIFT_FLAG == 0)key = ';'  ;
                else                   key = ':'  ;
                break;
            case 0x28:
                if(KEY_SHIFT_FLAG == 0)key = 0x27 ;
                else                   key = '"'  ;
                break;
            case 0x2B:
                if(KEY_SHIFT_FLAG == 0)key = 0x5C ;
                else                   key = '|'  ;
                break;
            case 0x33:
                if(KEY_SHIFT_FLAG == 0)key = ','  ;
                else                   key = '<'  ;
                break;
            case 0x34:
                if(KEY_SHIFT_FLAG == 0)key = '.'  ;
                else                   key = '>'  ;
                break;
            case 0x35:
                if(KEY_SHIFT_FLAG == 0)key = '/'  ;
                else                   key = '?'  ;
                break;
            case 0x4F:
                if(KEY_SHIFT_FLAG == 0)key = '1'  ;
                break;
            case 0x50:
                if(KEY_SHIFT_FLAG == 0)key = '2'  ;
                break;
            case 0x51:
                if(KEY_SHIFT_FLAG == 0)key = '3'  ;
                break;
            case 0x4B:
                if(KEY_SHIFT_FLAG == 0)key = '4'  ;
                break;
            case 0x4C:
                if(KEY_SHIFT_FLAG == 0)key = '5'  ;
                break;
            case 0x4D:
                if(KEY_SHIFT_FLAG == 0)key = '6'  ;
                break;
            case 0x47:
                if(KEY_SHIFT_FLAG == 0)key = '7'  ;
                break;
            case 0x48:
                if(KEY_SHIFT_FLAG == 0)key = '8'  ;
                break;
            case 0x49:
                if(KEY_SHIFT_FLAG == 0)key = '9'  ;
                break;
            case 0x52:
                if(KEY_SHIFT_FLAG == 0)key = '0'  ;
                break;
            case 0x4A:
                if(KEY_SHIFT_FLAG == 0)key = '-'  ;
                break;
            case 0x53:
                if(KEY_SHIFT_FLAG == 0)key = '.'  ;
                break;
            case 0x4E:
                if(KEY_SHIFT_FLAG == 0)key = '+'  ;
                break;
            case 0x37:
                if(KEY_SHIFT_FLAG == 0)key = '*'  ;
                break;
            case 0x39:
                key = ' '  ;
                break;
            case 0x0F:
                /*Tab*/
                key = key  ;
                break;
            case 0x3A:
                KeyCapsLk();
                key = 0xFF ;
                break;
            case 0x1C:
                KeyEnter() ;
                break;
            case 0x0E:
                KeyBack()  ;
                break;
            case 0x2A:case 0x36:
                KeyShiftD();
                key = 0x2A;
                break;
            case 0xAA:case 0xB6:
                KeyShiftU();
                key = 0xAA;
                break;
            default:
                key = 0xFF;
                break;
        }
    }
    if(key == 0xFF)return;
    volatile uint8_t *mem_ptr = (volatile uint8_t *)(keybuf_ptr);
    *mem_ptr = key;
    return;
}
__attribute__((noinline)) void cursor_set(uint32_t x,uint32_t y){
    uint32_t vm_offset = (y * 80 + x);
    uint8_t high = (uint8_t)((vm_offset & 0x0000FF00) >> 8);
    uint8_t low  = (uint8_t)(vm_offset & 0x000000FF);
    outpb(0x3D4,0x0E);
    outpb(0x3D5,high);
    outpb(0x3D4,0x0F);
    outpb(0x3D5,low);
    return;
}
__attribute__((noinline)) void video_isr_print_string(
    uint32_t    esi_reg,
    uint32_t    edi_reg,
    uint32_t    length,
    uint32_t    color
){
    uint32_t video_memory_write_base = 
        (((edi_reg & 0x0000FF00) >> 8) * 0xA0)   +
        ((edi_reg & 0x000000FF)*2)               +
        VIDEO_MEMORY_BASE_ADDR                   ;
    for(uint32_t i = 0;i < length;i++){
        volatile uint8_t *source_pointer = (volatile uint8_t *)(esi_reg + i);
        volatile uint16_t *destination_pointer = (volatile uint16_t *)(video_memory_write_base + i * 2);
        uint16_t write_word = ((color & 0xFF) << 8) | *source_pointer;
        *destination_pointer = write_word;
    }
    return;
}
__attribute__((noinline)) void vm_scolling(){
    for(int i = 0;i < ((22 * 160) / 4);i++){
        volatile uint32_t *source_pointer = (volatile uint32_t *)(0xB80A0 + i * 4);
        volatile uint32_t *destination_pointer = (volatile uint32_t *)(0xB8000 + i * 4);
        *destination_pointer = *source_pointer;
    }
    for(int i = 0;i < 80;i++){
        volatile uint8_t *destination_pointer1 = (volatile uint8_t *)(0xB8DC0 + i * 2);
        *destination_pointer1 = 0x20;
        volatile uint8_t *destination_pointer2 = (volatile uint8_t *)(0xB8E60 + i * 2);
        *destination_pointer2 = 0x2D;
    }
    return;
}
#endif