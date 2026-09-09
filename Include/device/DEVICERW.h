#ifndef          DEVICERW_H
#define          DEVICERW_H
#define HD_BF_BASE           0x00016000
#define DeviceMax            32768
#include     <device/PCI.h>
#include     <device/IDE.h>
#include     <device/ISA.h>
#include     <device/USB.h>
#include     <device/IEEE1394.h>
#include     <device/SATA.h>
#include     <device/SDHOST.h>
struct DriveStruct{
    uint8_t  BusType[4];
    uint32_t ClassCode[4];
    char     DriveNote[16];
};
struct DeviceStruct{
    uint8_t  BusType;
    uint8_t  DeviceType;
    uint8_t  IOflag;
    uint8_t  Function;
    char     DeviceNote[16];
    uint32_t DeviceSpace;
};
struct DeviceStruct Device[DeviceMax];
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