#ifndef      STDIO_H
#define      STDIO_H
#include <stdint.h>
#include <bit.h>
#define true    0x00000001
#define false   0x00000000 
uint8_t color = 0x07;
uint8_t vmx = 0,vmy = 0;
static __attribute__((optimize("O0","no-inline"))) void set_color(){
    __asm__ __volatile__ (
        "movb $0x05,%%ah\n"
        "int $0x30\n"
        :
        : "a"(color)
        : "ebx","ecx","edx","memory"
    );
    return;
}
static __attribute__((optimize("O0","no-inline"))) void set_cursor(uint8_t x,uint8_t y){
    vmx = x;
    vmy = y;
    __asm__ __volatile__ (
        "movb $0x80,%%ah\n"
        "int $0x41\n"
        :
        : "S"(vmx),"D"(vmy)
        : "eax","edx","memory"
    );
    return;
}
static __attribute__((optimize("O0","no-inline"))) void asm_scolling(){
    __asm__ __volatile__ (
        "movb $0x81,%%ah\n"
        "int $0x41\n"
        :
        : 
        : "eax","edx","memory"
    );
    return;
}
static __attribute__((optimize("O0","no-inline"))) uint32_t test_scolling(uint32_t len){
    while(vmy >= 23){
        vmy--;
        asm_scolling();
    }
    uint32_t final_vmx = vmx + len;
    uint32_t test_vmy  = vmy;
    while(final_vmx >= 80){
        test_vmy++;
        if(test_vmy >= 23){
            vmy--;
            test_vmy--;
            asm_scolling();
        }
        final_vmx -= 80;
    }
    uint8_t rawvmx = vmx,rawvmy = vmy;
    set_cursor(final_vmx,test_vmy);
    return (uint32_t)((rawvmy << 8) | (rawvmx));
}
static __attribute__((optimize("O0","no-inline"))) void printf(const char *s){
    uint32_t len = 0;
    while(s[len])++len;
    uint32_t video_memory_ptr = test_scolling(len);
    __asm__ __volatile__ (
        "movb $0x00,%%ah\n"
        "int $0x41\n"
        :
        : "S"(s),"D"(video_memory_ptr),"c"(len),"a"(color)
        : "ebx","edx","memory"
    );
    return;
}
static __attribute__((optimize("O0","no-inline"))) void println(const char *s){
    uint32_t len = 0;
    while(s[len])++len;
    uint32_t video_memory_ptr = test_scolling(len);
    __asm__ __volatile__ (
        "movb $0x00,%%ah\n"
        "int $0x41\n"
        :
        : "S"(s),"D"(video_memory_ptr),"c"(len),"a"(color)
        : "ebx","edx","memory"
    );
    vmx = 0;
    vmy++;
    set_cursor(vmx,vmy);
    return;
}
static __attribute__((optimize("O0","no-inline"))) void type(char s){
    __asm__ __volatile__ (
        "movb $0x01,%%ah\n"
        "int $0x30\n"
        "movb $0x80,%%ah\n"
        "int $0x30\n"
        :
        : "c"(s),"a"(color)
        : "edi","memory"
    );
    return;
}

#endif