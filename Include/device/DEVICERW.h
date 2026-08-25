#ifndef          DEVICERW_H
#define          DEVICERW_H
#define HD_BF_BASE           0x00016000
#include     <device/PCI.h>
#include     <device/ATA.h>
#include     <device/ATAPI.h>
#include     <device/SCSI.h>
#include     <device/USB.h>
#include     <device/IEEE1394.h>
#include     <device/FIBRECHANNEL.h>
#include     <device/SATA.h>
static inline void ISR_DEVICE_READ(){
    return;
}
static inline void ISR_DEVICE_WRITE(){
    return;
}
static inline uint32_t DEVICE_READ(uint32_t FileOffset){
    uint32_t RetData;
    return RetData;
}
static inline void DEVICE_WRITE(uint32_t FileOffset,uint8_t Byte){
    return;
}
#endif