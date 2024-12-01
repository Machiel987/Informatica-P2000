#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"

#define keySpace 17
#define keyEnter 52

#define keyUp 2
#define keyRight 23
#define keyDown 21
#define keyLeft 0

unsigned char popCount[] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

unsigned char getKey(void){
    return *(unsigned char*) 0x600D;
}

unsigned int getTime(void){
    return *(unsigned int*) 0x6010;
}

void drawBoard(unsigned char* boardStatus, unsigned char* adr, unsigned char w, unsigned char h){
    for (unsigned char j = 0; j < h; j++){ //j = y-coordinate
        for (unsigned char i = 0; i < w; i++){ //i = x-coordinate
            adr[80*j + i] = boardStatus[w*j + i];
        }
    }
}

void evolveBoard(unsigned char* adr, unsigned char* out, unsigned char w, unsigned char h, unsigned char stopChar){
    memset(out, 0x20, w*h);

    for (unsigned char j = 0; j < h; j++){

        if (getKey() == stopChar);

        for (unsigned char i = 0; i < w; i++){
            register unsigned char circum = 0;
            unsigned char val = *adr;

            circum =  popCount[(adr[-81] & 0b01000000) | (adr[-1] & 0b00001010)];
            circum += popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001110)];
            if (((val & pxNumToChar0) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar0;
        
            circum =  popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001101)];
            circum += popCount[(adr[-79] & 0b00010000) | (adr[1] & 0b00000101)];
            if (((val & pxNumToChar1) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar1;

            circum =  popCount[adr[-1] & 0b01001010];
            circum += popCount[adr[0] & 0b01011011];
            if (((val & pxNumToChar2) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar2;

            circum =  popCount[adr[0] & 0b01010111];
            circum += popCount[adr[1] & 0b00010101];
            if (((val & pxNumToChar3) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar3;

            circum =  popCount[(adr[-1] & 0b01001000) | (adr[79] & 0b00000010)];
            circum += popCount[(adr[0] & 0b01001100) | (adr[80] & 0b00000011)];
            if (((val & pxNumToChar4) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar4;

            circum =  popCount[(adr[0] & 0b00011100) | (adr[80] & 0b00000011)];
            circum += popCount[(adr[1] & 0b00010100) | (adr[81] & 0b00000001)];
            if (((val & pxNumToChar[5]) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar[5];

            adr++;
            out++;
        }

        adr += 80 - w;
    }
}

void runGOT(char* adr, unsigned char w, unsigned char h, unsigned char stopChar){
    unsigned char* newBoard = (unsigned char*) malloc(((unsigned) w) * ((unsigned) h));
    if (newBoard == NULL) return;

    while (getKey() != stopChar){
        evolveBoard(adr, newBoard, w, h, stopChar);
        drawBoard(newBoard, adr, w, h);
    }

    free(newBoard);
}

void drawIntroScene(void){
    drawText(10, 0, "Game of Life", true, WHITETEXT);

    horzLine(4, 77, 38, true);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    unsigned char* drawAdr = (unsigned char*) (vidmem + 3*80 + 2);
    runGOT(drawAdr, 37, 19, keyEnter);

    while (getKey() != keyEnter);
}

void drawInfoScene(void){
    while (getKey() == keyEnter);

    drawText(10, 0, "Game of Life", true, WHITETEXT);

    drawText(4, 9,  "The Game of Life, invented by", false, WHITETEXT);
    drawText(4, 12, "mathematician John Conway is a very", false, WHITETEXT);
    drawText(4, 15, "simple 0-player game played on a ", false, WHITETEXT);
    drawText(4, 18, "2D grid of cells with just 4 rules:", false, WHITETEXT);
    drawText(6, 24, "-If an alive cell is surrounded by <2", false, WHITETEXT);
    drawText(8, 27, "or >3 alive cells, the cell dies.", false, WHITETEXT);
    drawText(6, 30, "-If an alive cell is surrounded by 2", false, WHITETEXT);
    drawText(8, 33, "or 3 alive cells, the cell survives.", false, WHITETEXT);
    drawText(6, 36, "-If a dead cell is surrounded by 3", false, WHITETEXT);
    drawText(8, 39, "alive cells, the cell comes to life.", false, WHITETEXT);
    drawText(6, 42, "-If a dead cell is surrounded by <3,", false, WHITETEXT);
    drawText(8, 45, "or >3 alive cells, stays dead.", false, WHITETEXT);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void drawInstructionScene(void){
    while (getKey() == keyEnter);

    drawText(10, 0, "Game of Life", true, WHITETEXT);

    drawText(4, 9,  "Walk through the screen using", false, WHITETEXT);
    drawText(4, 12, "up, down, right and left arrows.", false, WHITETEXT);

    drawText(4, 18, "Use space to place/delete a pixel.", false, WHITETEXT);

    drawText(4, 24, "Press enter when you're ready to play!", false, WHITETEXT);

    drawText(4, 30, "Press space to stop the simulation.", false, WHITETEXT);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void gameOfLife(void){
    startGraphics();
    drawIntroScene();
    initializeScreen();
    drawInfoScene();
    initializeScreen();
    drawInstructionScene();

    startRun: 
    initializeScreen();

    sprintf(vidmem + 1840, "Press enter to start");

    while (getKey() == keyEnter);

    //setPixel(30, 30, true);
    //setPixel(31, 30, true);
    //setPixel(31, 29, true);
    //setPixel(32, 29, true);
    //setPixel(32, 31, true);

    vertLine(2, 1, 67, true);
    vertLine(79, 1, 67, true);
    horzLine(2, 79, 1, true);
    horzLine(2, 79, 67, true);

    unsigned char key = 255, prevKey = 255;
    unsigned char cursorPosX = 4, cursorPosY = 3;

    while (getKey() != keyEnter){
        unsigned char prevPixel = getPixel(cursorPosX, cursorPosY);
        setPixel(cursorPosX, cursorPosY, true);

        while (key == prevKey){
            unsigned int startTime = getTime();
            while (key == prevKey && getTime() - startTime < 30){
                key = getKey();
            }

            setPixel(cursorPosX, cursorPosY, !getPixel(cursorPosX, cursorPosY));
        }

        setPixel(cursorPosX, cursorPosY, prevPixel);
        prevKey = key;

        if ((key == keyUp) && cursorPosY > 3) cursorPosY--;
        if ((key == keyRight) && cursorPosX < 77) cursorPosX++;
        if ((key == keyDown) && cursorPosY < 65) cursorPosY++;
        if ((key == keyLeft) && cursorPosX > 4) cursorPosX--;

        if (key == keySpace){
            setPixel(cursorPosX, cursorPosY, !getPixel(cursorPosX, cursorPosY));
        }
    }

    unsigned char* drawAdr = (unsigned char*) (vidmem + 80 + 4);

    sprintf(vidmem + 1840, "Press space to stop");
    runGOT(drawAdr, 35, 21, keySpace);

    sprintf(vidmem + 1840, "Done! Press enter to run again");

    while (getKey() != keyEnter);

    goto startRun;

    //return 0;
}