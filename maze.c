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
#define SCALING 34

#define TRIGPRECISION 4

#define LABDAPRECISION 6
#define LABDAPRECBITS (int)(1 << LABDAPRECISION)
#define NOLABDA -1000

const static unsigned char windowTLX = 8, windowTLY = 6, windowBRX = 72, windowBRY = 70;

const static unsigned char voxelSize = 8;

const signed int sinIndex[] = {0, 3, 6, 9, 11, 13, 15, 16, 16, 16, 15, 13, 11, 9, 6,
3, 0, -3, -6, -9, -11, -13, -15, -16, -16, -16, -15, -13, -11, -9, -6, -3};

const signed int cosIndex[] = {16, 16, 15, 13, 11, 9, 6, 3, 0, -3, -6, -9, -11, -13,
-15, -16, -16, -16, -15, -13, -11, -9, -6, -3, 0, 3, 6, 9, 11, 13, 15, 16};

unsigned char angleY = 0;
signed char sinAngle = 0, cosAngle = 16;

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

    struct screenVec2 s0;
    struct screenVec2 s1;
    struct screenVec2 s2;
    struct screenVec2 s3;

    unsigned char use2D;
    unsigned char wt;
};

//struct screenVec2* visited;
struct screenVec2* visited; //List of co-ordinates already visited
unsigned int visLen = 0;

unsigned char drawX = 0, drawY = 0;
unsigned char sizeX = 30, sizeY = 30; //Default size is of maze is 30x30;

//struct vec3 viewLoc = {10, 8, 0};

struct vec3 voxelList[] = {{-2, -4, 24}, {6, -4, 16}, {-10, -4, 16}}; //List of voxels te draw, will later be generated from the maze
unsigned int voxelLen = sizeof (voxelList) / sizeof (struct vec3);

//Add up 2 screen vectors
static struct screenVec2 addCoords(struct screenVec2 a, struct screenVec2 b){
    struct screenVec2 out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

//Substract 2 screen vectors
static struct screenVec2 subCoords(struct screenVec2 a, struct screenVec2 b){
    struct screenVec2 out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

//Add a co-ordinate to the list of co-ordinates visited
static inline void addVis(struct screenVec2 coords){
    setPixel(coords.x + OFFSET, coords.y, true);

    visited[visLen] = coords;
    visLen++;
}

//Delete a co-ordinate from the list of co-ordinates visited.
static inline void delVis(struct screenVec2* coords){
    setPixel(coords->x + OFFSET, coords->y, false);

    visLen--;
    *coords = visited[visLen];
}

//Check whether a co-ordinate is in the visited list
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

//Check whether co-ordinate can be drawn
static unsigned char checkLoc(struct screenVec2 coords){
    if (findVis(coords)) return false;
    if (getPixel(coords.x, coords.y)) return false;
    if (coords.x < drawX || coords.x > drawX + sizeX || coords.y < drawY || coords.y > drawY + sizeY) return false;
    
    return true;
}

//Generate the possible edges of a pixel
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

//Generate/draw a maze
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

//Walk through a maze
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

//Draw a sub-triangle of the face of a voxel.
//Uses two 3D co-ordinates and two single X co-ordinates which encode how far the triangle should be extended if it clips outside the screen.
unsigned char voxelTriangle(struct screenVec2 *s0, struct screenVec2* s1, unsigned char xExt0, unsigned char xExt1, unsigned char wt){
    unsigned char xBeg, yBeg, xa, ya, xb, yb;

    unsigned char xExtreme;
    if ((s0->y < MIDY) ^ GT_INT(s0->y, s1->y)){
        xExtreme = xExt0;

        xBeg = s1->x;
        yBeg = s1->y;
        
        xb = s0->x;
        yb = s0->y;
    }
    else{
        xExtreme = xExt1;
        
        xBeg = s0->x;
        yBeg = s0->y;
        
        xb = s1->x;
        yb = s1->y;
    }

    xa = xBeg;
    ya = yBeg;

    unsigned char horY;
    if (s0->y < MIDY) horY = MAX(ya,yb);
    else horY = MIN(ya, yb);

    //sprintf(vidmem + 1760, "(%d, %d) -> (%d, %d)", xa, ya, xb, yb);

    //if ((xa == xb) && (ya == yb)) return horY;

    signed int dx = abs(xb - xa);
    signed char sx = xa < xb ? 1 : -1;

    signed int dy = -abs(yb - ya);
    signed char sy = ya < yb ? 1 : -1;

    int error = dx + dy;
    int e2;

    horzLine(xa, xExtreme, ya, wt);

    while(true){
        //vertLine(xa, ya, horY, wt);
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
            horzLine(xa, xExtreme, ya, wt);
        }
    }

    //if (xBeg != xExt0) fillRectangle(xBeg, yBeg, xExt0, horY, wt);
    //if (xa != xExt1) fillRectangle(xa, ya, xExt1, horY, wt);

    vertLine(xExtreme, yBeg, horY, true);
    vertLine(xExtreme, ya, horY, true);

    //sprintf(vidmem + 1840, "(%u, %u), (%u, %u)", v0.x, v0.y, v1.x, v1.y);

    return horY;
}

//Calculate the following:
// /x\   /a\           / /d\   /a\ \ 
// |y| = |b| + labda * | |e| - |b| |
// \z/   \c/           \ \f/   \c/ /
struct vec3 labdaToVec(struct vec3* d0, struct vec3* d1, signed int labda){
    if ((labda > LABDAPRECBITS) || (labda & 0x8000))
        sprintf(vidmem + 1760, "Labda out of range"); //Should never happen

    struct vec3 out;

    out.x = d0->x + ((labda * (d1->x - d0->x)) >> LABDAPRECISION);
    out.y = d0->y + ((labda * (d1->y - d0->y)) >> LABDAPRECISION);
    out.z = d0->z + ((labda * (d1->z - d0->z)) >> LABDAPRECISION);

    return out;
}

//State encoding the positions a point could be in relation to the camera
enum direction{
    InScreen, OutOfScreenA, OutOfScreenB, BehindScreen
};

//Determine in which state a 3D co-ordinate is when compared to the X co-ordinate
enum direction inRangeVec3X(struct vec3* v0){
    if (v0->z & 0x8000) return BehindScreen;

    if (abs(v0->x) < abs(v0->z))
        return InScreen;
    else
        if (v0->x & 0x8000) return OutOfScreenA;
        else return OutOfScreenB;
}

//Determine in which state a 3D co-ordinate is when compared to the Y co-ordinate
enum direction inRangeVec3Y(struct vec3* v0){
    if (v0->z & 0x8000) return BehindScreen;

    if (abs(v0->y) < abs(v0->z))
        return InScreen;
    else
        if (v0->y & 0x8000) return OutOfScreenA;
        else return OutOfScreenB;
}

unsigned char inRangeTotal(struct vec3* v0){
    if (inRangeVec3X(v0) == InScreen && inRangeVec3Y(v0) == InScreen)
        return true;
    else
        return false;
}

//Determine the correct labdas for clipping the 3D vector onto the screen
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

//Clip a line between two 3D vectors, so that it will never:
//1: draw points which are outside the FOV
//2: experience a wrap-around because of integer arithmetic
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

//Draw the face of a voxel
void voxelFace(struct face *f0){
    unsigned char wt = f0->wt;

/*
    struct vec3 allVecs[4];
    allVecs[0] = f0->v0, allVecs[1] = f0->v1, allVecs[2] = f0->v2, allVecs[3] = f0->v3;

    for (unsigned char i = 0; i < 4; i++){
        //struct vec3 sub = {allVecs[i].x - viewLoc.x, allVecs[i].y - viewLoc.y, allVecs[i].z - viewLoc.z};
        struct vec3 sub = {allVecs[i].x, allVecs[i].y, allVecs[i].z};
        struct vec3 next = {(sub.x * cosAngle - sub.z * sinAngle), sub.y << 4, (sub.x * sinAngle + sub.z * cosAngle)};
        allVecs[i] = next;
    }
*/

    unsigned char xExt0 = 0, xExt1 = 0;
    unsigned char hor0 = 0, hor1 = 0;
    unsigned char ret0 = 0, ret1 = 0;

    if (f0->use2D){
        xExt0 = f0->s0.x;
        xExt1 = f0->s1.x;
    }
    else {
        ret0 = clipLine(&(f0->v0), &(f0->v1), &xExt0, &xExt1);
        ret1 = clipLine(&(f0->v2), &(f0->v3), &xExt0, &xExt1);

        f0->s0.x = MIDX + (f0->v0.x * SCALING) / f0->v0.z;
        f0->s0.y = MIDY + (f0->v0.y * SCALING) / f0->v0.z;

        f0->s1.x = MIDX + (f0->v1.x * SCALING) / f0->v1.z;
        f0->s1.y = MIDY + (f0->v1.y * SCALING) / f0->v1.z;

        f0->s2.x = MIDX + (f0->v2.x * SCALING) / f0->v2.z;
        f0->s2.y = MIDY + (f0->v2.y * SCALING) / f0->v2.z;

        f0->s3.x = MIDX + (f0->v3.x * SCALING) / f0->v3.z;
        f0->s3.y = MIDY + (f0->v3.y * SCALING) / f0->v3.z;
    }

    if (ret0 == 0) hor0 = voxelTriangle(&(f0->s0), &(f0->s1), xExt0, xExt1, wt);
    if (ret1 == 0) hor1 = voxelTriangle(&(f0->s2), &(f0->s3), xExt0, xExt1, wt);

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

//Calculate the midpoint of a face
void avgVec(struct face *f0, struct vec3 *v0){
    v0->x = ((f0->v0.x >> TRIGPRECISION) + (f0->v1.x >> TRIGPRECISION) + (f0->v2.x >> TRIGPRECISION) + (f0->v3.x >> TRIGPRECISION)) >> 2;
    v0->y = ((f0->v0.y >> TRIGPRECISION) + (f0->v1.y >> TRIGPRECISION) + (f0->v2.y >> TRIGPRECISION) + (f0->v3.y >> TRIGPRECISION)) >> 2;
    v0->z = ((f0->v0.z >> TRIGPRECISION) + (f0->v1.z >> TRIGPRECISION) + (f0->v2.z >> TRIGPRECISION) + (f0->v3.z >> TRIGPRECISION)) >> 2;
}

//Calculate the norm of a vector
inline int vec2Dist(struct vec3 *v0){
    return v0->x * v0->x + v0->y * v0->y + v0->z * v0->z;
}

//Compare two 3D co-ordinates by their distance to the origin (used during sorting)
int cmpVec3(struct vec3 *v0, struct vec3 *v1){
    return vec2Dist(v1) - vec2Dist(v0);
}

//Compare two faces by their distance to the origin (used during sorting)
int cmpFace(struct face *f0, struct face *f1){
    struct vec3 v0 = {0,0,0}, v1 = {0,0,0};
    avgVec(f0, &v0);
    avgVec(f1, &v1);
    return vec2Dist(&v1) - vec2Dist(&v0);
}

void convertFace2D(struct face *f0){
    if (!(f0->use2D)) return;

    f0->s0.x = MIDX + (f0->v0.x * SCALING) / f0->v0.z;
    f0->s1.x = MIDX + (f0->v1.x * SCALING) / f0->v1.z;
    f0->s2.x = f0->s0.x;
    f0->s3.x = f0->s1.x;

    f0->s0.y = MIDY + (f0->v0.y * SCALING) / f0->v0.z;
    f0->s1.y = MIDY + (f0->v1.y * SCALING) / f0->v1.z;
    f0->s2.y = MIDY + (f0->v2.y * SCALING) / f0->v2.z;
    f0->s3.y = MIDY + (f0->v3.y * SCALING) / f0->v3.z;
}

//Draw a voxel
void drawVoxel(struct vec3 *vector){
    struct vec3 points[8];
    const unsigned char pointInds[16] = {7,6,5,4,6,2,4,0,3,7,1,5,2,3,0,1};

    unsigned char use2D[8];
    unsigned char skipVoxel = true;

    for (unsigned char i = 0; i < 8; i++){
        int pointsAbsX = (i & 0b1) * voxelSize + vector->x;
        int pointsAbsY = ((i & 0b10) >> 1) * voxelSize + vector->y;
        int pointsAbsZ = ((i & 0b100) >> 2) * voxelSize + vector->z;

        struct vec3 sub = {pointsAbsX, pointsAbsY, pointsAbsZ};
        struct vec3 next = {(sub.x * cosAngle - sub.z * sinAngle), sub.y << TRIGPRECISION, (sub.x * sinAngle + sub.z * cosAngle)};

        points[i] = next;
        unsigned char irt = inRangeTotal(&next);

        use2D[i] = irt;

        skipVoxel &= ~irt;
    }

    if (skipVoxel) return;

    struct face faces[4];

    for (unsigned char i = 0; i < 4; i++){
        faces[i].v0 = points[pointInds[4*i + 0]];
        faces[i].v1 = points[pointInds[4*i + 1]];
        faces[i].v2 = points[pointInds[4*i + 2]];
        faces[i].v3 = points[pointInds[4*i + 3]];

        //unsigned char useAll = use2D[pointInds[4*i+0]] & use2D[pointInds[4*i+1]] & use2D[pointInds[4*i+2]] & use2D[pointInds[4*i+3]];

        faces[i].use2D = use2D[pointInds[4*i+0]] & use2D[pointInds[4*i+1]] & use2D[pointInds[4*i+2]] & use2D[pointInds[4*i+3]];
    }

    faces[0].wt = false;
    faces[1].wt = true;
    faces[2].wt = true;
    faces[3].wt = false;

    struct face relFaces[2];
    struct vec3 relScaled[2];

    struct vec3 scaled0 = {(faces[0].v0.x + faces[0].v1.x) >> 5, (faces[0].v0.y + faces[0].v1.y) >> 5, (faces[0].v0.z + faces[0].v1.z) >> 5};
    struct vec3 scaled1 = {(faces[1].v0.x + faces[1].v1.x) >> 5, (faces[1].v0.y + faces[1].v1.y) >> 5, (faces[1].v0.z + faces[1].v1.z) >> 5};
    struct vec3 scaled2 = {(faces[2].v0.x + faces[2].v1.x) >> 5, (faces[2].v0.y + faces[2].v1.y) >> 5, (faces[2].v0.z + faces[2].v1.z) >> 5};
    struct vec3 scaled3 = {(faces[3].v0.x + faces[3].v1.x) >> 5, (faces[3].v0.y + faces[3].v1.y) >> 5, (faces[3].v0.z + faces[3].v1.z) >> 5};

    if (cmpVec3(&scaled0, &scaled3) & 0x8000){
        relFaces[0] = faces[3];
        relScaled[0] = scaled3;
    }
    else{
        relFaces[0] = faces[0];
        relScaled[0] = scaled0;
    }

    if (cmpVec3(&scaled1, &scaled2) & 0x8000){
        relFaces[1] = faces[2];
        relScaled[1] = scaled2;
    }
    else{
        relFaces[1] = faces[1];
        relScaled[1] = scaled1;
    }

    if (relFaces[0].use2D)
        convertFace2D(relFaces + 0);
    else if (relFaces[1].use2D)
        convertFace2D(relFaces + 1);

    if (relFaces[0].use2D && relFaces[1].use2D){
        if (!memcmp(&relFaces[0].v0, &relFaces[1].v1, sizeof (struct vec3))){
            relFaces[1].s1 = relFaces[0].s0;
            relFaces[1].s3 = relFaces[0].s2;
    
            relFaces[1].s0.x = MIDX + (relFaces[1].v0.x * SCALING) / relFaces[1].v0.z;
            relFaces[1].s2.x = relFaces[1].s0.x;
    
            relFaces[1].s0.y = MIDY + (relFaces[1].v0.y * SCALING) / relFaces[1].v0.z;
            relFaces[1].s2.y = MIDY + (relFaces[1].v2.y * SCALING) / relFaces[1].v2.z;
        }
    
        if (!memcmp(&relFaces[0].v1, &relFaces[1].v0, sizeof (struct vec3))){
            relFaces[1].s0 = relFaces[0].s1;
            relFaces[1].s2 = relFaces[0].s3;
    
            relFaces[1].s1.x = MIDX + (relFaces[1].v1.x * SCALING) / relFaces[1].v1.z;
            relFaces[1].s3.x = relFaces[1].s1.x;
    
            relFaces[1].s1.y = MIDY + (relFaces[1].v1.y * SCALING) / relFaces[1].v1.z;
            relFaces[1].s3.y = MIDY + (relFaces[1].v3.y * SCALING) / relFaces[1].v3.z;
        }
    }

    if (cmpVec3(relScaled + 0, relScaled + 1) & 0x8000){
        voxelFace(relFaces + 0);
        voxelFace(relFaces + 1);
    }
    else{
        voxelFace(relFaces + 1);
        voxelFace(relFaces + 0);
    }
}

static void drawInfoScreen(void){
    drawText(10, 0, "3D walker", true, WHITETEXT);

    drawText(4, 9,  "This is nothing but a celebration of", false, WHITETEXT);
    drawText(4, 12, "what is possible on the P2000:", false, WHITETEXT);
    drawText(4, 15, "A full-3D voxel renderer, including", false, WHITETEXT);
    drawText(4, 18, "perspective, off-grid camera", false, WHITETEXT);
    drawText(4, 24, "positions and rotations, and filled", false, WHITETEXT);
    drawText(4, 27, "voxel sides. This should be regarded", false, WHITETEXT);
    drawText(4, 30, "as a showcase of the possibilities", false, WHITETEXT);
    drawText(4, 33, "that even old hardware offers when", false, WHITETEXT);
    drawText(4, 36, "pushed to the limit. Therefore, bugs", false, WHITETEXT);
    drawText(4, 39, "may very well exist.", false, WHITETEXT);

    drawText(4, 45, "Press the arrows to move around on", false, WHITETEXT);
    drawText(6, 48, "the xy-plane", false, WHITETEXT);
    drawText(4, 51, "Press the (,) and (.) keys to move", false, WHITETEXT);
    drawText(6, 54, "up and down", false, WHITETEXT);
    drawText(4, 57, "Press the A and S keys to rotate", false, WHITETEXT);
    drawText(6, 60, "the camera", false, WHITETEXT);
    drawText(4, 63, "Press the 0 key at any point to", false, WHITETEXT);
    drawText(6, 66, "exit", false, WHITETEXT);


    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

//Walk through maze in 3D
void maze(void){
    setWindow(2, 0, 79, 70);
    startGraphics(WHITEGFS);

    drawInfoScreen();

    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    signed int height = 0;

    srand(getTime());

    while (getKey() == keyEnter);

    while (true){
        unsigned char key;

        while ((key = getKey()) == keyNone);
        if (key == key0) break;

        fillRectangle(windowTLX, windowTLY, windowBRX, windowBRY, false);

        unsigned char walkState = 0xFF;


        switch (key)
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
            if (height == 4) break;
            height++;
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].y--;
            break;
        
        case 5:
            if (height == -4) break;
            height--;
            for (unsigned int i = 0; i < voxelLen; i++)
                voxelList[i].y++;
            break;
        }

        unsigned int startTime = getTime();

        //Since the viewing position only changes a small amount, the list will always be (nearly) sorted, so using qsort alone is a bad idea
        //Therefore we introduce a small amount of noise to the system
        for (unsigned char i = 0; i < 2; i++){
            unsigned int i = rand() % voxelLen, j = rand() % voxelLen;
            struct vec3 temp = {voxelList[i].x, voxelList[i].y, voxelList[i].z};
            voxelList[i] = voxelList[j];
            voxelList[j] = temp;
        }

        qsort(voxelList, voxelLen, sizeof(struct vec3), cmpVec3);

        for (unsigned char i = 0; i < voxelLen; i++){
            drawVoxel(voxelList + i);
        }

        //sprintf(vidmem + 1840, "Time: %d", getTime() - startTime);
    }

    setWindow(2, 0, 79, 70);
    startGraphics(WHITETEXT);
}
