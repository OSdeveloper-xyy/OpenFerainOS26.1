#ifndef      IDE_H
#define      IDE_H

#include <device/PCI.h>
static inline void PCI_IDE_ENU(){
    println("[IDE] IDE starts enumerating devices");
    if(IDE_device[0] == 0xFFFFFFFF){
        println("[IDE] Not Found Any IDE Controller!!!");
        return;
    }
    for(int i = 0;i < 4;i++){
        if(IDE_device[i] == 0xFFFFFFFF)break;
        printf("[IDE] Enumerating IDE Contraller ");
        dec_digit(i);
        println(&num_string[0]);
        uint8_t pcibus = device[IDE_device[i]].Bus;
        uint8_t pcidev = device[IDE_device[i]].Device;
        uint8_t pcifun = device[IDE_device[i]].Function;
        printf("[IDE] IDE Contraller Located in ");
        printf(" Bus : ");
        dec_digit(pcibus);
        printf(&num_string[0]);
        printf(" Device : ");
        dec_digit(pcidev);
        printf(&num_string[0]);
        printf(" Function : ");
        dec_digit(pcifun);
        println(&num_string[0]);
    }
}
#endif