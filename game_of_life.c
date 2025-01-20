#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

//Table including the "pop count" of a giving number (byte).
//The pop count is defined as the number of 1s in a number
//This has been hard-coded for better performance
unsigned char popCountTable[] = {
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

unsigned char *popCount;
unsigned char popCountHigh;

unsigned char evolve_w;
unsigned char evolve_h;
unsigned char* evolve_changeListIn;
unsigned char* evolve_changeListOut;

//Draws a given board state, where adr is the adress of the position on screen where the board will be drawn
void drawBoard(unsigned char* adr, unsigned char* boardStatus) __sdcccall(1);

//New function for computing the next board state. The current implemantation uses a "change list" to keep track of changing bytes
//And only aplies runByte to bytes that have just changed, or neighbor a changing byte
//This could be optimised further by using an algorithm which knows beforehand which bytes can be left unchecked, (i.e. using a quadtree)
void evolveBoard(unsigned char* adr, unsigned char* out) __sdcccall(1);

//Run Game of Life on a specified adress.
void runGOL(char* adr, unsigned char w, unsigned char h, unsigned char stopChar){
    unsigned int sizeBoard = ((unsigned) w) * ((unsigned) h);
    unsigned int sizeCList = ((unsigned) (w+2)) * ((unsigned) (h+2));

    unsigned char* board = (unsigned char*) malloc(sizeBoard);
    unsigned char* changeList = (unsigned char*) malloc(sizeCList);
    unsigned char* newChangeList = (unsigned char*) malloc(sizeCList);

    if (board == NULL || changeList == NULL || newChangeList == NULL) return;

    memset(changeList, 1 /*0x5F*/, sizeCList);
    memset(newChangeList, 0, sizeCList);

    evolve_w = w;
    evolve_h = h;

    unsigned int startTime = getTime();

    while (getKey() != stopChar){
        evolve_changeListIn = changeList + w + 1;
        evolve_changeListOut = newChangeList + w + 1;

        evolveBoard(adr, board);
        drawBoard(adr, board);

        unsigned char* temp = changeList;
        changeList = newChangeList;
        newChangeList = temp;
        memset(newChangeList, 0, sizeCList);
    }

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

static void drawInfoScene(void){
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

    unsigned char* popCountAlloc = malloc(511);

    popCountHigh = ((unsigned int)popCountAlloc + 255) / 256;
    popCount = (unsigned char*) (256*popCountHigh);

    memcpy(popCount, popCountTable, 256);

    setWindow(2, 0, 79, 70);

    startGraphics();
    drawIntroScene();
    initializeScreen();
    drawInfoScene();
    initializeScreen();
    drawInstructionScene();

    start: 
    initializeScreen();

    drawText(4, 70, "Press [enter] to start", false, WHITETEXT);

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

    drawText(4, 70, "                    ", false, WHITETEXT);
    drawText(4, 70, "Press [space] to stop", false, WHITETEXT);

    runGOL(drawAdr, 35, 21, keySpace);

    drawText(30, 67, "Press Space restart", false, WHITETEXT);
    drawText(30, 70, "Press 0 to return to LCL", false, WHITETEXT);

    while (getKey() == keySpace);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;
}