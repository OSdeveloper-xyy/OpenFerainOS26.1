#ifndef      TASK_H
#define      TASK_H

#define MEM_MAP_BASE         0x00020000

static inline void mem_map_write(uint32_t i){
    volatile uint8_t *map_read_pointer = (volatile uint8_t *)(MEM_MAP_BASE + (i / 8));
    uint8_t map_byte = 
        (1 << (i % 8))       ^
        *map_read_pointer;
    *map_read_pointer = map_byte;
    return;
}

#endif