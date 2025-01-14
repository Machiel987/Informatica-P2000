#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#define BLOCK_I 0b0000000011110000
#define BLOCK_O 0b0000011001100000
#define BLOCK_T 0b0000011100100000
#define BLOCK_S 0b0000001101100000
#define BLOCK_Z 0b0000110001100000
#define BLOCK_J 0b0010001001100000
#define BLOCK_L 0b0100010001100000

//Co-ordinates of the entire window
const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;
//Co-ordinates of the rolling window
const static unsigned char rollTLX = 26, rollTLY = 3, rollBRX = 55, rollBRY = 65;

#if 1
void blockBounds(unsigned int block, unsigned char* left, unsigned char* right, unsigned char* top, unsigned char* bottom){
    *left = 0;
    *right = 3;
    *top = 0;
    *bottom = 3;

    unsigned char done = 0;
    unsigned int mask = 8;
    unsigned char startP = mask;

    //sprintf(vidmem + 1840, "0");

    while (!done){
        mask = startP;
        for (unsigned char i = 0; i < 4; i++){
            done |= ((block & mask) != 0);
            mask <<= 4;
        }

        if (!done) (*left)++;

        startP >>= 1;
    }

    //sprintf(vidmem + 1842, "1");

    done = 0;
    mask = 1;
    startP = mask;

    while (!done){
        mask = startP;
        for (unsigned char i = 0; i < 4; i++){
            done |= ((block & mask) != 0);
            mask <<= 4;
        }

        if (!done) (*right)--;

        startP <<= 1;
    }

    //sprintf(vidmem + 1850, "2");

    done = 0;
    mask = 0x8000;

    while (!done){
        for (unsigned char i = 0; i < 4; i++){
            done |= ((block & mask) != 0);
            mask >>= 1;
        }

        if (!done) (*top)++;
    }

    //sprintf(vidmem + 1860, "3");

    done = 0;
    mask = 1;

    while (!done){
        for (unsigned char i = 0; i < 4; i++){
            done |= ((block & mask) != 0);
            mask <<= 1;
        }

        if (!done) (*bottom)--;
    }

    //sprintf(vidmem + 1870, "4");
}
#endif

#if 0
void blockBounds(unsigned int block, unsigned char* left, unsigned char* right, unsigned char* bottom, unsigned char* top){
    *left = 0;
    *right = 0;
    *bottom = 0;
    *top = 0;

    for (unsigned int i = 1; i != 0; i <<= 4)
        *right |= ((block & i) != 0);

    for (unsigned int i = 8; i != 0; i <<= 4)
        *left |= ((block & i) != 0);

    for (unsigned int i = 1; i < 0x10; i <<= 1)
        *bottom |= ((block & i) != 0);

    for (unsigned int i = 0x1000; i != 0; i <<= 1)
        *top |= ((block & i) != 0);
}
#endif

unsigned int rotateBlockL(unsigned int block){
    unsigned int out = 0;
    unsigned int maskIn = 0x8000;
    unsigned int maskOutBot = 0x8;

    for (unsigned char j = 0; j < 4; j++){
        unsigned int maskOut = maskOutBot;

        for (unsigned char i = 0; i < 4; i++){
            if (maskIn & block) out |= maskOut;

            maskIn >>= 1;
            maskOut <<= 4;
        }

        maskOutBot >>= 1;
    }

    return out;
}

unsigned int rotateBlockR(unsigned int block){
    unsigned int out = 0;
    unsigned int maskIn = 0x8000;
    unsigned int maskOutTop = 0x1000;

    for (unsigned char j = 0; j < 4; j++){
        unsigned int maskOut = maskOutTop;

        for (unsigned char i = 0; i < 4; i++){
            if (maskIn & block) out |= maskOut;

            maskIn >>= 1;
            maskOut >>= 4;
        }

        maskOutTop <<= 1;
    }

    return out;
}

void drawClearBlock(unsigned int block, unsigned char x, unsigned char y, unsigned char yn){
    unsigned int mask = 0x8000;

    for (unsigned char j = 0; j < 4; j++){
        for (unsigned char i = 0; i < 4; i++){
            unsigned int wt = (block & mask);
            mask >>= 1;

            if (wt){
                setPixel(x + 3*i, y + 3*j, yn);
                setPixel(x + 3*i + 1, y + 3*j, yn);
                setPixel(x + 3*i + 2, y + 3*j, yn);
                setPixel(x + 3*i, y + 3*j + 1, yn);
                setPixel(x + 3*i + 1, y + 3*j + 1, yn);
                setPixel(x + 3*i + 2, y + 3*j + 1, yn);
                setPixel(x + 3*i, y + 3*j + 2, yn);
                setPixel(x + 3*i + 1, y + 3*j + 2, yn);
                setPixel(x + 3*i + 2, y + 3*j + 2, yn);
            }
        }
    }
}

//Returns 0 on drawable, 1 on undrawable
unsigned char unDrawable(unsigned int block, unsigned char x, unsigned char y){
    unsigned int mask = 0x8000;

    for (unsigned char j = 0; j < 4; j++){
        for (unsigned char i = 0; i < 4; i++){
            if (getPixel(x + 3*i + 1, y + 3*j + 1) && (block & mask)) return 1;
            mask >>= 1;
        }
    }

    return 0;
}

//Returns 0 on normal case, returns 1 if block cannot be drawn in that location
unsigned char drawBlock(unsigned int block, unsigned char x, unsigned char y){
    if (unDrawable(block, x, y)) return 1;

    drawClearBlock(block, x, y, true);

    return 0;
}

void clearBlock(unsigned int block, unsigned char x, unsigned char y){
    drawClearBlock(block, x, y, false);
}

void tetris(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics();

    rectangle(rollTLX - 2, rollTLY - 2, rollBRX + 2, rollBRY + 2, true);
    rectangle(6, 6, 21, 21, true);

    unsigned int holdBlock = BLOCK_I;
    drawBlock(holdBlock, 8, 8);

    setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

    unsigned int score = 0;
    unsigned char *scoreStr = (unsigned char*) malloc(10);
    memset(scoreStr, 0, 10);

    srand(getTime());

    //unsigned char leftBound, rightBound, bottomBound, topBound;
    //sprintf(vidmem + 1840, "Get 1");
    //blockBounds(BLOCK_J, &leftBound, &rightBound, &bottomBound, &topBound);
    //sprintf(vidmem + 1840, "%d, %d, %d, %d", leftBound, rightBound, bottomBound, topBound);

#if 1

    while (true){
        unsigned char blockNum = rand() % 7;
        unsigned int block = 0;
        unsigned char blockPosX = rollTLX + 9;
        unsigned char blockPosY = rollTLY;

        switch (blockNum)
        {
        case 0: block = BLOCK_I; break;
        case 1: block = BLOCK_O; break;
        case 2: block = BLOCK_T; break;
        case 3: block = BLOCK_S; break;
        case 4: block = BLOCK_Z; break;
        case 5: block = BLOCK_J; break;
        case 6: block = BLOCK_L; break;
        }

        for (unsigned char j = 0; j < 22; j++){
            unsigned char clearLine = true;

            for (unsigned char i = 0; i < 10; i++)
                clearLine &= getPixel(rollTLX + 3*i, rollTLY + 3*j);

            if (clearLine) {
                setWindow(rollTLX, rollTLY, rollBRX, rollTLY + 3*j);
                rollAreaDown();
                rollAreaDown();
                rollAreaDown();
                setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

                score += 200;
            }
        }

        if (drawBlock(block, blockPosX, blockPosY)) break;
        clearBlock(block, blockPosX, blockPosY);

        unsigned char prevPosX = 0, prevPosY = 0;
        unsigned int prevBlock = block;

        unsigned char leftBound, rightBound, bottomBound, topBound;
        blockBounds(block, &leftBound, &rightBound, &bottomBound, &topBound);

        while (!drawBlock(block, blockPosX, blockPosY)){
            unsigned int startTime = getTime();
            unsigned char prevKey = getKey();

            while (getTime() - startTime < 5){
                if (getKey() == keySpace) break;
                if (getKey() == prevKey) continue;
                prevKey = getKey();
                if (prevKey == keyNone) continue;
                
                prevPosX = blockPosX;
                
                clearBlock(block, blockPosX, blockPosY);

                switch (getKey())
                {
                case keyRight:
                    if (blockPosX + 3 * rightBound > rollBRX - 3) break;
                    blockPosX += 3;
                    break;
                
                case keyLeft:
                    if (blockPosX + 3 * leftBound < rollTLX + 3) break;
                    blockPosX -= 3;
                    break;

                case keyA:
                    prevBlock = block;
                    block = rotateBlockL(block);
                    blockBounds(block, &leftBound, &rightBound, &bottomBound, &topBound);
                    
                    if (blockPosX + 3 * leftBound < rollTLX){
                        prevPosX = blockPosX;
                        blockPosX = rollTLX - 3 * leftBound;
                    }
                    if (blockPosX + 3 * rightBound > rollBRX){
                        prevPosX = blockPosX;
                        blockPosX = rollBRX - 3 * rightBound;
                    }
                    break;
                
                case keyS:
                    prevBlock = block;
                    block = rotateBlockR(block);
                    blockBounds(block, &leftBound, &rightBound, &bottomBound, &topBound);

                    if (blockPosX + 3 * leftBound < rollTLX){
                        prevPosX = blockPosX;
                        blockPosX = rollTLX - 3 * leftBound;
                    }
                    if (blockPosX + 3 * rightBound > rollBRX){
                        prevPosX = blockPosX;
                        blockPosX = rollBRX - 3 * rightBound;
                    }
                    break;
                }

                if (drawBlock(block, blockPosX, blockPosY)){
                    blockPosX = prevPosX;
                    block = prevBlock;

                    drawBlock(block, blockPosX, blockPosY);
                }
            }

            clearBlock(block, blockPosX, blockPosY);

            setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
            if (getKey() == keyZ && !unDrawable(holdBlock, blockPosX, blockPosY + 3)){
                clearBlock(holdBlock, 8, 8);

                prevBlock = block;

                block = holdBlock;
                holdBlock = prevBlock;

                drawBlock(holdBlock, 8, 8);
            }

            score++;
            sprintf(scoreStr, "%i", score);
            drawText(4, 70, scoreStr, false, WHITETEXT);

            setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

            prevPosY = blockPosY;
            blockPosY += 3;
        }

        drawBlock(block, blockPosX, prevPosY);
    }

#endif

setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
//drawText(4, 70, "Done!", false, WHITETEXT);

while (true);

}
