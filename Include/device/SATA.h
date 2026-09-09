#ifndef      SATA_H
#define      SATA_H

#include <device/PCI.h>
static inline void PCI_SATA_ENU(){
    println("[SATA] SATA starts enumerating devices");
    if(SATA_device[0] == 0xFFFFFFFF){
        println("[SATA] Not Found Any SATA Bus!!!");
        return;
    }
    for(int i = 0;i < 8;i++){
        if(SATA_device[i] == 0xFFFFFFFF)break;
        printf("[SATA] Enumerating SATA Bus ");
        dec_digit(i);
        println(&num_string[0]);
        uint8_t pcibus = device[SATA_device[i]].Bus;
        uint8_t pcidev = device[SATA_device[i]].Device;
        uint8_t pcifun = device[SATA_device[i]].Function;
        printf("[SATA] SATA Bus Located in ");
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