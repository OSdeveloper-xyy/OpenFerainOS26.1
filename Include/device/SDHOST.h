#ifndef      SDHOST_H
#define      SDHOST_H

#include <device/PCI.h>
static inline void PCI_SDHOST_ENU(){
    println("[SDHOST] SDHOST starts enumerating devices");
    if(SD_device[0] == 0xFFFFFFFF){
        println("[SDHOST] Not Found Any SDHOST Controller!!!");
        return;
    }
    for(int i = 0;i < 8;i++){
        if(SD_device[i] == 0xFFFFFFFF)break;
        printf("[SDHOST] Enumerating SDHOST Contraller ");
        dec_digit(i);
        println(&num_string[0]);
        uint8_t pcibus = device[SD_device[i]].Bus;
        uint8_t pcidev = device[SD_device[i]].Device;
        uint8_t pcifun = device[SD_device[i]].Function;
        printf("[SDHOST] SDHOST Contraller Located in ");
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