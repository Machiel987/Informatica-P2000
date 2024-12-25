#include "utils.h"

unsigned int getTime(void){
    return *(unsigned int*) 0x6010;
}