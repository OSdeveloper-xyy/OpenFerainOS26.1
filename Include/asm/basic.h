#ifndef      BASIC_H
#define      BASIC_H
#include <stdint.h>

static inline uint8_t inpb(uint16_t port){
    uint8_t portout;
    __asm__ __volatile__ (
        "inb %%dx,%%al"
        : "=a"(portout)
        : "d"(port)
        : "memory"
    );
    return portout;
}
static inline void outpb(uint16_t port,uint8_t operand){
    __asm__ __volatile__ (
        "outb %%al,%%dx"
        : 
        : "a"(operand),"d"(port)
        : "memory"
    );
    return;
}

static inline uint32_t inpd(uint16_t port){
    uint32_t portout;
    __asm__ __volatile__ (
        "inl %%dx,%%eax"
        : "=a"(portout)
        : "d"(port)
        : "memory"
    );
    return portout;
}
static inline void outpd(uint16_t port,uint32_t operand){
    __asm__ __volatile__ (
        "outl %%eax,%%dx"
        : 
        : "a"(operand),"d"(port)
        : "memory"
    );
    return;
}

#endif