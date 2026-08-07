#ifndef      BIT_H
#define      BIT_H
#include <stdint.h>
static __attribute__((optimize("Ofast","no-inline"))) uint32_t rotr(uint32_t x, int n) {
    return (x >> n) | (x << (32 - n));
}

#endif