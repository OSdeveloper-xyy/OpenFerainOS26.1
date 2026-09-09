#ifndef      USB_H
#define      USB_H

#include <device/PCI.h>
static inline void PCI_USB_ENU(){
    println("[USB] USB starts enumerating devices");
    if(usb_device[0] == 0xFFFFFFFF){
        println("[USB] Not Found Any USB Control!!!");
        return;
    }
    for(int i = 0;i < 8;i++){
        if(usb_device[i] == 0xFFFFFFFF)break;
        printf("[USB] Enumerating USB Contraller ");
        dec_digit(i);
        println(&num_string[0]);
        uint8_t pcibus = device[usb_device[i]].Bus;
        uint8_t pcidev = device[usb_device[i]].Device;
        uint8_t pcifun = device[usb_device[i]].Function;
        printf("[USB] USB Contraller Located in ");
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
static inline void USB_write(uint32_t dev_sel,uint32_t data){

}

#endif