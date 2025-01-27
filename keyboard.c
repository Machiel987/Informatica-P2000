#include "keyboard.h"

//static char niceKeys[] = 

unsigned char getKey(void){
    return *(unsigned char*) 0x600D;
}

unsigned char getNiceKeyL(void){
    switch (getKey())
    {
    case keyA: return 'a';
    case keyB: return 'b';
    case keyC: return 'c';
    case keyD: return 'd';
    case keyE: return 'e';
    case keyF: return 'f';
    case keyG: return 'g';
    case keyH: return 'h';
    case keyI: return 'i';
    case keyJ: return 'j';
    case keyK: return 'k';
    case keyL: return 'l';
    case keyM: return 'm';
    case keyN: return 'n';
    case keyO: return 'o';
    case keyP: return 'p';
    case keyQ: return 'q';
    case keyR: return 'r';
    case keyS: return 's';
    case keyT: return 't';
    case keyU: return 'u';
    case keyV: return 'v';
    case keyW: return 'w';
    case keyX: return 'x';
    case keyY: return 'y';
    case keyZ: return 'z';
    case keySpace: return ' ';
    case keyDot: return '.';
    case keyComma: return ',';
    case keyMinus: return '-';

    default: return '\0';
    }
}