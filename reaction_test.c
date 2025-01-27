#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;

static void drawInfoScreen(void){
    drawText(10, 0, "Reaction test", true, WHITETEXT);

    drawText(4, 9,  "For almost every popular sport, a ", false, WHITETEXT);
    drawText(4, 12, "good reaction time is a requirement.", false, WHITETEXT);
    drawText(4, 15, "Therefore, your own reaction time", false, WHITETEXT);
    drawText(4, 18, "will now be put to the test.", false, WHITETEXT);
    drawText(4, 24, "When the screen turns white, press", false, WHITETEXT);
    drawText(4, 27, "any key as quickly as possible, and", false, WHITETEXT);
    drawText(4, 30, "you will get a score.", false, WHITETEXT);
    drawText(4, 33, "Good luck!", false, WHITETEXT);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void reactionTest(void){
    srand(getTime());

    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    drawInfoScreen();

    start:

    startGraphics(WHITEGFS);

    vidmem[800] = WHITETEXT;

    unsigned char countDown = 5;
    for (unsigned char i = countDown; i > 0; i--){
        sprintf(vidmem + 801, "STARTING IN %d", i);

        unsigned int startTime = getTime();
        while (getTime() - startTime < 50);
    }

    vidmem[800] = WHITEGFS;

    unsigned int startCount = 15000 + (rand() & 0xFFF);
    unsigned int counter = 0;

    while (counter < startCount) counter++;

    fillRectangle(windowTLX + 1, windowTLY + 1, windowBRX - 1, windowBRY - 2, true);

    unsigned int reactTime = 0;
    unsigned char startKey = getKey();

    while (getKey() == startKey) reactTime++;

    fillRectangle(windowTLX + 1, windowTLY + 1, windowBRX - 1, windowBRY - 2, false);

    vidmem[800] = WHITETEXT;

    sprintf(vidmem + 801, "Your score is: %d", reactTime);

    drawText(30, 67, "Press Space to restart", false, WHITETEXT);
    drawText(30, 70, "Press 0 to return to LCL", false, WHITETEXT);

    drawText(10, 0, "Results", true, WHITETEXT);

    unsigned char prevKey = getKey();
    while (getKey() == prevKey);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;

    while (true);
}