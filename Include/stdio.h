#ifndef      STDIO_H
#define      STDIO_H
#include <stdint.h>
#define true    0x00000001
#define false   0x00000000 
uint8_t color = 0x07;
uint32_t video_memory_point = 0x0000FF00;
static __attribute__((optimize("O0","no-inline"))) uint32_t Server(uint8_t Funcnum){
    uint32_t ret_val;
    __asm__ __volatile__ (
        "movb %%al,%%bh\n"
        "int $0x31\n"
        : "=a"(ret_val)
        : "a"(Funcnum)
        : "ebx","edi","memory"
    );
    return ret_val;
}
static __attribute__((optimize("O2","no-inline"))) uint16_t char_to_hex(char ch){
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0x1000;
}
static __attribute__((optimize("O2","no-inline"))) uint16_t hex_to_char(uint8_t hex){
    if (hex >= 0 && hex <= 9) return hex + 0x30;
    if (hex >= 0xA && hex <= 0xF) return hex + 0x57;
    return 0xFF3F;
}
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
static __attribute__((optimize("O0","no-inline"))) void test_scolling(){
    video_memory_point = video_memory_point + 0x00000100;
    if(video_memory_point >= 0x00001700){
        uint32_t sub_num = video_memory_point - 0x00001700;
        sub_num += 0x00000100;
        video_memory_point -= sub_num;
        while(sub_num != 0){
            sub_num -= 0x00000100;
            __asm__ __volatile__ (
                "movb $0x02,%%ah\n"
                "int $0x30\n"
                :
                : 
                : "eax","ebx","ecx","edx","edi","esi","memory"
            );
            __asm__ __volatile__ (
                "movb $0x04,%%ah\n"
                "movl $0x00000016,%%edi\n"
                "int $0x30\n"
                :
                : "a"(color)
                : "ebx","ecx","edx","edi","esi","memory"
            );
            __asm__ __volatile__ (
                "movb $0x80,%%ah\n"
                "int $0x30\n"
                :
                : 
                : "eax","ebx","ecx","edx","edi","esi","memory"
            );
        }
    }
    return;
}
static __attribute__((optimize("O2","no-inline"))) uint32_t recursion_test(uint32_t len){
    uint32_t ret_val = 0;
    while(len >= 80){
        len -= 80;
        test_scolling();
        video_memory_point -= 0x00000100;
        ret_val += 0x00000100;
    }
    test_scolling();
    return ret_val;
}
static __attribute__((optimize("O0","no-inline"))) void printf(const char *s){
    uint32_t len = 0;
    while (s[len])++len;
    uint32_t point_add_num = recursion_test(len);
    __asm__ __volatile__ (
        "movb $0x00,%%ah\n"
        "int $0x41\n"
        "movb $0x80,%%ah\n"
        "int $0x41\n"
        :
        : "S"(s),"D"(video_memory_point),"c"(len),"a"(color)
        : "ebx","edx","memory"
    );
    video_memory_point += point_add_num;
    return;
}
static __attribute__((optimize("O0","no-inline"))) void print_time(uint32_t mode){
    uint32_t len = 0;
    if(mode == 0){
        mode = 0;
        len = 19;
    }else if(mode == 1){
        mode = 0;
        len = 4;
    }else if(mode == 2){
        mode = 5;
        len = 2;
    }else if(mode == 3){
        mode = 8;
        len = 2;
    }else if(mode == 4){
        mode = 11;
        len = 2;
    }else if(mode == 5){
        mode = 14;
        len = 2;
    }else if(mode == 6){
        mode = 17;
        len = 2;
    }
    test_scolling();
    __asm__ __volatile__ (
        "movb $0x00,%%ah\n"
        "int $0x30\n"
        "movb $0x80,%%ah\n"
        "int $0x30\n"
        :
        : "S"(mode + 0x4000),"D"(video_memory_point),"c"(len),"a"(color)
        : "ebx","edx","memory"
    );
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