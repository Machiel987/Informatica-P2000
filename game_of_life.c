#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#ifndef FASTCALL
#define FASTCALL
#endif

//Table including the "pop count" of a giving number (byte).
//The pop count is defined as the number of 1s in a number
//This has been hard-coded for better performance
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

//Draws a given board state, where adr is the adress of the position on screen where the board will be drawn
void drawBoard(unsigned char* adr, unsigned char* boardStatus, unsigned char w, unsigned char h){
    for (unsigned char j = 0; j < h; j++){ //j = y-coordinate
        for (unsigned char i = 0; i < w; i++){ //i = x-coordinate
            adr[80*j + i] = boardStatus[w*j + i];
        }
    }
}

#if 0
//Legacy algorithm for computing next board state
void evolveBoard(unsigned char* adr, unsigned char* out, unsigned char w, unsigned char h, unsigned char stopChar){
    memset(out, 0x20, w*h);

    for (unsigned char j = 0; j < h; j++){

        if (getKey() == stopChar) break;

        for (unsigned char i = 0; i < w; i++){
            register unsigned char circum = 0;
            register unsigned char val = *adr;

            circum =  popCount[(adr[-81] & 0b01000000) | (adr[-1] & 0b00001010)];
            circum += popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001110)];
            if (((val & pxNumToChar0) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar0;
        
            circum =  popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001101)];
            circum += popCount[(adr[-79] & 0b00010000) | (adr[1] & 0b00000101)];
            if (((val & pxNumToChar1) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar1;

            circum =  popCount[adr[-1] & 0b01001010];
            circum += popCount[adr[0]  & 0b01011011];
            if (((val & pxNumToChar2) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar2;

            circum =  popCount[adr[0] & 0b01010111];
            circum += popCount[adr[1] & 0b00010101];
            if (((val & pxNumToChar3) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar3;

            circum =  popCount[(adr[-1] & 0b01001000) | (adr[79] & 0b00000010)];
            circum += popCount[(adr[0]  & 0b01001100) | (adr[80] & 0b00000011)];
            if (((val & pxNumToChar4) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar4;

            circum =  popCount[(adr[0] & 0b00011100) | (adr[80] & 0b00000011)];
            circum += popCount[(adr[1] & 0b00010100) | (adr[81] & 0b00000001)];
            if (((val & pxNumToChar[5]) && (circum == 2)) || (circum == 3))
                *out |= pxNumToChar[5]; //For some unknown reason, using the array for one pixel number is faster

            adr++;
            out++;
        }

        adr += 80 - w;
    }
}
#endif

unsigned char runByte(unsigned char* adr) __sdcccall(1);

#if 0
unsigned char runByte(unsigned char* adr) FASTCALL {
    unsigned char circum = 0;
    unsigned char val = *adr;
    unsigned char out = 0x20;

    circum =  popCount[(adr[-81] & 0b01000000) | (adr[-1] & 0b00001010)];
    circum += popCount[(adr[-80] & 0b01010000) | (adr[ 0] & 0b00001110)];
    if (((val & pxNumToChar0) && (circum == 2)) || (circum == 3))
        out |= pxNumToChar0;

    circum =  popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001101)];
    circum += popCount[(adr[-79] & 0b00010000) | (adr[1] & 0b00000101)];
    if (((val & pxNumToChar1) && (circum == 2)) || (circum == 3))
        out |= pxNumToChar1;

    circum =  popCount[adr[-1] & 0b01001010];
    circum += popCount[adr[ 0] & 0b01011011];
    if (((val & pxNumToChar2) && (circum == 2)) || (circum == 3))
        out |= pxNumToChar2;

    circum =  popCount[adr[0] & 0b01010111];
    circum += popCount[adr[1] & 0b00010101];
    if (((val & pxNumToChar3) && (circum == 2)) || (circum == 3))
        out |= pxNumToChar3;

    circum =  popCount[(adr[-1] & 0b01001000) | (adr[79] & 0b00000010)];
    circum += popCount[(adr[ 0] & 0b01001100) | (adr[80] & 0b00000011)];
    if (((val & pxNumToChar4) && (circum == 2)) || (circum == 3))
        out |= pxNumToChar4;

    circum =  popCount[(adr[0] & 0b00011100) | (adr[80] & 0b00000011)];
    circum += popCount[(adr[1] & 0b00010100) | (adr[81] & 0b00000001)];
    if (((val & pxNumToChar5) && (circum == 2)) || (circum == 3))
        out |= pxNumToChar5;

    return out;
}
#endif

//New function for computing the next board state. The current implemantation uses a "change list" to keep track of changing bytes
//And only aplies runByte to bytes that have just changed, or neighbor a changing byte
//This could be optimised further by using an algorithm which knows beforehand which bytes can be left unchecked, (i.e. using a quadtree)

unsigned char evolve_w;
unsigned char evolve_h;
unsigned char* evolve_changeListIn;
unsigned char* evolve_changeListOut;

void evolveBoard(unsigned char* adr, unsigned char* out) __sdcccall(1);

#if 0
void evolveBoard(unsigned char* adr, unsigned char* out) __sdcccall(1) {
    for (unsigned char j = 0; j < evolve_h; j++){
        for (unsigned char i = 0; i < evolve_w; i++){
            if (*evolve_changeListIn == 0)
                goto done;

            unsigned char newByte = runByte(adr);
            *out = newByte;

            if (newByte == *adr)
                goto done;

            /*
            unsigned char change = newByte ^ *adr;

            if (change & 0b00000001) changeListOut[-w - 2 - 1] = 1;//|= 0x40;
            if (change & 0b00000011) changeListOut[-w - 2] = 1;//|= 0x50;
            if (change & 0b00000010) changeListOut[-w - 2 + 1] = 1;//|= 0x10;
            if (change & 0b00010101) changeListOut[-1] = 1;//|= 0x4A;
            if (change & 0b01011111) changeListOut[0] = 1;//|= 0x5F;
            if (change & 0b01001010) changeListOut[1] = 1;//|= 0x15;
            if (change & 0b00010000) changeListOut[w + 2 - 1] = 1;//|= 0x02;
            if (change & 0b01010000) changeListOut[w + 2] = 1;//|= 0x03;
            if (change & 0b01000000) changeListOut[w + 2 + 1] = 1;//|= 0x01;
            */

            register char *clo = evolve_changeListOut;
            unsigned char wp2 = evolve_w + 2;

            //The following code has been optimised for speed, not readability
            //To whoever is reading this 2-am code at a spectecular time of your life: My sincere apologies
            clo -= wp2;
            *(clo--) = 1;
            *(clo++) = 1;
            *(clo++) = 1;
            *(clo += wp2) = 1;
            *(clo--) = 1;
            *(clo--) = 1;
            *(clo += wp2) = 1;
            *(clo++) = 1;
            *(clo++) = 1;

            done:
            adr++;
            out++;
            evolve_changeListIn++;
            evolve_changeListOut++;
        }

        evolve_changeListIn += 2;
        evolve_changeListOut += 2;
        adr += 80 - evolve_w;
    }
}
#endif

#if 0
//Legacy function for running Game of Life algorithm
void runGOL(char* adr, unsigned char w, unsigned char h, unsigned char stopChar){
    unsigned char* newBoard = (unsigned char*) malloc(((unsigned) w) * ((unsigned) h));
    if (newBoard == NULL) return;

    /*
    while (getKey() != stopChar){
        evolveBoard(adr, newBoard, w, h, stopChar);
        drawBoard(adr, newBoard, w, h);
    }
    */

    unsigned int startTime = getTime();

    for (unsigned char i = 0; i < 10 && getKey() != stopChar; i++){
        evolveBoard(adr, newBoard, w, h, stopChar);
        drawBoard(adr, newBoard, w, h);
    }

    sprintf(vidmem + 1840, "Time (10): %d", getTime() - startTime);

    free(newBoard);
}
#endif

//Run Game of Life on a specified adress.
//Note that in at
void runGOL(char* adr, unsigned char w, unsigned char h, unsigned char stopChar){
    unsigned int sizeBoard = ((unsigned) w) * ((unsigned) h);
    unsigned int sizeCList = ((unsigned) (w+2)) * ((unsigned) (h+2));

    unsigned char* board = (unsigned char*) malloc(sizeBoard);
    unsigned char* changeList = (unsigned char*) malloc(sizeCList);
    unsigned char* newChangeList = (unsigned char*) malloc(sizeCList);

    if (board == NULL || changeList == NULL || newChangeList == NULL) return;

    memset(changeList, 1/*0x5F*/, sizeCList);
    memset(newChangeList, 0, sizeCList);

    evolve_w = w;
    evolve_h = h;

    unsigned int startTime = getTime();

    for (unsigned char i = 0; i < 10 && getKey() != stopChar; i++){
        evolve_changeListIn = changeList + w + 1;
        evolve_changeListOut = newChangeList + w + 1;

        evolveBoard(adr, board);
        drawBoard(adr, board, w, h);

        unsigned char* temp = changeList;
        changeList = newChangeList;
        newChangeList = temp;
        memset(newChangeList, 0, sizeCList);
    }

    sprintf(vidmem + 1840, "t(10)=%d", getTime() - startTime);

    /*
    while (getKey() != stopChar){
        evolve_changeListIn = changeList + w + 1;
        evolve_changeListOut = newChangeList + w + 1;

        evolveBoard(adr, board);
        drawBoard(adr, board, w, h);

        unsigned char* temp = changeList;
        changeList = newChangeList;
        newChangeList = temp;
        memset(newChangeList, 0, sizeCList);
    }
    */

    free(board);
    free(changeList);
    free(newChangeList);
}

void drawIntroScene(void){
    drawText(10, 0, "Game of Life", true, WHITETEXT);

    horzLine(4, 77, 38, true);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    unsigned char* drawAdr = (unsigned char*) (vidmem + 3*80 + 2);
    runGOL(drawAdr, 37, 19, keyEnter);

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
    drawText(8, 45, "or >3 alive cells, it stays dead.", false, WHITETEXT);

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

//Main function for running the Game of Life
void gameOfLife(void){
    setWindow(2, 0, 79, 70);

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

    horzLine(10, 60, 35, true);

    /*
    setPixel(26, 26, true);
    setPixel(27, 26, true);
    setPixel(27, 25, true);
    setPixel(28, 25, true);
    setPixel(28, 27, true);
    */

    rectangle(2, 1, 79, 67, true);

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

    unsigned char* drawAdr = (unsigned char*) (vidmem + 80 + 2);

    sprintf(vidmem + 1840, "                    ");
    //sprintf(vidmem + 1840, "Press space to stop");

    runGOL(drawAdr, 35, 21, keySpace);

    //sprintf(vidmem + 1840, "Done! Press enter to run again");

    while (getKey() != keyEnter);

    goto startRun;

    //return 0;
}