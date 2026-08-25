#ifndef      BIT_H
#define      BIT_H
#include <stdint.h>
static uint32_t rotr(uint32_t x, int n) {
    return (x >> n) | (x << (32 - n));
}
static uint16_t char_to_hex(char ch){
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    return 0x1000;
}
static uint16_t hex_to_char(uint8_t hex){
    if (hex >= 0 && hex <= 9) return hex + 0x30;
    if (hex >= 0xA && hex <= 0xF) return hex + 0x57;
    return 0xFF3F;
}
char num_string[12] = {0};
static void dec_digit(uint32_t num){
    uint32_t length = 0,num1 = num;
    for(int i = 0;i < 12;i++){
        num_string[i] = 0x00;
    }
    while(1){
        num1 /= 10;
        length++;
        if(num1 == 0)break;
    }
    for(int i = length;i > 0;i--){
        num_string[i - 1] = (char)((num % 10) + 0x30);
        num /= 10;
        if(num == 0)break;
    }
    return;
}
#endif