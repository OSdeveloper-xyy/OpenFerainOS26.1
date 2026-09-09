#ifndef          PCI_H
#define          PCI_H

#include <asm/basic.h>
#include <stdint.h>
#include <stdio.h>
#include <bit.h>

#define PCIE_DEVICE_MAX     1024
#define PCIE_CONF_MEM_BASE  0x00050000

#define CLASS_USB           0x0C0300 
#define CLASS_IDE           0x010100 
#define CLASS_SATA_BUS      0x010600
#define CLASS_ISA_BRIDGE    0x060100 
#define CLASS_PCI_BRIDGE    0x060400
#define CLASS_IEEE1394      0x0C0000 
#define CLASS_SD_HOST       0x080500

struct pcie_device{
    uint8_t   Bus;
    uint8_t   Device;
    uint8_t   Function;
    uint16_t  VendorID;
    uint32_t  ClassCode;
    uint8_t   HeaderType;
    uint32_t  Conf_Base0;
    uint32_t  Conf_Base1;
    uint32_t  Conf_Base2;
    uint32_t  Conf_Base3;
    uint32_t  Conf_Base4;
    uint32_t  Conf_Base5;
};
struct pcie_device device[PCIE_DEVICE_MAX];
uint32_t pcie_conf_mem_base = PCIE_CONF_MEM_BASE;
uint16_t pcie_conf_io_base = 0x1000;
uint8_t device_count[8]= {0};
uint32_t ISA_device[4] = {0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF};
uint32_t IDE_device[4] = {0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF};
uint32_t IEEE1394_device[4] = {0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF};
uint32_t SD_device[4] = {0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF};
uint32_t usb_device[8] = {
    0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,
    0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF
};
uint32_t SATA_device[8] = {
    0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,
    0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF
};
uint32_t PCI_device[16] = {
    0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,
    0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,
    0xFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,
    0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF,0XFFFFFFFF
};
uint8_t continue_flag = 0;
static inline uint32_t Convert_to_32data(uint8_t bus,uint8_t dev,uint8_t function,uint8_t reg){
    uint32_t ret_val =
        (1 << 31)                     |
        (bus << 16)                   |
        ((dev & 0x1F) << 11)          |
        ((function & 0x7) << 8)       |
        ((reg & 0x3F) << 2);
    return ret_val;
}
static inline uint32_t conf_bar(uint8_t bus,uint8_t dev,uint8_t function,uint32_t select){
    if(continue_flag = 1)return 0;
    uint32_t bar_conf_size;
    uint32_t sign_conf_base = 0;
    outpd(0xCF8,Convert_to_32data(bus,dev,function,(4 + select)));
    uint32_t Bar = inpd(0xCFC);
    outpd(0xCF8,Convert_to_32data(bus,dev,function,(4 + select)));
    outpd(0xCFC,0xFFFFFFFF);
    outpd(0xCF8, Convert_to_32data(bus,dev,function,(4 + select)));
    uint32_t size_mask = inpd(0xCFC);
    outpd(0xCF8, Convert_to_32data(bus,dev,function,(4 + select)));
    outpd(0xCFC,Bar);
    if((Bar & 7) == 0x0){
        size_mask &= ~0xF;
        bar_conf_size = (~size_mask) + 1;

        pcie_conf_mem_base = (pcie_conf_mem_base + 0xF) & ~0xF;
        sign_conf_base = pcie_conf_mem_base;
        pcie_conf_mem_base += bar_conf_size;

        uint32_t flags = Bar & 0xF;
        uint32_t sign_conf_mem_base = (sign_conf_base & ~0xF) | flags;
        outpd(0xCF8, Convert_to_32data(bus,dev,function,(4 + select)));
        outpd(0xCFC,sign_conf_mem_base);

        outpd(0xCF8,Convert_to_32data(bus,dev,function,1));
        uint32_t cmd = inpd(0xCFC);
        cmd |= (1 << 1) | (1 << 2);
        outpd(0xCF8,Convert_to_32data(bus,dev,function,1));
        outpd(0xCFC,cmd);
    }else if((Bar & 1) == 0x1){
        size_mask &= ~0x3;
        bar_conf_size = (~size_mask) + 1;
            
        pcie_conf_io_base = (pcie_conf_io_base + 0x3) & ~0x3;
        sign_conf_base = pcie_conf_io_base;
        pcie_conf_io_base += bar_conf_size;
            
        uint32_t flags = Bar & 0x1;
        uint32_t sign_conf_io_base = (sign_conf_base & ~0x3) | flags;
        outpd(0xCF8, Convert_to_32data(bus,dev,function,(4 + select)));
        outpd(0xCFC,sign_conf_io_base);
        outpd(0xCF8,Convert_to_32data(bus,dev,function,1));
        uint32_t cmd = inpd(0xCFC);
        cmd |= (1 << 0) | (1 << 2);
        outpd(0xCF8,Convert_to_32data(bus,dev,function,1));
        outpd(0xCFC, cmd);
    }else if((Bar & 7) == 0x4){
        outpd(0xCF8,Convert_to_32data(bus,dev,function,(4 + select + 1)));
        uint32_t BarH = inpd(0xCFC);
        outpd(0xCF8,Convert_to_32data(bus,dev,function,(4 + select + 1)));
        outpd(0xCFC,0xFFFFFFFF);
        uint32_t size_maskH = inpd(0xCFC);
        outpd(0xCF8,Convert_to_32data(bus,dev,function,(4 + select + 1)));
        outpd(0xCFC,BarH);

        uint64_t size64 = (~(((uint64_t)size_maskH << 32) | (size_mask & ~0xF))) + 1;
        bar_conf_size = (uint32_t)size64;

        pcie_conf_mem_base = (pcie_conf_mem_base + 0xF) & ~0xF;
        sign_conf_base = pcie_conf_mem_base;
        pcie_conf_mem_base += bar_conf_size;

        uint32_t flags = Bar & 0xF;
        uint32_t sign_conf_mem_base = (sign_conf_base & ~0xF) | flags;
        outpd(0xCF8, Convert_to_32data(bus,dev,function,(4 + select)));
        outpd(0xCFC,sign_conf_mem_base);

        outpd(0xCF8, Convert_to_32data(bus,dev,function,(4 + select + 1)));
        outpd(0xCFC,0);

        outpd(0xCF8,Convert_to_32data(bus,dev,function,1));
        uint32_t cmd = inpd(0xCFC);
        cmd |= (1 << 1) | (1 << 2);
        outpd(0xCF8,Convert_to_32data(bus,dev,function,1));
        outpd(0xCFC,cmd);
        continue_flag = 1;
    }
    return sign_conf_base;
}
static inline void PCI_Enumeration(){
    println("[PCI] PCI starts enumerating devices");
    uint32_t ans = 0;
    for(int i = 0;i < 256;i++){
        for(int j = 0;j < 32;j++){
            for(int k = 0;k < 8;k++){
                outpd(0xCF8,Convert_to_32data(i,j,k,0));
                uint16_t vendor = (uint16_t)inpd(0xCFC);
                if(vendor != 0xFFFF){
                    if(ans >= PCIE_DEVICE_MAX)return;
                    device[ans].Bus        = i;
                    device[ans].Device     = j;
                    device[ans].Function   = k;
                    device[ans].VendorID   = vendor;
                    outpd(0xCF8,Convert_to_32data(i,j,k,2));
                    device[ans].ClassCode  = (inpd(0xCFC) >> 8);
                    outpd(0xCF8,Convert_to_32data(i,j,k,3));
                    device[ans].HeaderType = (uint8_t)(inpd(0xCFC) >> 16);
                    outpd(0xCF8,Convert_to_32data(i,j,k,4));
                    for(uint32_t l = 0;l < 6;l++){
                        if(l = 0)device[ans].Conf_Base0 = conf_bar(i,j,k,l);
                        if(l = 1)device[ans].Conf_Base1 = conf_bar(i,j,k,l);
                        if(l = 2)device[ans].Conf_Base2 = conf_bar(i,j,k,l);
                        if(l = 3)device[ans].Conf_Base3 = conf_bar(i,j,k,l);
                        if(l = 4)device[ans].Conf_Base4 = conf_bar(i,j,k,l);
                        if(l = 5)device[ans].Conf_Base5 = conf_bar(i,j,k,l);
                    }
                    printf("[PCI] Found Device -- ");
                    printf("Bus : ");
                    dec_digit(i);
                    printf(&num_string[0]);
                    printf(" ; Device : ");
                    dec_digit(j);
                    printf(&num_string[0]);
                    printf(" ; Function : ");
                    dec_digit(k);
                    println(&num_string[0]);

                    if((device[ans].ClassCode & 0xFFFF00) == CLASS_PCI_BRIDGE){
                        println("[PCI] Device is PCI Bridge!");
                        if(device_count[0] <= 16){
                            PCI_device[device_count[0]] = ans;
                            device_count[0] += 1;
                        }
                    }else if((device[ans].ClassCode & 0xFFFF00) == CLASS_USB){
                        println("[PCI] Device is USB Controller!");
                        if(device_count[1] <= 8){
                            usb_device[device_count[1]] = ans;
                            device_count[1] += 1;
                        }
                    }else if((device[ans].ClassCode & 0xFFFF00) == CLASS_SATA_BUS){
                        println("[PCI] Device is SATA Bus");
                        if(device_count[2] <= 8){
                            SATA_device[device_count[2]] = ans;
                            device_count[2] += 1;
                        }
                    }else if((device[ans].ClassCode & 0xFFFF00) == CLASS_IDE){
                        println("[PCI] Device is IDE Controller!");
                        if(device_count[3] <= 4){
                            IDE_device[device_count[3]] = ans;
                            device_count[3] += 1;
                        }
                    }else if((device[ans].ClassCode & 0xFFFF00) == CLASS_ISA_BRIDGE){
                        println("[PCI] Device is ISA Bridge!");
                        if(device_count[4] <= 4){
                            ISA_device[device_count[4]] = ans;
                            device_count[4] += 1;
                        }
                    }else if((device[ans].ClassCode & 0xFFFF00) == CLASS_IEEE1394){
                        println("[PCI] Device is IEEE1394 Controller!");
                        if(device_count[5] <= 4){
                            IEEE1394_device[device_count[5]] = ans;
                            device_count[5] += 1;
                        }
                    }else if((device[ans].ClassCode & 0xFFFF00) == CLASS_SD_HOST){
                        println("[PCI] Device is SD Host Controller!");
                        if(device_count[6] <= 4){
                            SD_device[device_count[6]] = ans;
                            device_count[6] += 1;
                        }
                    }

                    ans += 1;
                }
            }
        }
    }
    return;
}
#endif