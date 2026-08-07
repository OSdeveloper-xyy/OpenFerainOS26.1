#ifndef      DESCRIPTOR_H
#define      DESCRIPTOR_H
#include <stdint.h>

#define GDT_BASE       0x00002000
#define GDT_LIMIT      0x00001FFF
#define IDT_BASE       0x00004000
#define IDT_LIMIT      0x000007FF
#define GD_SYSTEM      0x00
#define GD_NORMAL      0x01
#define GD_TYPE_CODE   0x0A
#define GD_TYPE_DATA   0x02
#define GD_TYPE_ATSS   0x09
#define GD_TYPE_BTSS   0x0B
#define GD_TYPE_LDT    0x02
#define GD_G_BYTE      0x00
#define GD_G_PAGE      0x01
#define PL_KERNEL      0x00
#define PL_USER        0x03
#define ID_TYPE_INT    0x0E
#define ID_TYPE_TRAP   0x0F
#define ID_TYPE_TASK   0x05

static inline void gdtr_init(uint16_t limit,uint32_t base){
    uint8_t gdtr_inf[6];                     // Global Descriptor Table Register Information Structure
    *(uint16_t *)(gdtr_inf) = limit;         // Global Descriptor Table Limit
    *(uint32_t *)(gdtr_inf + 2) = base;      // Global Descriptor Table Base Address
    __asm__ __volatile__(                    // Load Global Descriptor Table Register
        "lgdt %[gdtr]"
        :
        : [gdtr] "m"(gdtr_inf)
        : "memory"
    );
    return;
}
static inline void idtr_init(uint16_t limit,uint32_t base){
    uint8_t idtr_inf[6];                     // Interrupt Descriptor Table Register Information Structure
    *(uint16_t *)(idtr_inf) = limit;         // Interrupt Descriptor Table Limit
    *(uint32_t *)(idtr_inf + 2) = base;      // Interrupt Descriptor Table Base Address
    __asm__ __volatile__(                    // Load Interrupt Descriptor Table Register
        "lidt %[idtr]"
        :
        : [idtr] "m"(idtr_inf)
        : "memory"
    );
    return;
}
static inline void set_gd(
    uint32_t gd_number,                      // 0~1023
    uint32_t gd_base,                        // Descriptor base address
    uint32_t gd_limit,                       // Descriptor limit
    uint8_t G,                               // 0:Byte 1:Page
    uint8_t S,                               // 0:System 1:Code/Data
    uint8_t type,                            // Descriptor type
    uint8_t dpl                              // Descriptor Privilege Level
){
    if(gd_number > GDT_LIMIT)return;         // Number Check
    volatile uint32_t *gd_address_low = (volatile uint32_t *)(GDT_BASE + gd_number * 8);
    volatile uint32_t *gd_address_high = (volatile uint32_t *)((GDT_BASE + gd_number * 8) + 4);
    uint32_t gd_low =
        (gd_base & 0xFFFF) << 16 |           // Base 15:0
        (gd_limit & 0xFFFF);                 // Limit 15:0
    *gd_address_low = gd_low;                // Write Low 32 bits
    uint32_t gd_high = 
        (gd_base & 0xFF000000)   |           // Base 31:24
        ((G & 0x01) << 23)       |           // G
        (0x00408000)             |           // D/B,P
        (gd_limit & 0x000F0000)  |           // Limit 19:16
        ((dpl & 0x03) << 13)     |           // DPL
        ((S & 0x01) << 12)       |           // S
        ((type & 0x0F) << 8)     |           // Type
        ((gd_base & 0x00FF0000) >> 16);      // Base 23:16
    *gd_address_high = gd_high;              // Write High 32 bits
    return;
}

static inline void set_id(
    uint32_t id_number,                      // 0~1023
    uint16_t id_segment,                     // Descriptor segment selector
    uint32_t id_offset,                      // Descriptor offset
    uint8_t dpl,                             // Descriptor Privilege Level
    uint8_t type                             // Descriptor type
){
    if(id_number > IDT_LIMIT)return;         // Number Check
    volatile uint32_t *id_address_low = (volatile uint32_t *)(IDT_BASE + id_number * 8);
    volatile uint32_t *id_address_high = (volatile uint32_t *)((IDT_BASE + id_number * 8) + 4);
    uint32_t id_low,id_high;
    if(type == ID_TYPE_TASK){
        id_low = (id_segment << 16);         // Segment 15:0
        id_high = (0x00008000)       |       // P
            ((dpl & 0x03) << 13)     |       // DPL
            ((type & 0x0F) << 8);            // Type
    }else{
        id_low =
            (id_segment << 16)       |       // Segment 15:0
            (id_offset & 0xFFFF);            // Offset 15:0
        id_high = 
            (id_offset & 0xFFFF0000) |         
            (0x00008000)             |       // P
            ((dpl & 0x03) << 13)     |       // DPL
            ((type & 0x0F) << 8);            // Type
    }
    *id_address_low = id_low;                // Write Low 32 bits
    *id_address_high = id_high;              // Write High 32 bits
    return;
}
#endif