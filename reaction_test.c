#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;

void reactionTest(void){
    srand(getTime());

    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics();

    unsigned int startCount = 5000 + (rand() & 0x7FF);
    //sprintf(vidmem + 1840, "Time: %i", startCount);
    unsigned int counter = 0;

    while (counter < startCount) counter++;

    fillRectangle(windowTLX + 1, windowTLY + 1, windowBRX - 1, windowBRY - 2, true);

    //horzLine(10, 50, 10, true);

    unsigned int reactTime = 0;

    while (getKey() == keyNone) reactTime++;

    sprintf(vidmem + 1840, "Done: %i", reactTime);

    while (true);
}