#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#define OFFSET 80
#define MIDX 40
#define MIDY 38
#define SCALING 36

#define LABDAPRECISION 6
#define LABDAPRECBITS ((int)(1 << LABDAPRECISION))
#define NOLABDA -1000

const static unsigned char windowTLX = 6, windowTLY = 4, windowBRX = 74, windowBRY = 72;

const static unsigned char voxelSize = 8;

unsigned char angleY = 0;
signed char sinAngle, cosAngle;

const signed int sinIndex[] = {0, 3, 6, 9, 11, 13, 15, 16, 16, 16, 15, 13, 11, 9, 6,
3, 0, -3, -6, -9, -11, -13, -15, -16, -16, -16, -15, -13, -11, -9, -6, -3};

const signed int cosIndex[] = {16, 16, 15, 13, 11, 9, 6, 3, 0, -3, -6, -9, -11, -13,
-15, -16, -16, -16, -15, -13, -11, -9, -6, -3, 0, 3, 6, 9, 11, 13, 15, 16};

struct screenVec2{
    unsigned char x;
    unsigned char y;
};

struct vec3{
    signed int x;
    signed int y;
    signed int z;
};

struct face{
    struct vec3 v0;
    struct vec3 v1;
    struct vec3 v2;
    struct vec3 v3;
    unsigned char wt;
};

//struct screenVec2* visited;
struct screenVec2* visited;
unsigned int visLen = 0;

unsigned char drawX = 0, drawY = 0;
unsigned char sizeX = 30, sizeY = 30; //Default size is  30x30;

//struct vec3 viewLoc = {10, 8, 0};

struct vec3 voxelList[] = {{-2, -4, 24}, {6, -4, 16}, {-10, -4, 16}};
unsigned int voxelLen = sizeof (voxelList) / sizeof (struct vec3);

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

unsigned char voxelTriangle(struct vec3 *v0, struct vec3 *v1, unsigned char xExt0, unsigned char xExt1, unsigned char wt){
    unsigned char xBeg = MIDX + (v0->x * SCALING) / v0->z;
    unsigned char yBeg = MIDY + (v0->y * SCALING) / v0->z;

    unsigned char xa = xBeg;
    unsigned char xb = MIDX + (v1->x * SCALING) / v1->z;

    unsigned char ya = yBeg;
    unsigned char yb = MIDY + (v1->y * SCALING) / v1->z;

    unsigned char horY;
    if (LT_INT(v0->y, 0))
        horY = MAX(ya,yb);
    else
        horY = MIN(ya,yb);

    //sprintf(vidmem + 1760, "(%d, %d) -> (%d, %d)", xa, ya, xb, yb);

    //if ((xa == xb) && (ya == yb)) return horY;

    signed int dx = abs(xb - xa);
    signed char sx = xa < xb ? 1 : -1;

    signed int dy = -abs(yb - ya);
    signed char sy = ya < yb ? 1 : -1;

    int error = dx + dy;
    int e2;

    while(true){
        vertLine(xa, ya, horY, wt);
        setPixel(xa, ya, true);
        
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

    fillRectangle(xBeg, yBeg, xExt0, horY, wt);
    fillRectangle(xa, ya, xExt1, horY, wt);

    vertLine(xExt0, yBeg, horY, true);
    vertLine(xExt1, ya, horY, true);

    //sprintf(vidmem + 1840, "(%u, %u), (%u, %u)", v0.x, v0.y, v1.x, v1.y);

    return horY;
}

struct vec3 labdaToVec(struct vec3* d0, struct vec3* d1, signed int labda){
    if ((labda > LABDAPRECBITS) || (labda & 0x8000))
        sprintf(vidmem + 1760, "Weird labda");

    struct vec3 out;

    out.x = d0->x + ((labda * (d1->x - d0->x)) >> LABDAPRECISION);
    out.y = d0->y + ((labda * (d1->y - d0->y)) >> LABDAPRECISION);
    out.z = d0->z + ((labda * (d1->z - d0->z)) >> LABDAPRECISION);

    return out;
}

enum direction{
    InScreen, OutOfScreenA, OutOfScreenB, BehindScreen
};

enum direction inRangeVec3X(struct vec3* v0){
    if (v0->z & 0x8000) return BehindScreen;

    if (abs(v0->x) < abs(v0->z))
        return InScreen;
    else
        if (v0->x & 0x8000) return OutOfScreenA;
        else return OutOfScreenB;
}

enum direction inRangeVec3Y(struct vec3* v0){
    if (v0->z & 0x8000) return BehindScreen;

    if (abs(v0->y) < abs(v0->z))
        return InScreen;
    else
        if (v0->y & 0x8000) return OutOfScreenA;
        else return OutOfScreenB;
}

unsigned char findLabdas(struct vec3* d0, struct vec3* d1, signed int* labda0, signed int* labda1, unsigned char y){
    signed int relCoord0 = y ? d0->y : d0->x;
    signed int relCoord1 = y ? d1->y : d1->x;

    signed int labdaA = ((relCoord0 + d0->z) << LABDAPRECISION) / (relCoord0 + d0->z - relCoord1 - d1->z); //Left screen bound
    signed int labdaB = ((relCoord0 - d0->z) << LABDAPRECISION) / (relCoord0 - d0->z - relCoord1 + d1->z); //Right screen bound

    signed int labdaMin = MIN(labdaA, labdaB); //Note: DO NOT USE MIN_INT
    signed int labdaMax = LABDAPRECBITS - MIN(LABDAPRECBITS - labdaA, LABDAPRECBITS - labdaB);

    enum direction r0 = y ? inRangeVec3Y(d0) : inRangeVec3X(d0);
    enum direction r1 = y ? inRangeVec3Y(d1) : inRangeVec3X(d1);

    if ((r0 == r1) && (r0 == BehindScreen || r0 == OutOfScreenA || r0 == OutOfScreenB)) return false;
    if ((r0 == OutOfScreenA || r0 == OutOfScreenB) && r1 == BehindScreen) return false; //Not actually correct
    if ((r1 == OutOfScreenA || r1 == OutOfScreenB) && r0 == BehindScreen) return false; //Not actually correct
    if ((r0 == r1) && (r0 == InScreen)){
        *labda0 = 0;
        *labda1 = LABDAPRECBITS;
        return true;
    }
    if ((r0 == OutOfScreenA || r0 == OutOfScreenB) && r1 == InScreen){
        *labda0 = labdaMin;
        *labda1 = LABDAPRECBITS;
        return true;
    }
    if ((r1 == OutOfScreenA || r1 == OutOfScreenB) && r0 == InScreen){
        *labda0 = 0;
        *labda1 = labdaMin;
        return true;
    }
    if (r0 == BehindScreen && r1 == InScreen){
        *labda0 = labdaMax;
        *labda1 = LABDAPRECBITS;
        return true;
    }
    if (r1 == BehindScreen && r0 == InScreen){
        *labda0 = 0;
        *labda1 = labdaMin;
        return true;
    }
    if (r0 == OutOfScreenA && r1 == OutOfScreenB){
        *labda0 = labdaA;
        *labda1 = labdaB;
        return true;
    }
    if (r0 == OutOfScreenB && r1 == OutOfScreenA){
        *labda0 = labdaB;
        *labda1 = labdaA;
        return true;
    }
    //*labda0 = 0;
    //*labda1 = 0;

    return false;
}

unsigned char clipLine(struct vec3* d0, struct vec3* d1, unsigned char* x0, unsigned char* x1){
    signed int labda0, labda1;

    if (!findLabdas(d0, d1, &labda0, &labda1, false)) return 1;

    struct vec3 clippedX0, clippedX1;
    clippedX0 = labdaToVec(d0, d1, labda0);
    clippedX1 = labdaToVec(d0, d1, labda1);

    *x0 = MIDX + (clippedX0.x * SCALING) / clippedX0.z;
    *x1 = MIDX + (clippedX1.x * SCALING) / clippedX1.z;

    if (!findLabdas(&clippedX0, &clippedX1, &labda0, &labda1, true)) return 2;
    
    struct vec3 clippedY0, clippedY1;
    clippedY0 = labdaToVec(&clippedX0, &clippedX1, labda0);
    clippedY1 = labdaToVec(&clippedX0, &clippedX1, labda1);

    *d0 = clippedY0;
    *d1 = clippedY1;

    return 0;
}

void voxelFace(struct face *f0){
    unsigned char wt = f0->wt;
    struct vec3 allVecs[4];
    allVecs[0] = f0->v0, allVecs[1] = f0->v1, allVecs[2] = f0->v2, allVecs[3] = f0->v3;

    for (unsigned char i = 0; i < 4; i++){
        //struct vec3 sub = {allVecs[i].x - viewLoc.x, allVecs[i].y - viewLoc.y, allVecs[i].z - viewLoc.z};
        struct vec3 sub = {allVecs[i].x, allVecs[i].y, allVecs[i].z};
        struct vec3 next = {(sub.x * cosAngle - sub.z * sinAngle), sub.y << 4, (sub.x * sinAngle + sub.z * cosAngle)};
        allVecs[i] = next;
    }

    unsigned char xExt0 = 0, xExt1 = 0;
    unsigned char hor0 = 0, hor1 = 0;

    unsigned char ret0 = clipLine(allVecs + 0, allVecs + 1, &xExt0, &xExt1);
    if (ret0 == 0) hor0 = voxelTriangle(allVecs + 0, allVecs + 1, xExt0, xExt1, wt);

    unsigned char ret1 = clipLine(allVecs + 2, allVecs + 3, &xExt0, &xExt1);
    if (ret1 == 0) hor1 = voxelTriangle(allVecs + 2, allVecs + 3, xExt0, xExt1, wt);

    unsigned char topY = 0, botY = 0;

    switch (ret0)
    {
    case 0: topY = hor0 - 1; break;
    case 1: return;
    case 2: topY = MIDY + SCALING; break;
    }

    switch (ret1)
    {
    case 0: botY = hor1 + 1; break;
    case 1: return;
    case 2: botY = MIDY - SCALING; break;
    }

    fillRectangle(xExt0, topY, xExt1, botY, wt);
    vertLine(xExt0, topY, botY, true);
    vertLine(xExt1, topY, botY, true);

    //sprintf(vidmem + 1760, "%d, %d, %d, %d", xExt0, topY, xExt1, botY);

    return;
}

void avgVec(struct face *f0, struct vec3 *v0){
    v0->x = (f0->v0.x + f0->v1.x + f0->v2.x + f0->v3.x) >> 2;
    v0->y = (f0->v0.y + f0->v1.y + f0->v2.y + f0->v3.y) >> 2;
    v0->z = (f0->v0.z + f0->v1.z + f0->v2.z + f0->v3.z) >> 2;
}

inline int vec2Dist(struct vec3 *v0){
    return v0->x * v0->x + v0->y * v0->y + v0->z * v0->z;
}

int cmpVec3(struct vec3 *v0, struct vec3 *v1){
    return vec2Dist(v1) - vec2Dist(v0);
}

int cmpFace(struct face *f0, struct face *f1){
    struct vec3 v0 = {0,0,0}, v1 = {0,0,0};
    avgVec(f0, &v0);
    avgVec(f1, &v1);
    return vec2Dist(&v1) - vec2Dist(&v0);
}

void drawVoxel(struct vec3 *vector){
    struct vec3 points[8];

    for (unsigned char i = 0; i < 8; i++){
        points[i].x = (i & 0b1) * voxelSize + vector->x;
        points[i].y = ((i & 0b10) >> 1) * voxelSize + vector->y;
        points[i].z = ((i & 0b100) >> 2) * voxelSize + vector->z;
    }

    struct face faces[4];

    faces[0].v0 = points[7], faces[0].v1 = points[6], faces[0].v2 = points[5], faces[0].v3 = points[4], faces[0].wt = false;
    faces[1].v0 = points[6], faces[1].v1 = points[2], faces[1].v2 = points[4], faces[1].v3 = points[0], faces[1].wt = true;
    faces[2].v0 = points[3], faces[2].v1 = points[7], faces[2].v2 = points[1], faces[2].v3 = points[5], faces[2].wt = true;
    faces[3].v0 = points[2], faces[3].v1 = points[3], faces[3].v2 = points[0], faces[3].v3 = points[1], faces[3].wt = false;

    qsort(faces, 4, sizeof(struct face), cmpFace);

    for (unsigned char i = 0; i < 4; i++){
        voxelFace(faces + i);
    }

    //voxelFace(points[7], points[6], points[5], points[4], false);
    //voxelFace(points[6], points[2], points[4], points[0], true);
    //voxelFace(points[3], points[7], points[1], points[5], true);
    //voxelFace(points[2], points[3], points[0], points[1], false);
}

void initializeVoxels(void){
    for (unsigned int i = 0; i < voxelLen; i++){
        voxelList[i].x = voxelList[i].x;
        voxelList[i].y = voxelList[i].y;
        voxelList[i].z = voxelList[i].z;
    }
}

void maze(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    srand(getTime());

    initializeVoxels();

#if 1
    while (true){
        while (getKey() == keyNone);

        fillRectangle(windowTLX, windowTLY, windowBRX, windowBRY, false);

        unsigned int startTime = getTime();

        unsigned char walkState = 0xFF;

        switch (getKey())
        {
        case keyUp:
            if (4 < angleY && angleY <= 12) walkState = 0;
            if (12 < angleY && angleY <= 20) walkState = 1;
            if (20 < angleY && angleY <= 28) walkState = 2;
            if (angleY <= 4 || 28 < angleY) walkState = 3;
            break;
        
        case keyDown:
            if (4 < angleY && angleY <= 12) walkState = 2;
            if (12 < angleY && angleY <= 20) walkState = 3;
            if (20 < angleY && angleY <= 28) walkState = 0;
            if (angleY <= 4 || 28 < angleY) walkState = 1;
            break;

        case keyRight:
            if (4 < angleY && angleY <= 12) walkState = 1;
            if (12 < angleY && angleY <= 20) walkState = 2;
            if (20 < angleY && angleY <= 28) walkState = 3;
            if (angleY <= 4 || 28 < angleY) walkState = 0;
            break;
        
        case keyLeft:
            if (4 < angleY && angleY <= 12) walkState = 3;
            if (12 < angleY && angleY <= 20) walkState = 0;
            if (20 < angleY && angleY <= 28) walkState = 1;
            if (angleY <= 4 || 28 < angleY) walkState = 2;
            break;

        case keyComma:
            walkState = 4;
            break;
        
        case keyDot:
            walkState = 5;
            break;

        case keyS:
            if (angleY == 31) angleY = 0;
            else angleY++;
            sinAngle = sinIndex[angleY];
            cosAngle = cosIndex[angleY];
            break;

        case keyA:
            if (angleY == 0) angleY = 31;
            else angleY--;
            sinAngle = sinIndex[angleY];
            cosAngle = cosIndex[angleY];
            break;
        }

        //viewLoc.x++ -> 0
        //viewLoc.z-- -> 1
        //viewLoc.x-- -> 2
        //viewLoc.z++ -> 3
        //viewLoc.y++ -> 4
        //viewLoc.y-- -> 5

        switch (walkState)
        {
        case 0:
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].x--;
            break;
        
        case 1:
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].z++;
            break;
        
        case 2:
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].x++;
            break;
        
        case 3:
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].z--;
            break;
        
        case 4:
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].y--;
            break;
        
        case 5:
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].y++;
            break;
        }

        //Since the viewing position only changes a small amount, the list will always be (nearly) sorted, so using qsort is a bad idea
        qsort(voxelList, voxelLen, sizeof(struct vec3), cmpVec3);

        for (unsigned char i = 0; i < voxelLen; i++){
            drawVoxel(voxelList + i);
        }

        sprintf(vidmem + 1840, "Time: %d", getTime() - startTime);
    }
#endif

    //voxelTriangle(-5, 20, -10, 30, -20, 40, true);

    //sprintf(vidmem + 1840, "done");

    //drawMaze(2, 0, 78, 68);

    //traverseMaze();

    while (1);
}
