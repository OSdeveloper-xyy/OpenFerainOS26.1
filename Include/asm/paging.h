#ifndef      PAGING_H
#define      PAGING_H
#include <stdint.h>

#define KERNEL_PD_PHY_ADDR   0x00000000
#define KERNEL_PT_PHY_ADDR   0x00001000
#define P_USER               0x01
#define P_STSTEM             0x00
#define P_READ_WRITE         0x01
#define P_ONLY_READ          0x00
#define P_PRESENT            0x01
#define P_NOT_PERSENT        0x00
#define PAGE_SIZE            0x1000

static inline void PDE_SET(
    uint32_t PDE_NUM,                        // Page Directory Entry Number
    uint32_t PD_PHY_BASE,                    // Page Directory BASE
    uint32_t PT_PHY_ADDR,                    // Page Table physical address
    uint8_t  US,                             // USER MODE ENABLE
    uint8_t  RW,                             // ONLY READ OR READ WRITE
    uint8_t  P                               // PRESENT
){
    if(PDE_NUM > 1023)return;                // PDE Check
    volatile uint32_t *PDE_BASE_ADDR = (volatile uint32_t *)(PD_PHY_BASE + PDE_NUM * 4);
    uint32_t PDE = 
        (PT_PHY_ADDR & 0xFFFFF000)    |
        ((US & 0x01) << 2)            |
        ((RW & 0x01) << 1)            |
        (P & 0x01);
    *PDE_BASE_ADDR = PDE;
    return;
}



static inline void PTE_SET(
    uint32_t PTE_NUM,                        // Page Table Entry Number
    uint32_t PT_PHY_BASE,                    // Page Table BASE
    uint32_t PAGE_PHY_ADDR,                  // Page physical address
    uint8_t  US,                             // USER MODE ENABLE
    uint8_t  RW,                             // ONLY READ OR READ WRITE
    uint8_t  P                               // PRESENT
){
    if(PTE_NUM > 1023)return;                // PTE Check
    volatile uint32_t *PTE_BASE_ADDR = (volatile uint32_t *)(PT_PHY_BASE + PTE_NUM * 4);
    uint32_t PTE = 
        (PAGE_PHY_ADDR & 0xFFFFF000)  |
        ((US & 0x01) << 2)            |
        ((RW & 0x01) << 1)            |
        (P & 0x01);
    *PTE_BASE_ADDR = PTE;
    return;
}

static inline void load_cr3(uint32_t pd_addr){
    __asm__ __volatile__ (
        "movl %[cr3_data],%%cr3"
        : 
        : [cr3_data]"r"(pd_addr)
        : "memory"
    );
    return;
}

static inline void paging_enable(){
    __asm__ __volatile__ (
        "movl %%cr0,%%eax\n\t"
        "orl $0x80000000,%%eax\n\t"
        "movl %%eax,%%cr0"
        : 
        : 
        : "eax","memory"
    );
    return;
}

#endif