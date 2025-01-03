#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

struct hLine{
    unsigned char x0Index;
    unsigned char x1Index;
    unsigned char yIndex;
};

struct vLine{
    unsigned char xIndex;
    unsigned char y0Index;
    unsigned char y1Index;
};

//Co-ordinates of the entire window
const unsigned char walkerTLX = 2, walkerTLY = 0, walkerBRX = 79, walkerBRY = 68;
//Co-ordinates of the rolling window
const unsigned char rollTLX = 4, rollTLY = 3, rollBRX = 77, rollBRY = 65;

const unsigned char middleX = 41, middleY = 34;

struct hLine* horizontals;
struct vLine* verticals;

unsigned int xCoords[] = {
52, 54, 56, 62, 92, 94, 96, 100, 102, 104, 106, 108, 110, 114, 116,
118, 120, 122, 126, 130, 138, 140, 142, 144, 146, 148, 150, 154, 164,
166, 168, 170, 172, 174, 176, 178, 182, 184, 186, 188, 190, 192, 194,
196, 200, 202, 206, 208, 210, 214, 218, 220, 222, 224, 228, 230, 232,
236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 258, 260, 262, 264,
266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286, 288, 290, 292,
294, 296, 298, 300, 310, 312, 314, 318, 320, 324, 326, 328, 330, 332,
334, 336, 340, 350, 354, 356, 362, 370, 372, 374, 378, 380, 382, 384,
386, 388, 390, 392, 394, 396, 398, 400, 404, 406, 408, 410, 414, 418,
420, 424, 426, 430, 432, 434, 438, 440, 442, 454, 456, 458, 460, 462,
464, 466, 470, 472, 476, 482, 484, 486, 490, 492, 496, 498, 500, 502,
506, 508, 510, 512, 514, 516, 520, 528, 530, 532, 536, 542, 544, 546,
548, 550, 552, 554, 562, 564, 566, 568, 572, 574, 576, 580, 582, 584,
590, 594, 596, 600, 602, 606, 610, 612, 622, 626, 630, 636, 640, 642,
646, 650, 652, 654, 658, 660, 662, 666, 672, 678, 680, 682, 686, 692,
694, 696, 698, 704, 706, 708, 718, 722, 724, 746, 756, 780, 794, 806,
822, 824};

unsigned int yCoords[] = {
61, 62, 63, 66, 67, 68, 70, 75, 76, 80, 81, 82, 89, 98, 100, 103,
105, 107, 108, 109, 110, 111, 112, 113, 114, 116, 117, 121, 122, 124,
125, 126, 127, 128, 129, 131, 132, 135, 136, 138, 139, 140, 141, 144,
145, 146, 147, 149, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162,
163, 164, 165, 167, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
179, 180, 181, 183, 188, 189, 190, 192, 193, 195, 196, 197, 198, 199,
200, 202, 203, 204, 205, 206, 207, 208, 212, 213, 214, 215, 216, 217,
218, 219, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
233, 234, 235, 236, 238, 239, 240, 241, 243, 244, 247, 248, 249, 250,
251, 252, 253, 255, 256, 259, 260, 261, 262, 263, 265, 266, 267, 268,
269, 270, 271, 273, 275, 278, 279, 280, 282, 285, 286, 291, 292, 296,
297, 298, 299, 300, 301, 302, 303, 307, 308, 309, 313, 314, 315, 318,
319, 320, 321, 322, 325, 326, 327, 328, 329, 330, 331, 333, 335, 337,
345, 346};

struct hLine hLines0Inds[] = {
  {195, 197, 1}, {199, 226, 1}, {200, 210, 10}, {200, 210, 13}, {200, 
  210, 25}, {211, 212, 45}, {215, 225, 45}, {85, 113, 46}, {118, 120, 
  46}, {123, 195, 46}, {200, 211, 74}, {0, 85, 78}, {85, 104, 
  78}, {130, 142, 80}, {144, 158, 80}, {160, 177, 80}, {179, 182, 
  80}, {183, 195, 80}, {200, 211, 84}, {121, 124, 92}, {125, 133, 
  92}, {138, 151, 92}, {154, 174, 92}, {176, 180, 92}, {115, 121, 
  95}, {200, 211, 98}, {127, 133, 99}, {117, 121, 100}, {137, 140, 
  100}, {127, 133, 106}, {210, 225, 109}, {0, 2, 110}, {3, 7, 
  110}, {139, 146, 111}, {19, 44, 115}, {48, 60, 115}, {62, 85, 
  115}, {200, 211, 115}, {131, 133, 118}, {112, 124, 119}, {121, 121, 
  119}, {125, 127, 119}, {7, 11, 120}, {13, 19, 120}, {137, 140, 
  121}, {38, 40, 125}, {43, 47, 125}, {49, 53, 125}, {56, 60, 
  125}, {0, 7, 127}, {60, 79, 128}, {79, 80, 128}, {84, 85, 
  128}, {200, 202, 128}, {204, 208, 128}, {209, 211, 128}, {112, 132, 
  129}, {138, 151, 129}, {153, 173, 129}, {176, 181, 129}, {185, 191, 
  129}, {192, 195, 129}, {79, 85, 135}, {200, 211, 135}, {137, 151, 
  139}, {154, 166, 139}, {168, 187, 139}, {188, 192, 139}, {194, 195, 
  139}, {17, 19, 142}, {19, 34, 142}, {38, 61, 142}, {63, 76, 
  142}, {78, 81, 142}, {83, 85, 142}, {122, 129, 145}, {112, 122, 
  147}, {200, 211, 148}, {0, 8, 151}, {10, 14, 151}, {16, 29, 
  151}, {31, 35, 151}, {39, 46, 151}, {46, 85, 153}, {200, 210, 
  155}, {108, 115, 158}, {127, 127, 158}, {127, 127, 158}, {116, 128, 
  159}, {132, 139, 159}, {51, 61, 161}, {78, 85, 161}, {109, 115, 
  162}, {46, 51, 164}, {58, 61, 165}, {64, 66, 165}, {67, 73, 
  165}, {76, 78, 165}, {12, 18, 166}, {85, 88, 166}, {88, 89, 
  167}, {93, 97, 167}, {101, 102, 167}, {194, 196, 167}, {200, 211, 
  167}, {211, 212, 167}, {215, 220, 167}, {221, 223, 167}, {224, 225, 
  167}, {139, 193, 168}, {108, 109, 169}, {58, 65, 170}, {33, 35, 
  171}, {37, 46, 171}, {216, 217, 172}, {218, 219, 172}, {107, 112, 
  175}, {115, 116, 175}, {119, 128, 175}, {132, 139, 175}, {203, 205, 
  179}, {206, 213, 179}, {214, 216, 179}, {0, 89, 184}, {93, 97, 
  184}, {101, 139, 184}, {195, 226, 185}};

struct vLine vLines0Inds[] = {
  {0, 78, 133}, {0, 137, 185}, {7, 110, 144}, {7, 150, 151}, {12, 151, 
  166}, {16, 166, 185}, {18, 151, 166}, {19, 114, 116}, {19, 119, 
  142}, {22, 78, 115}, {33, 115, 117}, {33, 122, 142}, {33, 152, 
  185}, {38, 171, 184}, {39, 124, 142}, {46, 125, 142}, {46, 152, 
  184}, {49, 78, 80}, {49, 86, 115}, {51, 160, 162}, {51, 163, 
  164}, {53, 126, 141}, {57, 153, 161}, {58, 78, 105}, {58, 111, 
  115}, {58, 164, 185}, {60, 125, 142}, {65, 164, 170}, {68, 165, 
  184}, {69, 128, 142}, {78, 160, 162}, {78, 163, 184}, {79, 128, 
  142}, {85, 45, 77}, {85, 76, 118}, {85, 125, 146}, {85, 149, 
  154}, {85, 156, 166}, {88, 167, 185}, {102, 167, 184}, {103, 46, 
  78}, {107, 174, 184}, {108, 157, 169}, {109, 162, 169}, {112, 118, 
  129}, {112, 128, 147}, {113, 130, 147}, {114, 129, 147}, {115, 95, 
  119}, {115, 129, 147}, {116, 129, 147}, {116, 156, 175}, {117, 100, 
  119}, {117, 129, 147}, {118, 129, 147}, {119, 129, 147}, {121, 118, 
  119}, {122, 92, 112}, {122, 145, 147}, {124, 175, 177}, {124, 181, 
  184}, {127, 92, 129}, {127, 158, 158}, {127, 158, 158}, {128, 156, 
  162}, {128, 164, 182}, {129, 145, 147}, {130, 45, 80}, {132, 118, 
  147}, {132, 156, 162}, {132, 164, 176}, {132, 180, 184}, {133, 92, 
  94}, {133, 98, 101}, {133, 105, 108}, {133, 112, 119}, {137, 121, 
  123}, {137, 126, 129}, {138, 92, 94}, {138, 98, 100}, {139, 100, 
  121}, {139, 139, 168}, {139, 167, 184}, {145, 46, 80}, {146, 92, 
  129}, {155, 139, 168}, {161, 45, 80}, {170, 93, 129}, {170, 140, 
  167}, {180, 44, 49}, {180, 54, 80}, {180, 92, 129}, {185, 80, 
  85}, {185, 89, 129}, {189, 139, 167}, {195, 1, 132}, {195, 138, 
  176}, {195, 183, 185}, {200, 1, 8}, {200, 9, 14}, {200, 15, 
  79}, {200, 83, 99}, {200, 102, 129}, {200, 135, 145}, {200, 147, 
  156}, {200, 160, 166}, {203, 178, 184}, {205, 11, 13}, {205, 84, 
  97}, {205, 149, 155}, {207, 115, 128}, {210, 1, 6}, {210, 7, 
  17}, {210, 22, 36}, {210, 42, 51}, {210, 60, 80}, {211, 83, 
  102}, {211, 107, 131}, {211, 134, 139}, {211, 143, 162}, {211, 163, 
  167}, {211, 180, 184}, {216, 167, 173}, {216, 178, 185}, {219, 167, 
  184}, {222, 167, 185}, {226, 1, 11}, {226, 12, 61}, {226, 66, 
  121}, {226, 125, 185}};

struct hLine hLines1Inds[] = {
  {86, 196, 1}, {196, 210, 1}, {181, 211, 17}, {1, 86, 19}, {131, 140, 
  19}, {138, 147, 31}, {190, 196, 31}, {138, 140, 39}, {190, 210, 
  43}, {147, 148, 45}, {150, 165, 45}, {167, 171, 45}, {140, 147, 
  46}, {184, 189, 47}, {7, 20, 48}, {23, 36, 48}, {40, 42, 48}, {45, 
  59, 48}, {63, 81, 48}, {84, 87, 48}, {1, 5, 49}, {175, 184, 
  49}, {147, 159, 54}, {138, 140, 57}, {175, 184, 61}, {113, 131, 
  63}, {162, 164, 63}, {169, 172, 63}, {187, 198, 63}, {208, 210, 
  63}, {1, 8, 64}, {50, 62, 64}, {65, 76, 64}, {79, 82, 64}, {85, 87, 
  64}, {87, 104, 64}, {138, 149, 64}, {151, 155, 64}, {158, 161, 
  64}, {15, 21, 65}, {15, 21, 76}, {61, 87, 76}, {191, 196, 76}, {141,
   152, 77}, {155, 167, 77}, {170, 172, 77}, {178, 184, 77}, {186, 
  190, 77}, {51, 61, 78}, {171, 175, 82}, {177, 178, 82}, {136, 141, 
  87}, {195, 210, 87}, {50, 53, 88}, {55, 74, 88}, {77, 82, 88}, {86, 
  134, 88}, {15, 21, 89}, {1, 8, 91}, {178, 183, 99}, {57, 74, 
  104}, {78, 92, 104}, {94, 105, 104}, {106, 123, 104}, {125, 127, 
  104}, {129, 141, 104}, {1, 5, 105}, {7, 20, 105}, {23, 37, 
  105}, {40, 54, 105}, {141, 211, 113}, {1, 90, 135}, {90, 98, 
  135}, {98, 142, 135}};

struct vLine vLines1Inds[] = {
  {1, 19, 72}, {1, 72, 136}, {4, 49, 52}, {4, 58, 63}, {4, 90, 96}, {5,
   102, 105}, {8, 20, 49}, {8, 63, 75}, {8, 80, 90}, {8, 105, 
  136}, {15, 65, 67}, {15, 73, 81}, {15, 87, 89}, {21, 65, 88}, {24, 
  20, 49}, {25, 105, 135}, {41, 20, 48}, {41, 105, 135}, {51, 64, 
  88}, {56, 78, 88}, {57, 19, 48}, {58, 104, 135}, {61, 64, 78}, {70, 
  64, 76}, {79, 19, 48}, {79, 104, 135}, {80, 64, 76}, {81, 76, 
  88}, {86, 2, 19}, {87, 16, 48}, {87, 64, 88}, {96, 104, 135}, {104, 
  64, 88}, {107, 104, 135}, {113, 63, 88}, {116, 63, 88}, {118, 63, 
  88}, {120, 63, 88}, {122, 63, 88}, {124, 63, 87}, {126, 104, 
  135}, {131, 1, 19}, {131, 63, 88}, {138, 31, 36}, {140, 19, 
  22}, {140, 28, 31}, {140, 39, 57}, {141, 77, 134}, {147, 30, 
  63}, {152, 54, 64}, {155, 45, 54}, {156, 78, 113}, {157, 1, 
  30}, {157, 35, 45}, {159, 45, 64}, {163, 45, 63}, {171, 1, 
  63}, {171, 78, 113}, {175, 47, 53}, {175, 59, 64}, {178, 77, 
  93}, {178, 82, 82}, {178, 97, 100}, {181, 1, 17}, {181, 99, 
  112}, {183, 77, 100}, {184, 47, 64}, {189, 37, 63}, {190, 17, 
  26}, {190, 29, 34}, {190, 76, 113}, {195, 76, 112}, {196, 17, 
  43}, {198, 43, 50}, {198, 55, 63}, {201, 17, 43}, {201, 87, 
  112}, {205, 17, 43}, {205, 87, 112}, {208, 43, 63}, {211, 0, 114}};

struct hLine hLines2Inds[] = {
  {2, 88, 4}, {2, 143, 4}, {134, 143, 18}, {2, 5, 21}, {128, 130, 
  21}, {132, 134, 21}, {58, 77, 22}, {80, 95, 22}, {98, 107, 
  22}, {110, 126, 22}, {5, 6, 23}, {9, 21, 23}, {24, 38, 23}, {42, 55,
   23}, {2, 9, 32}, {141, 143, 32}, {16, 23, 33}, {30, 32, 33}, {35, 
  41, 33}, {49, 61, 33}, {64, 76, 33}, {79, 82, 33}, {82, 88, 
  33}, {99, 136, 33}, {23, 31, 35}, {30, 39, 39}, {16, 23, 44}, {49, 
  60, 46}, {60, 83, 46}, {28, 40, 53}, {99, 136, 54}, {142, 143, 
  54}, {49, 52, 55}, {54, 72, 55}, {75, 78, 55}, {81, 83, 55}, {86, 
  88, 55}, {16, 27, 56}, {39, 41, 56}, {2, 10, 58}, {128, 130, 
  68}, {133, 134, 68}, {59, 77, 69}, {80, 94, 69}, {98, 107, 
  69}, {110, 126, 69}, {5, 6, 70}, {9, 21, 70}, {25, 38, 70}, {42, 56,
   70}, {135, 143, 71}, {2, 5, 72}, {2, 143, 103}, {91, 100, 
  103}, {100, 143, 103}, {2, 91, 104}};

struct vLine vLines2Inds[] = {
  {2, 5, 41}, {2, 41, 104}, {5, 20, 24}, {5, 27, 32}, {6, 57, 62}, {6, 
  69, 73}, {9, 4, 22}, {9, 32, 35}, {9, 39, 57}, {10, 70, 103}, {16, 
  38, 50}, {23, 33, 56}, {26, 3, 22}, {26, 70, 103}, {28, 53, 
  56}, {30, 33, 40}, {40, 33, 56}, {43, 3, 22}, {43, 70, 103}, {50, 
  33, 46}, {52, 46, 56}, {60, 5, 23}, {60, 32, 46}, {60, 69, 
  103}, {71, 34, 55}, {78, 45, 56}, {81, 33, 45}, {82, 4, 22}, {82, 
  45, 55}, {82, 69, 102}, {88, 3, 4}, {88, 32, 55}, {99, 4, 22}, {99, 
  33, 54}, {99, 70, 104}, {111, 5, 22}, {111, 68, 102}, {125, 33, 
  55}, {127, 33, 54}, {128, 32, 54}, {129, 4, 22}, {129, 68, 
  103}, {130, 32, 54}, {132, 32, 54}, {133, 32, 54}, {134, 17, 
  22}, {135, 67, 71}, {143, 4, 103}};

//Length of the specified tables
unsigned char horizontalsLen, verticalsLen;
unsigned char horizontals0Len = sizeof(hLines0Inds) / sizeof(struct hLine), verticals0Len = sizeof(vLines0Inds) / sizeof(struct vLine);
unsigned char horizontals1Len = sizeof(hLines1Inds) / sizeof(struct hLine), verticals1Len = sizeof(vLines1Inds) / sizeof(struct vLine);
unsigned char horizontals2Len = sizeof(hLines2Inds) / sizeof(struct hLine), verticals2Len = sizeof(vLines2Inds) / sizeof(struct vLine);

//Draws a horizontal line using the normal parameters, except using ints and including an "offset" per axis (position of the frame)
void horzLineAtXY(unsigned char x0Index, unsigned char x1Index, unsigned int xPos, unsigned char yIndex, unsigned int yPos){
    unsigned int x0 = xCoords[x0Index];
    unsigned int x1 = xCoords[x1Index];
    unsigned int y  = yCoords[yIndex];

    if (x0 > xPos + rollBRX) return;
    if (x1 < xPos + rollTLX) return;
    if (y < yPos + rollTLY || y > yPos + rollBRY) return;

    unsigned char startX = x0 - xPos;
    unsigned char endX = x1 - xPos;
    
    if (x0 < xPos + rollTLX) startX = rollTLX;
    if (x1 > xPos + rollBRX) endX = rollBRX;

    horzLine(startX, endX, y - yPos, true);
}

//Draws a vertical line using the normal parameters, except using ints and including an "offset" per axis (position of the frame)
void vertLineAtXY(unsigned char xIndex, unsigned int xPos, unsigned char y0Index, unsigned char y1Index, unsigned int yPos){
    unsigned int x  = xCoords[xIndex];
    unsigned int y0 = yCoords[y0Index];
    unsigned int y1 = yCoords[y1Index];

    if (x < xPos + rollTLX || x > xPos + rollBRX) return;
    if (y0 > yPos + rollBRY) return;
    if (y1 < yPos + rollTLY) return;

    unsigned char startY = y0 - yPos;
    unsigned char endY = y1 - yPos;

    if (y0 < yPos + rollTLY) startY = rollTLY;
    if (y1 > yPos + rollBRY) endY = rollBRY;

    vertLine(x - xPos, startY, endY, true);
}

//Draws a horizontal line using the normal parameters, except using ints and including an "offset" on the x-axis (position of the frame)
void horzLineAtX(unsigned char x0Index, unsigned char x1Index, unsigned int xPos, unsigned char y){
    unsigned int x0 = xCoords[x0Index];
    unsigned int x1 = xCoords[x1Index];

    if (x0 > xPos + rollBRX) return;
    if (x1 < xPos + rollTLX) return;
    if (y < rollTLY || y > rollBRY) return;
    
    unsigned char startX = x0 - xPos;
    unsigned char endX = x1 - xPos;
    
    if (x0 < xPos + rollTLX) startX = rollTLX;
    if (x1 > xPos + rollBRX) endX = rollBRX;

    horzLine(startX, endX, y, true);
}

//Draws a vertical line using the normal parameters, except using ints and including an "offset" on the y-axis (position of the frame)
void vertLineAtY(unsigned char x, unsigned char y0Index, unsigned char y1Index, unsigned int yPos){
    unsigned int y0 = yCoords[y0Index];
    unsigned int y1 = yCoords[y1Index];

    if (x < rollTLX || x > rollBRX) return;
    if (y0 > yPos + rollBRY) return;
    if (y1 < yPos + rollTLY) return;

    unsigned char startY = y0 - yPos;
    unsigned char endY = y1 - yPos;

    if (y0 < yPos + rollTLY) startY = rollTLY;
    if (y1 > yPos + rollBRY) endY = rollBRY;

    vertLine(x, startY, endY, true);
}

//Sets a given pixel using the normal parameters, and including an offset on the x-axis
void setPixelAtX(unsigned char xIndex, unsigned int xPos, unsigned char y){
    unsigned int x = xCoords[xIndex];

    if (x < xPos || x > xPos + 255) return;
    setPixel(x - xPos, y, true);
}

//Sets a given pixel using the normal parameters, and including an offset on the y-axis
void setPixelAtY(unsigned char x, unsigned char yIndex, unsigned int yPos){
    unsigned int y = yCoords[yIndex];

    if (y < yPos || y > yPos + 255) return;
    setPixel(x, y - yPos, true);
}

//Main function for walking through a map
void walker(void){
    setWindow(walkerTLX, walkerTLY, walkerBRX, walkerBRY);
    startGraphics();

    unsigned char floor = 0;

    switch (floor)
    {
    case 0:
        horizontals = hLines0Inds;
        horizontalsLen = horizontals0Len;
        verticals = vLines0Inds;
        verticalsLen = verticals0Len;
        break;
    
    case 1:
        horizontals = hLines1Inds;
        horizontalsLen = horizontals1Len;
        verticals = vLines1Inds;
        verticalsLen = verticals1Len;
        break;
    
    case 2:
        horizontals = hLines2Inds;
        horizontalsLen = horizontals2Len;
        verticals = vLines2Inds;
        verticalsLen = verticals2Len;
        break;
    }

    rectangle(2, 1, 79, 67, true);

    unsigned int posX = 120, posY = 120;

    struct hLine* topEnd = horizontals - 1;
    struct hLine* botEnd = horizontals + horizontalsLen;
    struct vLine* leftEnd = verticals - 1;
    struct vLine* rightEnd = verticals + verticalsLen;
    
    struct hLine* topHs = topEnd + 1;
    struct hLine* botHs = botEnd - 1;
    struct vLine* leftVs = leftEnd + 1;
    struct vLine* rightVs = rightEnd - 1;

    setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

    for (unsigned char i = 0; i < horizontalsLen; i++){
        horzLineAtXY(horizontals[i].x0Index, horizontals[i].x1Index, posX, horizontals[i].yIndex, posY);
    }

    for (unsigned char i = 0; i < verticalsLen; i++){
        vertLineAtXY(verticals[i].xIndex, posX, verticals[i].y0Index, verticals[i].y1Index, posY);
    }

    //sprintf(vidmem + 1840, "%d", yCoords[35]);

    while (yCoords[topHs->yIndex] < posY + rollTLY) topHs++;
    while (yCoords[botHs->yIndex] > posY + rollBRY) botHs--;
    while (xCoords[leftVs->xIndex] < posX + rollTLX) leftVs++;
    while (xCoords[rightVs->xIndex] > posX + rollBRX) rightVs--;

    setPixel(middleX, middleY, true);

    unsigned char key = 255, prevKey = 255;

    while (true){
        unsigned int startTime = getTime();

        switch (key)
        {
        case keyUp:
            if (getPixel(middleX, middleY - 1)) break;
            setPixel(middleX, middleY, false);
            setPixel(middleX, middleY - 1, true);

            if (posY == 0) break;
            posY--;
            rollAreaDown();

            while(yCoords[(--topHs)->yIndex] >= posY + rollTLY){
                if (topHs == topEnd) break;
                horzLineAtX(topHs->x0Index, topHs->x1Index, posX, rollTLY);
            }
            
            topHs++;

            while(yCoords[botHs->yIndex] > posY + rollBRY) botHs--;

            for (const struct vLine* i = leftVs; i <= rightVs; i++){
                if (yCoords[i->y0Index] <= rollTLY + posY && yCoords[i->y1Index] >= rollTLY + posY)
                    setPixelAtX(i->xIndex, posX, rollTLY);
            }

            break;
        
        case keyDown:
            if (getPixel(middleX, middleY + 1)) break;
            setPixel(middleX, middleY, false);
            setPixel(middleX, middleY + 1, true);

            posY++;
            rollAreaUp();

            while (yCoords[topHs->yIndex] < posY + rollTLY) topHs++;

            while (yCoords[(++botHs)->yIndex] <= posY + rollBRY){
                if (botHs == botEnd) break;
                horzLineAtX(botHs->x0Index, botHs->x1Index, posX, rollBRY);
            }
            
            botHs--;

            for (const struct vLine* i = leftVs; i <= rightVs; i++){
                if (yCoords[i->y0Index] <= rollBRY + posY && yCoords[i->y1Index] >= rollBRY + posY)
                    setPixelAtX(i->xIndex, posX, rollBRY);
            }

            break;

        case keyLeft:
            if (getPixel(middleX - 1, middleY)) break;
            setPixel(middleX, middleY, false);
            setPixel(middleX - 1, middleY, true);

            if (posX == 0) break;
            posX--;
            rollAreaRight();

            for (const struct hLine* i = topHs; i <= botHs; i++){
                if (xCoords[i->x0Index] <= rollTLX + posX && xCoords[i->x1Index] >= rollTLX + posX){
                    setPixelAtY(rollTLX, i->yIndex, posY);
                }
            }

            while (xCoords[(--leftVs)->xIndex] >= posX + rollTLX){
                if (leftVs == leftEnd) break;
                vertLineAtY(rollTLX, leftVs->y0Index, leftVs->y1Index, posY);
            }
            
            leftVs++;

            while (xCoords[rightVs->xIndex] > posX + rollBRX) rightVs--;
            
            break;

        case keyRight:
            if (getPixel(middleX + 1, middleY)) break;
            setPixel(middleX, middleY, false);
            setPixel(middleX + 1, middleY, true);

            posX++;
            rollAreaLeft();

            for (const struct hLine* i = topHs; i <= botHs; i++){
                if (xCoords[i->x0Index] <= rollBRX + posX && xCoords[i->x1Index] >= rollBRX + posX){
                    setPixelAtY(rollBRX, i->yIndex, posY);
                }
            }

            while (xCoords[leftVs->xIndex] < posX + rollTLX) leftVs++;

            while (xCoords[(++rightVs)->xIndex] <= posX + rollBRX){
                if (rightVs == rightEnd) break;
                vertLineAtY(rollBRX, rightVs->y0Index, rightVs->y1Index, posY);
            }
            
            rightVs--;

            break;
        }

        key = getKey();

        //The program will TRY to match the time per frame to the number specified here
        //If drawing takes longer than this time, the program will continue immidiately
        while (key == prevKey && getTime() - startTime < 1){
            key = getKey();
        }

        prevKey = key;
    }
}