#ifndef      IEEE1394_H
#define      IEEE1394_H

#include <device/PCI.h>
static inline void PCI_IEEE1394_ENU(){
    println("[IEEE1394] IEEE1394 starts enumerating devices");
    if(IEEE1394_device[0] == 0xFFFFFFFF){
        println("[IEEE1394] Not Found Any IEEE1394 Controller!!!");
        return;
    }
    for(int i = 0;i < 4;i++){
        if(IEEE1394_device[i] == 0xFFFFFFFF)break;
        printf("[IEEE1394] Enumerating IEEE1394 Contraller ");
        dec_digit(i);
        println(&num_string[0]);
        uint8_t pcibus = device[IEEE1394_device[i]].Bus;
        uint8_t pcidev = device[IEEE1394_device[i]].Device;
        uint8_t pcifun = device[IEEE1394_device[i]].Function;
        printf("[IEEE1394] IEEE1394 Contraller Located in ");
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