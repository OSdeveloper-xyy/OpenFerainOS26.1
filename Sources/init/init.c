#include <asm/descriptor.h>
#include <asm/paging.h>
#include <asm/basic.h>
#include <asm/task.h>
#include <asm/interrupt.h>
#include <device/DEVICERW.h>
#include <FileSystem/FSALL.h>
#include <stdio.h>
extern void default_ISR();
extern void DE_ISR();
extern void SS_ISR();
extern void GP_ISR();
extern void PS2_ISR();
extern void SERVER_ISR();
extern void VIDEO_ISR();
extern void DEVICE_ISR();
uint32_t BootBus,BootDrive;
uint8_t  BootID = 0;
static inline void delay(uint32_t num){
    for(int i = 0;i < num;i++){
        for(int j = 0;j < 1000;j++){
            __asm__ __volatile__("nop");
        }
    }
    return;
}
static inline void set_system_int(uint32_t id_num,void(*func_offset)(void)){
    uint32_t func = (uint32_t)func_offset;
    set_id(id_num,0x08,func,PL_KERNEL,ID_TYPE_INT);
    return;
}
static inline void set_user_int(uint32_t id_num,void(*func_offset)(void)){
    uint32_t func = (uint32_t)func_offset;
    set_id(id_num,0x08,func,PL_USER,ID_TYPE_INT);
    return;
}
static inline void idt_init(){
    for (int i = 0; i < 256; i++){
        set_system_int(i,default_ISR);             // Interrupt Descriptor Table Initialization
    }
    set_system_int(0x00,DE_ISR    );               // Divide Error
    set_system_int(0x0C,SS_ISR    );               // Stack Segment
    set_system_int(0x0D,GP_ISR    );               // General Protection
    set_user_int  (0x21,PS2_ISR   );
    set_user_int  (0x40,SERVER_ISR);
    set_user_int  (0x41,VIDEO_ISR );
    set_user_int  (0x42,DEVICE_ISR);
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
    PDE_SET(0,KERNEL_PD_PHY_ADDR,KERNEL_PT_PHY_ADDR,P_SYSTEM,P_READ_WRITE,P_PRESENT);     // Kernel 4MB
    for(int i = 1;i < 1023;i++)PDE_SET(i,KERNEL_PD_PHY_ADDR,TEMP_PT_BASE,P_SYSTEM,P_READ_WRITE,P_PRESENT);
    for(int i = 0;i < 1024;i++)PTE_SET(i,KERNEL_PT_PHY_ADDR,(uint32_t)(i * PAGE_SIZE),P_SYSTEM,P_READ_WRITE,P_PRESENT);
    load_cr3(KERNEL_PD_PHY_ADDR);
    paging_enable();
    return;
}
static inline void mem_map_init(){
    // Low 4MB has used;
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
static inline void device_init(){
    PCI_Enumeration();
    PCI_ISA_ENU();
    PCI_IDE_ENU();
    PCI_SATA_ENU();
    PCI_USB_ENU();
    PCI_SDHOST_ENU();
    PCI_IEEE1394_ENU();
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
static inline void shell_init(){
    char line_string[] = "--------------------------------------------------------------------------------";
    uint32_t line_len = 80,len_xy = 0x1700;
    __asm__ __volatile__ (
        "movb $0x00,%%ah\n"
        "int $0x41\n"
        :
        : "S"(&line_string),"D"(len_xy),"c"(line_len),"a"(color)
        : "ebx","edx","memory"
    );
    color = 0x70;
    println("---**Welcome to OpenFerain OS 26.1**---");
    color = 0x07;
    return;
}
static inline void jmp_shell(){
    return;
}
__attribute__((noinline)) void init_main(){
    descriptor_init();
    paging_init();
    mem_map_init();
    port_init();
    device_init();
    extension_init();
    shell_init();
    jmp_shell();
    while(1);
}