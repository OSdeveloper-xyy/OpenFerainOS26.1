#ifndef      TASK_H
#define      TASK_H

#include <stdint.h>
#include <asm/paging.h>

#define TASK_DATA_BASE       0x00005000
#define TASK_LDT_BASE        0x00015000
#define MEM_MAP_BASE         0x00020000
#define TEMP_PT_BASE         0x00048000

struct task_state_segment{
    uint16_t Previous_Task_Link;uint16_t Reserved0;
    uint32_t ESP0;
    uint16_t SS0;uint16_t Reserved1;
    uint32_t ESP1;
    uint16_t SS1;uint16_t Reserved2;
    uint32_t ESP2;
    uint16_t SS2;uint16_t Reserved3;
    uint32_t CR3;
    uint32_t EIP;
    uint32_t EFLAGS;
    uint32_t EAX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t EBX;
    uint32_t ESP;
    uint32_t EBP;
    uint32_t ESI;
    uint32_t EDI;
    uint16_t ES;uint16_t Reserved4;
    uint16_t CS;uint16_t Reserved5;
    uint16_t SS;uint16_t Reserved6;
    uint16_t DS;uint16_t Reserved7;
    uint16_t FS;uint16_t Reserved8;
    uint16_t GS;uint16_t Reserved9;
    uint16_t LDT_Segment_Selector;
    uint32_t Reserved10;
    uint16_t IO_Map_base_addr;
}__attribute__((packed));
struct task_data_structure{
    uint32_t state;
    uint32_t Remaining_time;
    uint32_t priority;
    struct task_state_segment tss;   
}__attribute__((packed));
static inline void mem_map_write(uint32_t i){
    volatile uint8_t *map_read_pointer = (volatile uint8_t *)(MEM_MAP_BASE + (i / 8));
    uint8_t map_byte = 
        (1 << (i % 8))       ^
        *map_read_pointer;
    *map_read_pointer = map_byte;
    return;
}
static inline uint32_t get_available_page(){
    uint32_t ret_val = 0xFFFFFFFF;
    for(uint32_t i = 0;i < 32768;i++){
        volatile uint32_t *mem_map_ptr = (volatile uint32_t *)(MEM_MAP_BASE + i * 4);
        uint32_t map_section = *mem_map_ptr;
        for(uint32_t j = 0;j < 32;j++){
            uint32_t page_used_flag = (map_section >> j) & 0x00000001;
            if(page_used_flag == 0){
                ret_val = i * 32 + j;
            }
        }
    }
    return ret_val;
}
static inline void creat_task(){
    /*uint32_t pd_base = get_available_page();
    mem_map_write(pd_base);
    pd_base = pd_base * 0x1000;
    uint32_t pt_base = get_available_page();
    mem_map_write(pt_base);
    pt_base = pt_base * 0x1000;
    uint32_t task_base = get_available_page();
    mem_map_write(task_base);
    PDE_SET((task_base / 1024),KERNEL_PD_PHY_ADDR,TEMP_PT_BASE,P_STSTEM,P_READ_WRITE,P_PRESENT);
    for (int i = 0; i < 1024; i++)PTE_SET(i,TEMP_PT_BASE,((task_base / 1024) * 0x400000 + i * 0x1000),P_STSTEM,P_READ_WRITE,P_PRESENT);
    PDE_SET((task_base / 1024),pd_base,pt_base,P_USER,P_ONLY_READ,P_PRESENT);// Task Page Directory Entry
    PTE_SET((task_base % 1024),pt_base,(task_base * 0x1000),P_USER,P_ONLY_READ,P_PRESENT);
    task_base = task_base * 0x1000;
    PDE_SET(0,pd_base,KERNEL_PT_PHY_ADDR,P_STSTEM,P_READ_WRITE,P_PRESENT);   // Kernel Page Directory Entry
    return;*/
    uint32_t pd_base = get_available_page();
    mem_map_write(pd_base);
    pd_base = pd_base * 0x1000;
    PDE_SET((pd_base / 1024),KERNEL_PD_PHY_ADDR,TEMP_PT_BASE,P_STSTEM,P_READ_WRITE,P_PRESENT);
    for (int i = 0; i < 1024; i++)PTE_SET(i,TEMP_PT_BASE,((pd_base / 1024) * 0x400000 + i * 0x1000),P_STSTEM,P_READ_WRITE,P_PRESENT);
    uint32_t pt_base = get_available_page();
    mem_map_write(pt_base);
    pt_base = pt_base * 0x1000;
    PDE_SET(0,pd_base,pt_base,P_STSTEM,P_READ_WRITE,P_PRESENT);
    PDE_SET((pd_base / 1024),pd_base,pt_base,P_USER,P_ONLY_READ,P_PRESENT);

    PDE_SET((pt_base / 1024),KERNEL_PD_PHY_ADDR,TEMP_PT_BASE,P_STSTEM,P_READ_WRITE,P_PRESENT);
    for (int i = 0; i < 1024; i++)PTE_SET(i,TEMP_PT_BASE,((pt_base / 1024) * 0x400000 + i * 0x1000),P_STSTEM,P_READ_WRITE,P_PRESENT);
    uint32_t task_base = get_available_page();
    mem_map_write(task_base);
    task_base = task_base * 0x1000;
    PTE_SET((task_base % 0x400000)/4,pt_base,task_base,P_USER,P_ONLY_READ,P_PRESENT);
    
}

#endif