#include <asm/descriptor.h>
#include <asm/paging.h>
#include <asm/basic.h>
#include <asm/task.h>
#include <asm/interrupt.h>
#include <stdio.h>
extern void default_ISR();
extern void DE_ISR();
extern void SS_ISR();
extern void GP_ISR();
extern void SERVER_ISR();
extern void VIDEO_ISR();
static inline void set_system_int(uint32_t id_num,void(*func_offset)(void)){
    uint32_t func = (uint32_t)func_offset + 0x00100000;
    set_id(id_num,0x08,func,PL_KERNEL,ID_TYPE_INT);
    return;
}
static inline void idt_init(){
    for (int i = 0; i < 256; i++){
        set_system_int(i,default_ISR);       // Interrupt Descriptor Table Initialization
    }
    set_system_int(0,DE_ISR);                // Divide Error
    set_system_int(12,SS_ISR);               // Stack Segment
    set_system_int(13,GP_ISR);               // General Protection
    set_system_int(64,SERVER_ISR);
    set_system_int(65,VIDEO_ISR);
    return;
}
static inline void descriptor_init(){
    set_gd(0,0x00000000,0x00000,0,0,0x0,0);                                      // NULL
    set_gd(1,0x00000000,0xFFFFF,GD_G_PAGE,GD_NORMAL,GD_TYPE_CODE,PL_KERNEL);     // Kernel Code Segment
    set_gd(2,0x00000000,0xFFFFF,GD_G_PAGE,GD_NORMAL,GD_TYPE_DATA,PL_KERNEL);     // Kernel Data Segment
    set_gd(3,0x00000000,0x00000,0,0,0x0,0);                                      // NULL
    gdtr_init(GDT_LIMIT,GDT_BASE);
    idt_init();
    idtr_init(IDT_LIMIT,IDT_BASE);
    return;
}
static inline void paging_init(){
    PDE_SET(0,KERNEL_PD_PHY_ADDR,KERNEL_PT_PHY_ADDR,P_STSTEM,P_READ_WRITE,P_PRESENT);     // Kernel 4MB
    for(int i = 0;i < 1024;i++){
        uint32_t PAGE_PHY_ADDR = (uint32_t)(i * PAGE_SIZE);
        PTE_SET(i,KERNEL_PT_PHY_ADDR,PAGE_PHY_ADDR,P_STSTEM,P_READ_WRITE,P_PRESENT);
    }
    load_cr3(KERNEL_PD_PHY_ADDR);
    paging_enable();
    return;
}
static inline void mem_map_init(){
    for(uint32_t i = 0;i < 1024;i++){
        mem_map_write(i);
    }
    return;
}
static inline void port_init(){
    uint8_t operand;
    //8259A
    outpb(0x20,0x11);
    outpb(0x21,0x20);
    outpb(0x21,0x04);
    outpb(0x21,0x01);
    outpb(0x21,0xFF);
    outpb(0xA0,0x11);
    outpb(0xA1,0x28);
    outpb(0xA1,0x02);
    outpb(0xA1,0x01);
    outpb(0xA1,0xFF);
    //RTC
    outpb(0x70,0x0C);
    inpb(0x71);
    outpb(0x70,0x0B);
    outpb(0x71,0x12);
    outpb(0x70,0x0A);
    operand = inpb(0x71);
    operand = operand & 0xF0;
    outpb(0x70,0x0A);
    outpb(0x71,operand); 
    outpb(0x70,0x0C);
    inpb(0x71);
    //PIT
    outpb(0x43,0x34);
    outpb(0x40,0xA9);
    outpb(0x40,0x04);
    return;
}
static inline void extension_init(){
    __asm__ __volatile__(
        "fninit\n"
        "mov %%cr0, %%eax\n"
        "and $0xFFFFFFF3, %%eax\n"
        "or $0x02, %%eax\n"
        "mov %%eax, %%cr0\n"
        "mov %%cr4, %%eax\n"
        "or $0x00000600, %%eax\n"
        "mov %%eax, %%cr4\n"
        : 
        : 
        : "eax", "memory"
    );
    return;
}
static inline void window_init(){
    printf("OpenFerain 26.1");
    return;
}
static inline void creat_task(){
    return;
}
__attribute__((noinline)) void init_main(){
    descriptor_init();
    paging_init();
    mem_map_init();
    port_init();
    extension_init();
    window_init();
    creat_task();
    while(1);
    //Else initialization code can be added here
}