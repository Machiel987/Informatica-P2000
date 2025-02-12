#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

int main(void){
    //*(unsigned char*) 0x60AD = 10;

    //gameOfLife();
    //walker();
    //tetris();
    //reactionTest();
    //gameOfUr();
    //countries();
    //typing();
    //hangman();
    maze();

    //*(unsigned char*) 0x5000 = 0b01111111;

    while(1);

#if 0
    unsigned char prevKey = 0;

    while (true){
        while (getKey() == prevKey);

        prevKey = getKey();

        sprintf(vidmem + 1840, "   ");
        sprintf(vidmem + 1840, "%d", getKey());
    }
#endif

    return 0;
}
