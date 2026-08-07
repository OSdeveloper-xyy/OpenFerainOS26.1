#ifndef      SHA2_H
#define      SHA2_H
#include <stdint.h>
#include <stdio.h>
#include <bit.h>
static const uint32_t H[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19 
};
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 
};
uint32_t a,b,c,d,e,f,g,h;
static uint32_t hash32[8] = {0};
static uint8_t hash8[65] = {0};
static uint32_t W[64] = {0};
static uint8_t raw_data[64] = {0};
static __attribute__((optimize("Ofast","no-inline"))) uint32_t sha256_IE(uint32_t W[],int i){
    uint32_t s1 = rotr(W[i-2],17) ^ rotr(W[i-2],19) ^ (W[i-2] >> 10);
    uint32_t s0 = rotr(W[i-15],7) ^ rotr(W[i-15],18) ^ (W[i-15] >> 3);
    uint32_t IEB = s1 + W[i-7] + s0 + W[i-16];
    return IEB;
}
static __attribute__((optimize("Ofast","no-inline"))) void sha256_ROC(uint32_t W[],int i){
    uint32_t S1    = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
    uint32_t ch = (e & f) ^ ((~(uint32_t)e) & g);
    uint32_t temp1 = h + S1 + ch + K[i] + W[i];
    uint32_t S2    = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
    uint32_t maj   = (a & b) ^ (a & c) ^ (b & c);
    uint32_t temp2 = S2 + maj;
    h = g;
    g      = f;
    f      = e;
    e      = d + temp1;
    d      = c;
    c      = b;
    b      = a;
    a      = temp1 + temp2;
    return;
}
static __attribute__((optimize("Ofast","no-inline"))) void sha256(const char s[]){
    a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];
    for(int i=0;i<8;i++) hash32[i] = 0;
    for(int i=0;i<64;i++){
        raw_data[i] = 0;
        W[i] = 0;
        hash8[i] = 0;
    }
    uint32_t len = 0;
    while (s[len])++len;
    if(len <= 55){
        for(int i = 0;i < len; i++){
            raw_data[i] = s[i];
        }
        raw_data[len] = 0x80;
        uint64_t bit_len = (uint64_t)len * 8;
        raw_data[56] = (bit_len >> 56) & 0xFF;
        raw_data[57] = (bit_len >> 48) & 0xFF;
        raw_data[58] = (bit_len >> 40) & 0xFF;
        raw_data[59] = (bit_len >> 32) & 0xFF;
        raw_data[60] = (bit_len >> 24) & 0xFF;
        raw_data[61] = (bit_len >> 16) & 0xFF;
        raw_data[62] = (bit_len >> 8)  & 0xFF;
        raw_data[63] = bit_len & 0xFF;
        for (int i = 0; i < 64; i++) {
            if(i <16){
                W[i] = ((uint32_t)raw_data[i*4 + 0] << 24) |
                    ((uint32_t)raw_data[i*4 + 1] << 16) |
                    ((uint32_t)raw_data[i*4 + 2] << 8)  |
                    ((uint32_t)raw_data[i*4 + 3]);
            }else{
                W[i] = sha256_IE(W,i);
            }
            sha256_ROC(W,i);
        }
    }
    a += H[0],b += H[1],c += H[2],d += H[3],e += H[4],f += H[5],g += H[6],h += H[7];
    hash32[0] = a,hash32[1] = b,hash32[2] = c,hash32[3] = d,hash32[4] = e,hash32[5] = f,hash32[6] = g,hash32[7] = h;
    for (int i = 0; i < 32; i++) {
            uint8_t byte = (uint8_t)(hash32[i / 4] >> (24 - 8 * (i % 4)));
            hash8[i * 2]     = byte >> 4;
            hash8[i * 2 + 1] = byte & 0x0F;
    }
    for (int i = 0; i < 64; i++) {
            hash8[i] = (uint8_t)hex_to_char(hash8[i]);
    }
    hash8[64] = 0x00;
    printf(hash8);
    return;
}
#endif