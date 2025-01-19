#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

//The pixels of a tetrominoe is represented as an uint where the HSB is the top-left corner and the LSB is the bottom-right corner
#define BLOCK_I_PIXELS 0b0000000011110000
#define BLOCK_O_PIXELS 0b0000011001100000
#define BLOCK_T_PIXELS 0b0000011100100000
#define BLOCK_S_PIXELS 0b0000001101100000
#define BLOCK_Z_PIXELS 0b0000110001100000
#define BLOCK_J_PIXELS 0b0010001001100000
#define BLOCK_L_PIXELS 0b0100010001100000

//Structure representing a single tetrominoe
struct block {
    unsigned int pixels;
    unsigned char boundL : 2;
    unsigned char boundR : 2;
    unsigned char boundT : 2;
    unsigned char boundB : 2;
};

const struct block block_I = {BLOCK_I_PIXELS, 0, 3, 2, 2};
const struct block block_O = {BLOCK_O_PIXELS, 1, 2, 1, 2};
const struct block block_T = {BLOCK_T_PIXELS, 1, 3, 1, 2};
const struct block block_S = {BLOCK_S_PIXELS, 1, 3, 1, 2};
const struct block block_Z = {BLOCK_Z_PIXELS, 0, 2, 1, 2};
const struct block block_J = {BLOCK_J_PIXELS, 1, 2, 0, 2};
const struct block block_L = {BLOCK_L_PIXELS, 1, 2, 0, 2};

//Co-ordinates of the entire window
const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;
//Co-ordinates of the rolling window
const static unsigned char rollTLX = 26, rollTLY = 3, rollBRX = 55, rollBRY = 65;

static unsigned int score = 0;
static unsigned char scoreStr[10] = {0,0,0,0,0,0,0,0,0,0};

//Rotate the pixels of a block counter-clockwise and update the boundaries
struct block rotateBlockL(struct block blockIn){
    struct block blockOut;
    blockOut.boundT = 3 - blockIn.boundR;
    blockOut.boundL = blockIn.boundT;
    blockOut.boundB = 3 - blockIn.boundL;
    blockOut.boundR = blockIn.boundB;

    blockOut.pixels = 0;

    unsigned int maskIn = 0x8000; //Bit mask (popCount = 1) over the input pixels
    unsigned int maskOutBot = 0x8; //Bit mask (popCount = 1) over the bottom row of output pixels

    for (unsigned char j = 0; j < 4; j++){
        unsigned int maskOut = maskOutBot;

        for (unsigned char i = 0; i < 4; i++){
            if (maskIn & blockIn.pixels) blockOut.pixels |= maskOut;

            maskIn >>= 1;
            maskOut <<= 4;
        }

        maskOutBot >>= 1;
    }

    return blockOut;
}

//Rotate the pixels of a block clockwise and update the boundaries
struct block rotateBlockR(struct block blockIn){
    struct block blockOut;
    blockOut.boundT = blockIn.boundL;
    blockOut.boundL = 3 - blockIn.boundB;
    blockOut.boundB = blockIn.boundR;
    blockOut.boundR = 3 - blockIn.boundT;

    blockOut.pixels = 0;

    unsigned int maskIn = 0x8000; //Bit mask (popCount = 1) over the input pixels
    unsigned int maskOutTop = 0x1000; //Bit mask (popCount = 1) over the top row of output pixels

    for (unsigned char j = 0; j < 4; j++){
        unsigned int maskOut = maskOutTop;

        for (unsigned char i = 0; i < 4; i++){
            if (maskIn & blockIn.pixels) blockOut.pixels |= maskOut;

            maskIn >>= 1;
            maskOut >>= 4;
        }

        maskOutTop <<= 1;
    }

    return blockOut;
}

//Draw/clear a tetrominoe. yn = 0 -> clear, yn = 1 -> draw
void drawClearBlock(struct block block, unsigned char x, unsigned char y, unsigned char yn){
    unsigned int mask = 0x8000;

    for (unsigned char j = 0; j < 4; j++){
        for (unsigned char i = 0; i < 4; i++){
            unsigned int wt = (block.pixels & mask);
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

//Returns 0 on drawable, 1 on undrawable. Does not check bounds
unsigned char unDrawable(struct block block, unsigned char x, unsigned char y){
    unsigned int mask = 0x8000;

    for (unsigned char j = 0; j < 4; j++){
        for (unsigned char i = 0; i < 4; i++){
            if (getPixel(x + 3*i + 1, y + 3*j + 1) && (block.pixels & mask)) return 1;
            mask >>= 1;
        }
    }

    return 0;
}

//Returns 0 on normal case, returns 1 if block cannot be drawn in that location
unsigned char drawBlock(struct block block, unsigned char x, unsigned char y){
    if (unDrawable(block, x, y)) return 1;

    drawClearBlock(block, x, y, true);

    return 0;
}

//Clears a block
void clearBlock(struct block block, unsigned char x, unsigned char y){
    drawClearBlock(block, x, y, false);
}

//Main function
void tetris(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics();

    rectangle(rollTLX - 2, rollTLY - 2, rollBRX + 2, rollBRY + 2, true);
    rectangle(6, 6, 21, 21, true);

    struct block holdBlock; //block in the holding position
    holdBlock = block_I;
    drawBlock(holdBlock, 8, 8);

    setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

    srand(getTime()); //Randomise

    while (true){
        unsigned char blockNum = rand() % 7; //Use modulo to clip between 0 & 6
        struct block block;
        unsigned char blockPosX = rollTLX + 9;
        unsigned char blockPosY = rollTLY;

        switch (blockNum)
        {
        case 0: block = block_I; break;
        case 1: block = block_O; break;
        case 2: block = block_T; break;
        case 3: block = block_S; break;
        case 4: block = block_Z; break;
        case 5: block = block_J; break;
        case 6: block = block_L; break;
        }

        //Check whether line should be cleared
        for (unsigned char j = 0; j < 22; j++){
            unsigned char clearLine = true;

            for (unsigned char i = 0; i < 10; i++)
                clearLine &= getPixel(rollTLX + 3*i, rollTLY + 3*j);

            if (clearLine) {
                setWindow(rollTLX, rollTLY, rollBRX, rollTLY + 3*j);
                rollAreaDown(); //Needs to be done 3 times, as a block-pixel 3 screen-pixels high
                rollAreaDown();
                rollAreaDown();
                setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

                score += 200;
            }
        }

        //If a new block cannot be drawn in the starting location, the game is over
        if (drawBlock(block, blockPosX, blockPosY)) break;
        clearBlock(block, blockPosX, blockPosY);

        //Position and block indicating last stable position for drawing the block.
        //If a new block cannot be drawn, the previous position is maintained
        unsigned char prevPosX = 0, prevPosY = 0;
        struct block prevBlock;
        prevBlock = block;

        //Keep drawing the old block in a position one step down
        while (!drawBlock(block, blockPosX, blockPosY)){
            unsigned int startTime = getTime();
            unsigned char prevKey = getKey();

            while (getTime() - startTime < 10){ //Use this number to set the speed of the game
                if (getKey() == keySpace) break;
                if (getKey() == prevKey) continue;
                prevKey = getKey();
                if (prevKey == keyNone) continue;
                
                prevPosX = blockPosX;
                
                clearBlock(block, blockPosX, blockPosY); //Clear old block

                switch (getKey())
                {
                case keyRight:
                    if (blockPosX + 3 * block.boundR > rollBRX - 3) break;
                    blockPosX += 3;
                    break;
                
                case keyLeft:
                    if (blockPosX + 3 * block.boundL < rollTLX + 3) break;
                    blockPosX -= 3;
                    break;

                case keyA: case keyS:
                    prevBlock = block;
                    if (getKey() == keyA) block = rotateBlockL(block);
                    else block = rotateBlockR(block);
                    
                    if (blockPosX + 3 * block.boundL < rollTLX){
                        prevPosX = blockPosX;
                        blockPosX = rollTLX - 3 * block.boundL;
                    }
                    if (blockPosX + 3 * block.boundR > rollBRX){
                        prevPosX = blockPosX;
                        blockPosX = rollBRX - 2 - 3 * block.boundR;
                    }
                    break;
                }

                if (drawBlock(block, blockPosX, blockPosY)){ //If the new block cannot be drawn, it is drawn in the previous position
                    blockPosX = prevPosX;
                    block = prevBlock;

                    drawBlock(block, blockPosX, blockPosY);
                }
            }

            clearBlock(block, blockPosX, blockPosY);

            //Update the holding block
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

setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
//drawText(4, 70, "Done!", false, WHITETEXT);

return;
}
