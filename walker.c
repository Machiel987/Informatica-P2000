#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

static unsigned char walkerTLX = 2, walkerTLY = 0, walkerBRX = 79, walkerBRY = 68;
static unsigned char rollTLX = 4, rollTLY = 3, rollBRX = 77, rollBRY = 65;
static unsigned int topEdge = 3, bottomEdge = 65, leftEdge = 4, rightEdge = 77;

unsigned int horizontals[][4] = {
    {80, 115, 95, 1},
    //{90, 110, 105, 1}
};
unsigned int verticals[][4] = {
    {85, 85, 100, 1},
    //{120, 80, 120, 1}
};

static unsigned char horizontalsLen = sizeof(horizontals) / sizeof(int) / 4, verticalsLen = sizeof(verticals) / sizeof(int) / 4;

void walker(void){
    setWindow(walkerTLX, walkerTLY, walkerBRX, walkerBRY);
    startGraphics();

    rectangle(2, 1, 79, 67, true);

    unsigned int posX = 70, posY = 70;

    for (unsigned char i = 0; i < horizontalsLen; i++){
        horzLine(horizontals[i][0] - posX, horizontals[i][1] - posX, horizontals[i][2] - posY, horizontals[i][3]);
    }

    for (unsigned char i = 0; i < verticalsLen; i++){
        vertLine(verticals[i][0] - posX, verticals[i][1] - posY, verticals[i][2] - posY, verticals[i][3]);
    }

    setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

    unsigned char key = 255, prevKey = 255;
    while (true){
        while (key == prevKey){
            key = getKey();
        }

        prevKey = key;

        switch (key)
        {
        case keyUp:
            posY--;
            rollAreaDown(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][2] == topEdge + posY)
                    horzLine(horizontals[i][0] - posX, horizontals[i][1] - posX, horizontals[i][2] - posY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][1] <= topEdge + posY && verticals[i][2] >= topEdge + posY)
                    setPixel(verticals[i][0] - posX, topEdge, verticals[i][3]);
            }
            break;
        
        case keyDown:
            posY++;
            rollAreaUp(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][2] == bottomEdge + posY)
                    horzLine(horizontals[i][0] - posX, horizontals[i][1] - posX, horizontals[i][2] - posY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][1] <= bottomEdge + posY && verticals[i][2] >= bottomEdge + posY)
                    setPixel(verticals[i][0] - posX, bottomEdge, verticals[i][3]);
            }
            break;

        case keyLeft:
            posX--;
            rollAreaRight(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][0] <= leftEdge + posX && horizontals[i][1] >= leftEdge + posX)
                    setPixel(leftEdge, horizontals[i][2] - posY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][0] == leftEdge + posX)
                    vertLine(verticals[i][0] - posX, verticals[i][1] - posY, verticals[i][2] - posY, verticals[i][3]);
            }
            break;

        case keyRight:
            posX++;
            rollAreaLeft(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][0] <= rightEdge + posX && horizontals[i][1] >= rightEdge + posX)
                    setPixel(rightEdge, horizontals[i][2] - posY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][0] == rightEdge + posX)
                    vertLine(verticals[i][0] - posX, verticals[i][1] - posY, verticals[i][2] - posY, verticals[i][3]);
            }
            break;
        }
    }
}