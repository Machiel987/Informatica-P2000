#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#define OFFSET 80

const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 159, windowBRY = 70;

const static unsigned char voxelSize = 8;

struct vec3 viewLoc = {0, 0, 0};
struct vec3 viewPlane = {0, 0, 8};

struct screenVec2{
    unsigned char x;
    unsigned char y;
};

struct bigScreenVec2{
    signed int x;
    signed int y;
};

struct vec3{
    signed int x;
    signed int y;
    signed int z;
};

//struct screenVec2* visited;
struct screenVec2* visited;
unsigned int visLen = 0;

unsigned char drawX = 0, drawY = 0;
unsigned char sizeX = 30, sizeY = 30; //Default size is  30x30;

static struct screenVec2 addCoords(struct screenVec2 a, struct screenVec2 b){
    struct screenVec2 out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

static struct screenVec2 subCoords(struct screenVec2 a, struct screenVec2 b){
    struct screenVec2 out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

static inline void addVis(struct screenVec2 coords){
    setPixel(coords.x + OFFSET, coords.y, true);

    visited[visLen] = coords;
    visLen++;
}

static inline void delVis(struct screenVec2* coords){
    setPixel(coords->x + OFFSET, coords->y, false);

    visLen--;
    *coords = visited[visLen];
}

static unsigned char findVis(struct screenVec2 coords){
    return getPixel(coords.x + OFFSET, coords.y);
}

#if 0
static unsigned char* findVis(struct screenVec2 coords){
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
unsigned char* findVis(struct screenVec2 coords){
    for (unsigned char* i = visitedX; i < visitedX + visLen; i++){
        if (*i == coords.x && visitedY[i - visitedX] == coords.y)
            return i;
    }
    return NULL;
}
#endif

static unsigned char checkLoc(struct screenVec2 coords){
    if (findVis(coords)) return false;
    if (getPixel(coords.x, coords.y)) return false;
    if (coords.x < drawX || coords.x > drawX + sizeX || coords.y < drawY || coords.y > drawY + sizeY) return false;
    
    return true;
}

static inline unsigned char edgeGen(struct screenVec2 coords, struct screenVec2* edges){
    unsigned char len = 0;
    struct screenVec2 current;
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

    visited = (struct screenVec2*)malloc(((sizeX + sizeY) << 1) * sizeof(struct screenVec2)); //Theoretically not always enough, but fine in practice

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
        struct screenVec2 current;
        unsigned int index = rand() % visLen;
        current = visited[index];

        struct screenVec2 edges[4];

        unsigned char edgeNum = edgeGen(current, edges);

        if (edgeNum == 0){
            delVis(visited + index);
            continue;
        }

        struct screenVec2 nextEdge;
        nextEdge = edges[rand() % edgeNum];

        setPixel(nextEdge.x, nextEdge.y, true);
        setPixel((current.x + nextEdge.x) >> 1, (current.y + nextEdge.y) >> 1, true);

        addVis(nextEdge);
    }

    free(visited);
}

void traverseMaze(void){
    unsigned char posX = drawX, posY = drawY;

    while (posX != sizeX || posY != sizeY){
        switch (getKey())
        {
        case keyLeft:
            if (posX != drawX && getPixel(posX - 1, posY)){
                setPixel(posX, posY, true);
                posX--;
            }
            break;
        
        case keyRight:
            if (posX != drawX + sizeX && getPixel(posX + 1, posY)){
                setPixel(posX, posY, true);
                posX++;
            }
            break;

        case keyUp:
            if (posY != drawY && getPixel(posX, posY - 1)){
                setPixel(posX, posY, true);
                posY--;
            }
            break;

        case keyDown:
            if (posY != drawY + sizeY && getPixel(posX, posY + 1)){
                setPixel(posX, posY, true);
                posY++;
            }
            break;

        default:
            continue;
        }

        setPixel(posX, posY, false);

        unsigned int time = getTime();
        while (getTime() - time < 6);

        sprintf(vidmem + 1840, "(%d, %d)", posX, posY);
    }

    sprintf(vidmem + 1840, "You completed the maze");
}

static struct bigScreenVec2 toScreen(struct vec3 vector){
    signed int denom = vector.z - viewLoc.z;

    if (denom == 0){
        struct bigScreenVec2 out = {0, 0};
        return out;
    }

    int x = (viewPlane.z * (vector.x - viewLoc.x) << 2) / denom;
    x += 40;

    int y = (viewPlane.z * (vector.y - viewLoc.y) << 2) / denom;
    y += 37;

    struct bigScreenVec2 out = {x, y};
    return out;
}

void voxelLine(int x0, int y0, int x1, int y1, unsigned char wt){
    unsigned int xa = x0 + 0x8000, ya = y0 + 0x8000;
    unsigned int xb = x1 + 0x8000, yb = y1 + 0x8000;

    int dx = abs(xb - xa);
    signed char sx = xa < xb ? 1 : -1;

    int dy = -abs(yb - ya);
    signed char sy = ya < yb ? 1 : -1;

    int error = dx + dy;
    int e2;

    while(true){
        if (xa >= 0x8000 && xa <= 0x80FF && ya >= 0x8000 && ya <= 0x80FF)
            setPixel(xa & 0xFF, ya & 0xFF, wt);
        
        if (xa == xb && ya == yb) break;
        e2 = 2 * error;

        if (e2 - dy >= 0){
            error += dy;
            xa += sx;
        }

        if (e2 - dx < 0){
            error += dx;
            ya += sy;
        }
    }

    return;
}

void triangleLine(unsigned int* x, unsigned int* y, int* error, int dx, int dy, signed char sy){
    while (true){
        int e2 = 2 * (*error);

        if (e2 - dx < 0){
            *error += dx;
            (*y) += sy;
        }

        if (e2 - dy >= 0){
            *error += dy;
            (*x)++;
            break;
        }
    }
}

void voxelTriangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned char wt){
    unsigned int xs[3] = {x0 + 0x8000, x1 + 0x8000, x2 + 0x8000};
    unsigned int ys[3] = {y0 + 0x8000, y1 + 0x8000, y2 + 0x8000};

    unsigned int sortedXs[3];
    unsigned int sortedYs[3];

    for (unsigned char j = 0; j < 3; j++){
        unsigned char topInd = 0;
        if (xs[1] < xs[topInd]) topInd = 1;
        if (xs[2] < xs[topInd]) topInd = 2;

        sortedXs[j] = xs[topInd];
        sortedYs[j] = ys[topInd];
        xs[topInd] = UINT16_MAX;
    }

    unsigned int v0X = sortedXs[0], v0Y = sortedYs[0];
    unsigned int v1X = sortedXs[1], v1Y = sortedYs[1];
    unsigned int v2X = sortedXs[2], v2Y = sortedYs[2];

    //sprintf(vidmem + 1840, "(%d, %d), (%d, %d), (%d, %d)", v0X, v0Y, v1X, v1Y, v2X, v2Y);

    unsigned int xa = v0X, ya = v0Y, xb = v0X, yb = v0Y;

    int dx1 = v1X - v0X;

    int dy1 = -abs(v0Y - v1Y);
    signed char sy1 = v0Y < v1Y ? 1 : -1;

    int error1 = dx1 + dy1;

    int dx2 = v2X - v0X;

    int dy2 = -abs(v0Y - v2Y);
    signed char sy2 = v0Y < v2Y ? 1 : -1;

    int error2 = dx2 + dy2;

    while (true){
        if (xa >= 0x8000 && xa <= 0x80FF){
            unsigned int yMin = MIN(ya, yb), yMax = MAX(ya, yb);

            if (yMin < 0x8000) yMin = 0x8000;
            if (yMin > 0x80FF) break;
            if (yMax < 0x8000) break;
            if (yMax > 0x80FF) yMax = 0x80FF;

            vertLine(xa & 0xFF, yMin & 0xFF, yMax & 0xFF, wt);
        }
        
        if (xa == v1X) break;

        triangleLine(&xa, &ya, &error1, dx1, dy1, sy1);
        triangleLine(&xb, &yb, &error2, dx2, dy2, sy2);
    }

    ya = v1Y;

    dx1 = v2X - v1X;

    dy1 = -abs(v1Y - v2Y);
    sy1 = v1Y < v2Y ? 1 : -1;

    error1 = dx1 + dy1;

    while (true){
        if (xa >= 0x8000 && xa <= 0x80FF){
            unsigned int yMin = MIN(ya, yb), yMax = MAX(ya, yb);

            if (yMin < 0x8000) yMin = 0x8000;
            if (yMin > 0x80FF) goto skip;
            if (yMax < 0x8000) goto skip;
            if (yMax > 0x80FF) yMax = 0x80FF;

            vertLine(xa & 0xFF, yMin & 0xFF, yMax & 0xFF, wt);
        }

        skip:
        
        if (xa == v2X) break;

        triangleLine(&xa, &ya, &error1, dx1, dy1, sy1);
        triangleLine(&xb, &yb, &error2, dx2, dy2, sy2);
    }

    return;
}

void voxelFace(struct bigScreenVec2 v0, struct bigScreenVec2 v1, struct bigScreenVec2 v2, struct bigScreenVec2 v3, unsigned char wt){
    voxelTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, wt);
    voxelTriangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, wt);

    if (!wt){
        voxelLine(v0.x, v0.y, v1.x, v1.y, true);
        voxelLine(v1.x, v1.y, v3.x, v3.y, true);
        voxelLine(v3.x, v3.y, v2.x, v2.y, true);
        voxelLine(v2.x, v2.y, v0.x, v0.y, true);
    }
}

void drawVoxel(struct vec3 vector){
    struct bigScreenVec2 points[8];

    for (unsigned char i = 0; i < 8; i++){
        struct vec3 point3D = {(i & 0b1) * voxelSize + vector.x, ((i & 0b10) >> 1) * voxelSize + vector.y, ((i & 0b100) >> 2) * voxelSize + vector.z};
        points[i] = toScreen(point3D);
    }

    voxelFace(points[0], points[2], points[4], points[6], true);
    voxelFace(points[1], points[3], points[5], points[7], true);
    voxelFace(points[0], points[1], points[2], points[3], false);
/*
    voxelLine(points[0].x, points[0].y, points[1].x, points[1].y, true);
    voxelLine(points[0].x, points[0].y, points[2].x, points[2].y, true);
    voxelLine(points[1].x, points[1].y, points[3].x, points[3].y, true);
    voxelLine(points[2].x, points[2].y, points[3].x, points[3].y, true);
    voxelLine(points[0].x, points[0].y, points[4].x, points[4].y, true);
    voxelLine(points[1].x, points[1].y, points[5].x, points[5].y, true);
    voxelLine(points[2].x, points[2].y, points[6].x, points[6].y, true);
    voxelLine(points[3].x, points[3].y, points[7].x, points[7].y, true);
    voxelLine(points[4].x, points[4].y, points[5].x, points[5].y, true);
    voxelLine(points[4].x, points[4].y, points[6].x, points[6].y, true);
    voxelLine(points[5].x, points[5].y, points[7].x, points[7].y, true);
    voxelLine(points[6].x, points[6].y, points[7].x, points[7].y, true);
*/
}

void maze(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    srand(getTime());

    struct vec3 voxelList[] = {{0, 4, 24}, {8, 4, 16}, {-8, 4, 16}};
    unsigned int voxelLen = sizeof (voxelList) / sizeof (struct vec3);

#if 0
    while (true){
        while (getKey() == keyNone);

        switch (getKey())
        {
        case keyUp:
            viewLoc.z += 1;
            break;
        
        case keyDown:
            viewLoc.z -= 1;
            break;

        case keyRight:
            viewLoc.x += 1;
            break;
        
        case keyLeft:
            viewLoc.x -= 1;
            break;

        case keyComma:
            viewLoc.y += 1;
            break;
        
        case keyDot:
            viewLoc.y -= 1;
            break;
        }

        fillRectangle(windowTLX, windowTLY, windowBRX, windowBRY, false);

        for (unsigned char i = 0; i < voxelLen; i++){
            drawVoxel(voxelList[i]);
        }

        sprintf(vidmem + 1840, "(%d, %d, %d)", viewLoc.x, viewLoc.y, viewLoc.z);
    }
#endif

    voxelTriangle(5, 20, 15, 30, 20, -5, true);

    //sprintf(vidmem + 1840, "done");

    //drawMaze(2, 0, 78, 68);

    //traverseMaze();

    while (true);
}
