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

struct cellCoords* visited;
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
    visited[visLen] = coords;
    visLen++;
}

void delVis(struct cellCoords* coords){
    visLen--;
    *coords = *(visited + visLen);
}

struct cellCoords* findVis(struct cellCoords coords){
    struct cellCoords* loc = visited;

    unsigned char found = false;

    for (unsigned int i = 0; i < visLen; i++){
        if (memcmp(loc, &coords, sizeof(struct cellCoords)) == 0){
            found = true;
            break;
        }

        loc++;
    }

    if (found) return loc;
    else return NULL;
}

unsigned char checkLoc(struct cellCoords coords){
    if (findVis(coords)) return false;
    if (getPixel(coords.x, coords.y)) return false;
    if (coords.x < drawX || coords.x > drawX + sizeX || coords.y < drawY || coords.y > drawY + sizeY) return false;
    
    return true;
}

struct cellCoords* randomVis(void){
    unsigned int index = rand() % visLen;

    return (visited + index);
}

void drawMaze(unsigned char drawLocX, unsigned char drawLocY, unsigned char mazeSizeX, unsigned char mazeSizeY){
    drawX = drawLocX;
    drawY = drawLocY;
    sizeX = mazeSizeX;
    sizeY = mazeSizeY;

    visited = (struct cellCoords*)malloc((sizeX + sizeY) << 1); //Theoretically not always enough, but fine in practice

    if (visited == NULL){
        sprintf(vidmem + 1840, "Out of memory");
        return;
    }

    visited[0].x = drawLocX;
    visited[0].y = drawLocY;
    visLen = 1;

    setPixel(drawLocX, drawLocY, true);

    while (visLen != 0){
        struct cellCoords *current = randomVis();

        struct cellCoords edges[4] = {
            {current->x + 2, current->y    },
            {current->x    , current->y + 2},
            {current->x - 2, current->y    },
            {current->x    , current->y - 2}};

        struct cellCoords filteredEdges[4];
        unsigned char edgeNum = 0;

        for (unsigned char i = 0; i < 4; i++){
            if (checkLoc(edges[i])){
                filteredEdges[edgeNum] = edges[i];
                edgeNum++;
            }
        }

        if (edgeNum == 0){
            delVis(current);
            continue;
        }

        struct cellCoords nextEdge;
        memcpy(&nextEdge, filteredEdges + (rand() % edgeNum), sizeof(struct cellCoords));

        setPixel(nextEdge.x, nextEdge.y, true);
        setPixel((current->x + nextEdge.x) >> 1, (current->y + nextEdge.y) >> 1, true);

        addVis(nextEdge);
    }

    free(visited);
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