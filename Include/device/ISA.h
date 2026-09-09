#ifndef      ISA_H
#define      ISA_H

#include <device/PCI.h>
static inline void PCI_ISA_ENU(){
    println("[ISA] ISA starts enumerating devices");
    if(ISA_device[0] == 0xFFFFFFFF){
        println("[ISA] Not Found Any ISA Bridge!!!");
        return;
    }
    for(int i = 0;i < 4;i++){
        if(ISA_device[i] == 0xFFFFFFFF)break;
        printf("[ISA] Enumerating ISA Bridge ");
        dec_digit(i);
        println(&num_string[0]);
        uint8_t pcibus = device[ISA_device[i]].Bus;
        uint8_t pcidev = device[ISA_device[i]].Device;
        uint8_t pcifun = device[ISA_device[i]].Function;
        printf("[ISA] ISA Bridge Located in ");
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