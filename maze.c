#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#define OFFSET 80

const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 159, windowBRY = 70;

struct cellCoords{
    unsigned char x;
    unsigned char y;
};

//struct cellCoords* visited;
struct cellCoords* visited;
unsigned int visLen = 0;

unsigned char drawX = 0, drawY = 0;
unsigned char sizeX = 30, sizeY = 30; //Default size is  30x30;

static struct cellCoords addCoords(struct cellCoords a, struct cellCoords b){
    struct cellCoords out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

static struct cellCoords subCoords(struct cellCoords a, struct cellCoords b){
    struct cellCoords out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

static inline void addVis(struct cellCoords coords){
    setPixel(coords.x + OFFSET, coords.y, true);

    visited[visLen] = coords;
    visLen++;
}

static inline void delVis(struct cellCoords* coords){
    setPixel(coords->x + OFFSET, coords->y, false);

    visLen--;
    *coords = visited[visLen];
}

static unsigned char findVis(struct cellCoords coords){
    return getPixel(coords.x + OFFSET, coords.y);
}

#if 0
static unsigned char* findVis(struct cellCoords coords){
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
#endif

#if 0
unsigned char* findVis(struct cellCoords coords){
    for (unsigned char* i = visitedX; i < visitedX + visLen; i++){
        if (*i == coords.x && visitedY[i - visitedX] == coords.y)
            return i;
    }
    return NULL;
}
#endif

static unsigned char checkLoc(struct cellCoords coords){
    if (findVis(coords)) return false;
    if (getPixel(coords.x, coords.y)) return false;
    if (coords.x < drawX || coords.x > drawX + sizeX || coords.y < drawY || coords.y > drawY + sizeY) return false;
    
    return true;
}

static inline unsigned char edgeGen(struct cellCoords coords, struct cellCoords* edges){
    unsigned char len = 0;
    struct cellCoords current;
    current = coords;

    current.x = coords.x + 2;
    if (checkLoc(current)){
        edges[len] = current;
        len++;
    }

    current.x = coords.x - 2;
    if (checkLoc(current)){
        edges[len] = current;
        len++;
    }

    current.x = coords.x;
    current.y = coords.y + 2;
    if (checkLoc(current)){
        edges[len] = current;
        len++;
    }

    current.y = coords.y - 2;
    if (checkLoc(current)){
        edges[len] = current;
        len++;
    }

    return len;
}

void drawMaze(unsigned char drawLocX, unsigned char drawLocY, unsigned char mazeSizeX, unsigned char mazeSizeY){
    drawX = drawLocX;
    drawY = drawLocY;
    sizeX = mazeSizeX;
    sizeY = mazeSizeY;

    visited = (struct cellCoords*)malloc(((sizeX + sizeY) << 1) * sizeof(struct cellCoords)); //Theoretically not always enough, but fine in practice

    if (visited == NULL){
        sprintf(vidmem + 1840, "Out of memory");
        return;
    }

    visited[0].x = drawLocX;
    visited[0].y = drawLocY;
    visLen = 1;

    fillRectangle(OFFSET, 0, 159, 71, false);

    setPixel(drawLocX, drawLocY, true);

    while (visLen != 0){
        struct cellCoords current;
        unsigned int index = rand() % visLen;
        current = visited[index];

        struct cellCoords edges[4];

        unsigned char edgeNum = edgeGen(current, edges);

        if (edgeNum == 0){
            delVis(visited + index);
            continue;
        }

        struct cellCoords nextEdge;
        nextEdge = edges[rand() % edgeNum];

        setPixel(nextEdge.x, nextEdge.y, true);
        setPixel((current.x + nextEdge.x) >> 1, (current.y + nextEdge.y) >> 1, true);

        addVis(nextEdge);
    }

    free(visited);
}

void maze(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    srand(getTime());

    unsigned int startTime = getTime();

    drawMaze(2, 0, 79, 68);

    sprintf(vidmem + 1840, "Done in %d ticks", getTime() - startTime);

    while (true);
}