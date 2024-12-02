#include "utils.h"

unsigned char getKey(void){
    return *(unsigned char*) 0x600D;
}

unsigned int getTime(void){
    return *(unsigned int*) 0x6010;
}