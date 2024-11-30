#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"

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

void drawBoard(unsigned char* boardStatus, unsigned char* adr, unsigned char w, unsigned char h){
    for (unsigned char j = 0; j < h; j++){ //j = y-coordinate
        for (unsigned char i = 0; i < w; i++){ //i = x-coordinate
            adr[80*j + i] = boardStatus[w*j + i];
        }
    }
}

void evolveBoard(unsigned char* adr, unsigned char* out, unsigned char w, unsigned char h){
    memset(out, 0x20, w*h);

    for (unsigned char j = 0; j < h; j++){
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

void runGOT(char* adr, unsigned char w, unsigned char h){
    unsigned char* newBoard = (unsigned char*) malloc(((unsigned) w) * ((unsigned) h));
    if (newBoard == NULL) return;

    for (unsigned char i = 0; i < 20; i++){
        evolveBoard(adr, newBoard, w, h);
        drawBoard(newBoard, adr, w, h);
    }

    free(newBoard);
}

int main(void){
    startGraphics();

    unsigned char* drawAdr = (unsigned char*) (vidmem + 80*2 + 4);

    unsigned short *p_counter = (unsigned short*) 0x6010;
    short startTime = *p_counter;

    //drawLine(10, 10, 60, 50, true);
    setPixel(30, 30, true); //28, 26
    setPixel(31, 30, true);
    setPixel(31, 29, true);
    setPixel(32, 29, true);
    setPixel(32, 31, true);

    runGOT(drawAdr, 30, 15);
    

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