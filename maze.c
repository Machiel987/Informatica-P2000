#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;

struct cellCoords{
    unsigned char x;
    unsigned char y;
};

//struct cellCoords* visited;
unsigned char* visitedX, *visitedY;
unsigned int visLen = 0;

unsigned char drawX = 0, drawY = 0;
unsigned char sizeX = 30, sizeY = 30; //Default size is  30x30;

struct cellCoords addCoords(struct cellCoords a, struct cellCoords b){
    struct cellCoords out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

struct cellCoords subCoords(struct cellCoords a, struct cellCoords b){
    struct cellCoords out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

void addVis(struct cellCoords coords){
    visitedX[visLen] = coords.x;
    visitedY[visLen] = coords.y;
    visLen++;
}

void delVis(unsigned int index){
    visLen--;
    visitedX[index] = visitedX[visLen];
    visitedY[index] = visitedY[visLen];
}

unsigned char* findVis(struct cellCoords coords){
    unsigned char* p = visitedX;

    while (true){
        p = memchr(p, coords.x, visLen - (p - visitedX));
        if (!p)
            return NULL;

        if (visitedY[p - visitedX] == coords.y)
            return p;
        
        p++;
    }
}

#if 0
unsigned char* findVis(struct cellCoords coords){
    for (unsigned char* i = visitedX; i < visitedX + visLen; i++){
        if (*i == coords.x && visitedY[i - visitedX] == coords.y)
            return i;
    }
    return NULL;
}
#endif

unsigned char checkLoc(struct cellCoords coords){
    if (findVis(coords)) return false;
    if (getPixel(coords.x, coords.y)) return false;
    if (coords.x < drawX || coords.x > drawX + sizeX || coords.y < drawY || coords.y > drawY + sizeY) return false;
    
    return true;
}

unsigned int randomVis(void){
    unsigned int index = rand() % visLen;

    return index;
}

void drawMaze(unsigned char drawLocX, unsigned char drawLocY, unsigned char mazeSizeX, unsigned char mazeSizeY){
    drawX = drawLocX;
    drawY = drawLocY;
    sizeX = mazeSizeX;
    sizeY = mazeSizeY;

    visitedX = (unsigned char*)malloc((sizeX + sizeY) << 1); //Theoretically not always enough, but fine in practice
    visitedY = (unsigned char*)malloc((sizeX + sizeY) << 1);

    if (visitedX == NULL || visitedY == NULL){
        sprintf(vidmem + 1840, "Out of memory");
        return;
    }

    visitedX[0] = drawLocX;
    visitedY[0] = drawLocY;
    visLen = 1;

    setPixel(drawLocX, drawLocY, true);

    while (visLen != 0){
        struct cellCoords current;
        unsigned int index = randomVis();
        current.x = visitedX[index];
        current.y = visitedY[index];

        struct cellCoords edges[4] = {
            {current.x + 2, current.y    },
            {current.x    , current.y + 2},
            {current.x - 2, current.y    },
            {current.x    , current.y - 2}};

        struct cellCoords filteredEdges[4];
        unsigned char edgeNum = 0;

        for (unsigned char i = 0; i < 4; i++){
            if (checkLoc(edges[i])){
                filteredEdges[edgeNum] = edges[i];
                edgeNum++;
            }
        }

        if (edgeNum == 0){
            delVis(index);
            continue;
        }

        struct cellCoords nextEdge;
        nextEdge = filteredEdges[rand() % edgeNum];
        //memcpy(&nextEdge, filteredEdges + (rand() % edgeNum), sizeof(struct cellCoords));

        setPixel(nextEdge.x, nextEdge.y, true);
        setPixel((current.x + nextEdge.x) >> 1, (current.y + nextEdge.y) >> 1, true);

        addVis(nextEdge);
    }

    free(visitedY);
    free(visitedX);
}

void maze(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    srand(getTime());

    unsigned int startTime = getTime();

    drawMaze(3, 3, 40, 40);

    sprintf(vidmem + 1840, "Done in %d ticks", getTime() - startTime);

    while (true);
}