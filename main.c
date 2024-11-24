#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"

int main(void){
    startGraphics();

    unsigned short *p_counter = (unsigned short*) 0x6010;
    short startTime = *p_counter;

#if 1

    fillCircle(10, 10, 7, true);

    drawLine(10, 10, 40, 50, true);
    horzLine(10, 50, 10, true);

    rectangle(8, 35, 20, 50, true);

    circle(50, 35, 10, true);

    vertLine(30, 0, 20, true);
    vertLine(60, 0, 20, true);

    fillRectangle(55, 10, 70, 30, true);

    drawLine(55, 10, 70, 30, false);
    drawLine(70, 10, 55, 30, false);

    vertLine(25, 35, 45, true);

    //drawText(60, 50, "hai", 3, true, YELLOWTEXT);
    //drawText(47, 35, "Mooi", 4, false, BLUETEXT);
    for (unsigned char l = 0; l < 40; l++){
        for (unsigned char i = 2; i < 40; i++){
            rollDown(i, 1, 10);
        }
    }

#endif

#if 0
    struct sprite *testSprite = mallocSprite(5, 5);

    getSprite(testSprite, 5, 5);
    drawSprite(testSprite, 9, 5);

    freeSprite(testSprite) ;

#endif
#if 0

    for (unsigned char x = 2; x < 75; x++)
        for (unsigned char y = 0; y < 60; y++)
            setPixel(x, y, true);

#endif

    short endTime = *p_counter;

    sprintf(vidmem + 1840, "done in %d * 0.02 secs", (endTime - startTime));

    return 0;
}