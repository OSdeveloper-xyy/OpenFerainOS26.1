#ifndef      MATH_H
#define      MATH_H
int pow(int num,int exponent){
    int base = num;
    while(exponent > 1){
        num *= base;
        exponent--;
    }
    return num;
}
int abs(int num){
    if(num >= 0)return num;
    else return ((~num) + 1);
}
#endif