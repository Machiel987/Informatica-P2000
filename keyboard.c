#include "keyboard.h"

unsigned char getKey(void){
    return *(unsigned char*) 0x600D;
}