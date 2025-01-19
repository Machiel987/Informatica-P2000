#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#define DEFAULTENTRANCE {0,0,0,0}

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

struct stairs{
    unsigned int TLX, TLY, BRX, BRY; //Area in which stair is active
    unsigned int outXUp, outYUp, outXDown, outYDown; //Output position when taking the stairs up/down
    unsigned char inputDir; //Direction of travel for stair to funciton (implemented using key values from keyBoard.h)
    unsigned char floorUp, floorDown; //Booleans containing whether travel up/down is possible
};

enum classRoom{
    UNDEF, MATH, PHYS, CHEM, GEOG, HIST, COMS, 
    DUTC, ENGL, FREN, GERM, PE, BIO
};

struct classEntranceH{
    unsigned int posX0, posX1;
    unsigned int posY;
    enum classRoom roomType;
    signed char offSetY;
};

//Co-ordinates of the entire window
const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 68;
//Co-ordinates of the rolling window
const static unsigned char rollTLX = 4, rollTLY = 3, rollBRX = 77, rollBRY = 65;

const static unsigned char middleX = 41, middleY = 34; //Middle of the screen, used for drawing walking location

//The following data compression technique is used: Tables with length < 256 are used for the possible x and y coordinates of a
//horizontal/vertical line. A line struct has parameters that index into these tables to determine actual drawing positions.
//This method saves approximately 30% in space.

static unsigned int xCoords[] = {
52, 54, 56, 62, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96,
100, 102, 104, 106, 108, 110, 114, 116, 118, 120, 122, 126, 130, 138,
140, 142, 144, 146, 148, 150, 154, 164, 166, 168, 170, 172, 174, 176,
178, 182, 184, 186, 188, 190, 192, 194, 196, 200, 202, 206, 208, 210,
214, 218, 220, 222, 224, 228, 230, 232, 236, 238, 240, 242, 244, 246,
248, 250, 252, 254, 258, 260, 262, 264, 266, 268, 270, 272, 274, 276,
278, 280, 282, 284, 286, 288, 290, 292, 294, 296, 298, 300, 310, 312,
314, 318, 320, 324, 326, 328, 330, 332, 334, 336, 340, 350, 354, 356,
362, 370, 372, 374, 378, 380, 382, 384, 386, 388, 390, 392, 394, 396,
398, 400, 402, 404, 406, 408, 410, 414, 418, 420, 422, 424, 426, 428,
430, 432, 434, 436, 438, 440, 442, 454, 456, 458, 460, 462, 464, 466,
470, 472, 476, 482, 484, 486, 490, 492, 496, 498, 500, 502, 506, 508,
510, 512, 514, 516, 520, 528, 530, 532, 536, 542, 544, 546, 548, 550,
552, 554, 562, 564, 566, 568, 572, 574, 576, 580, 582, 584, 590, 594,
596, 600, 602, 606, 610, 612, 622, 626, 628, 630, 636, 640, 642, 646,
650, 652, 654, 658, 660, 662, 666, 672, 678, 680, 682, 686, 692, 694,
696, 698, 704, 706, 708, 718, 722, 724, 746, 756, 780, 794, 806, 822,
824};

static unsigned int yCoords[] = {
61, 62, 63, 66, 67, 68, 70, 75, 76, 80, 81, 82, 89, 98, 100, 103,
105, 107, 108, 109, 110, 111, 112, 113, 114, 116, 117, 121, 122, 124,
125, 126, 127, 128, 129, 131, 132, 135, 136, 138, 139, 140, 141, 144,
145, 146, 147, 149, 152, 153, 154, 155, 156, 157, 159, 160, 161, 162,
163, 164, 165, 167, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
179, 180, 181, 183, 188, 189, 190, 191, 192, 193, 195, 196, 197, 198,
199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 211, 212, 213,
214, 215, 216, 217, 218, 219, 221, 222, 223, 224, 225, 226, 227, 228,
229, 230, 231, 232, 233, 234, 235, 236, 238, 239, 240, 241, 243, 244,
247, 248, 249, 250, 251, 252, 253, 255, 256, 259, 260, 261, 262, 263,
265, 266, 267, 268, 269, 270, 271, 273, 275, 278, 279, 280, 282, 284,
285, 286, 288, 290, 291, 292, 294, 296, 297, 298, 299, 300, 301, 302,
303, 307, 308, 309, 313, 314, 315, 318, 319, 320, 321, 322, 325, 326,
327, 328, 329, 330, 331, 333, 335, 337, 345, 346};

//Tables containing horizontal/vertical lines.
//Horizontal lines are sorted by y-coordinate and vertical lines by x-coordinate.
static struct hLine hLines0Inds[] = {
  {210, 212, 1}, {214, 241, 1}, {215, 225, 10}, {215, 225, 13}, {215, 
  225, 25}, {226, 227, 45}, {230, 240, 45}, {95, 123, 46}, {128, 130, 
  46}, {134, 210, 46}, {215, 226, 74}, {0, 95, 78}, {95, 114, 
  78}, {143, 156, 81}, {158, 172, 81}, {174, 191, 81}, {193, 196, 
  81}, {197, 210, 81}, {215, 226, 85}, {132, 135, 94}, {136, 147, 
  94}, {152, 165, 94}, {168, 188, 94}, {190, 194, 94}, {125, 132, 
  99}, {215, 226, 102}, {138, 147, 103}, {127, 132, 104}, {151, 154, 
  104}, {138, 147, 110}, {225, 240, 113}, {0, 2, 114}, {3, 17, 
  114}, {153, 160, 115}, {29, 54, 119}, {58, 70, 119}, {72, 95, 
  119}, {215, 226, 119}, {144, 147, 122}, {122, 135, 123}, {132, 132, 
  123}, {136, 138, 123}, {17, 21, 124}, {23, 29, 124}, {151, 154, 
  125}, {48, 50, 129}, {53, 57, 129}, {59, 63, 129}, {66, 70, 
  129}, {0, 17, 131}, {70, 89, 132}, {89, 90, 132}, {94, 95, 
  132}, {215, 217, 132}, {219, 223, 132}, {224, 226, 132}, {122, 145, 
  133}, {152, 165, 133}, {167, 187, 133}, {190, 195, 133}, {199, 205, 
  133}, {206, 210, 133}, {89, 95, 139}, {215, 226, 139}, {0, 11, 
  142}, {151, 165, 143}, {168, 180, 143}, {182, 201, 143}, {202, 206, 
  143}, {209, 210, 143}, {27, 29, 146}, {29, 44, 146}, {48, 71, 
  146}, {73, 86, 146}, {88, 91, 146}, {93, 95, 146}, {133, 141, 
  149}, {122, 133, 151}, {215, 226, 152}, {203, 206, 153}, {203, 206, 
  154}, {203, 206, 155}, {0, 18, 156}, {20, 24, 156}, {26, 39, 
  156}, {41, 45, 156}, {49, 56, 156}, {204, 206, 157}, {203, 206, 
  158}, {203, 206, 159}, {56, 95, 160}, {203, 206, 161}, {203, 206, 
  162}, {215, 225, 163}, {118, 125, 166}, {138, 138, 166}, {138, 138, 
  166}, {126, 140, 167}, {145, 153, 167}, {61, 71, 169}, {88, 95, 
  169}, {119, 125, 170}, {56, 61, 172}, {68, 71, 173}, {74, 76, 
  173}, {77, 83, 173}, {86, 88, 173}, {22, 28, 174}, {95, 98, 
  174}, {98, 99, 175}, {103, 107, 175}, {111, 112, 175}, {209, 211, 
  175}, {215, 226, 175}, {226, 227, 175}, {230, 235, 175}, {236, 238, 
  175}, {239, 240, 175}, {153, 208, 176}, {118, 119, 177}, {68, 75, 
  178}, {43, 45, 179}, {47, 56, 179}, {231, 232, 180}, {233, 234, 
  180}, {117, 122, 183}, {125, 126, 183}, {129, 140, 183}, {145, 153, 
  183}, {218, 220, 187}, {221, 228, 187}, {229, 231, 187}, {0, 99, 
  192}, {103, 107, 192}, {111, 153, 192}, {210, 241, 193}};

static struct vLine vLines0Inds[] = {
  {0, 78, 137}, {0, 141, 193}, {4, 142, 156}, {5, 142, 156}, {6, 142, 
  156}, {7, 142, 156}, {8, 142, 156}, {9, 142, 156}, {10, 142, 
  156}, {11, 142, 156}, {17, 114, 148}, {17, 154, 156}, {22, 156, 
  174}, {26, 174, 193}, {28, 156, 174}, {29, 118, 120}, {29, 123, 
  146}, {32, 78, 119}, {43, 119, 121}, {43, 126, 146}, {43, 157, 
  193}, {48, 179, 192}, {49, 128, 146}, {56, 129, 146}, {56, 157, 
  192}, {59, 78, 81}, {59, 87, 119}, {61, 168, 170}, {61, 171, 
  172}, {63, 130, 145}, {67, 160, 169}, {68, 78, 109}, {68, 115, 
  119}, {68, 172, 193}, {70, 129, 146}, {75, 172, 178}, {78, 173, 
  192}, {79, 132, 146}, {88, 168, 170}, {88, 171, 192}, {89, 132, 
  146}, {95, 45, 77}, {95, 76, 122}, {95, 129, 150}, {95, 153, 
  161}, {95, 164, 174}, {98, 175, 193}, {112, 175, 192}, {113, 46, 
  78}, {117, 182, 192}, {118, 165, 177}, {119, 170, 177}, {122, 122, 
  133}, {122, 132, 151}, {123, 134, 151}, {124, 133, 151}, {125, 99, 
  123}, {125, 133, 151}, {126, 133, 151}, {126, 164, 183}, {127, 104, 
  123}, {127, 133, 151}, {128, 133, 151}, {129, 133, 151}, {132, 122, 
  123}, {133, 94, 116}, {133, 149, 151}, {135, 183, 185}, {135, 189, 
  192}, {138, 94, 133}, {138, 166, 166}, {138, 166, 166}, {140, 164, 
  170}, {140, 172, 190}, {141, 149, 151}, {143, 45, 81}, {145, 122, 
  151}, {145, 164, 170}, {145, 172, 184}, {145, 188, 192}, {147, 94, 
  98}, {147, 102, 105}, {147, 109, 112}, {147, 116, 123}, {151, 125, 
  127}, {151, 130, 133}, {152, 94, 98}, {152, 102, 104}, {153, 104, 
  125}, {153, 143, 176}, {153, 175, 192}, {159, 46, 81}, {160, 94, 
  133}, {169, 143, 176}, {175, 45, 81}, {184, 95, 133}, {184, 144, 
  175}, {194, 44, 49}, {194, 54, 81}, {194, 94, 133}, {199, 81, 
  86}, {199, 91, 133}, {203, 143, 175}, {206, 153, 175}, {210, 1, 
  136}, {210, 142, 184}, {210, 191, 193}, {215, 1, 8}, {215, 9, 
  14}, {215, 15, 80}, {215, 84, 103}, {215, 106, 133}, {215, 139, 
  149}, {215, 151, 164}, {215, 168, 174}, {218, 186, 192}, {220, 11, 
  13}, {220, 85, 101}, {220, 153, 163}, {222, 119, 132}, {225, 1, 
  6}, {225, 7, 17}, {225, 22, 36}, {225, 42, 51}, {225, 60, 81}, {226,
   84, 106}, {226, 111, 135}, {226, 138, 143}, {226, 147, 170}, {226, 
  171, 175}, {226, 188, 192}, {231, 175, 181}, {231, 186, 193}, {234, 
  175, 192}, {237, 175, 193}, {241, 1, 11}, {241, 12, 61}, {241, 66, 
  125}, {241, 129, 193}};

static struct hLine hLines1Inds[] = {
  {96, 211, 1}, {211, 225, 1}, {195, 226, 17}, {1, 96, 19}, {144, 154, 
  19}, {152, 161, 31}, {204, 211, 31}, {152, 154, 39}, {204, 225, 
  43}, {161, 162, 45}, {164, 179, 45}, {181, 185, 45}, {154, 161, 
  46}, {198, 203, 47}, {17, 30, 48}, {33, 46, 48}, {50, 52, 48}, {55, 
  69, 48}, {73, 91, 48}, {94, 97, 48}, {1, 15, 49}, {189, 198, 
  49}, {161, 173, 54}, {152, 154, 57}, {189, 198, 61}, {123, 144, 
  63}, {176, 178, 63}, {183, 186, 63}, {201, 213, 63}, {223, 225, 
  63}, {1, 18, 64}, {60, 72, 64}, {75, 86, 64}, {89, 92, 64}, {95, 97,
   64}, {97, 114, 64}, {152, 163, 64}, {165, 169, 64}, {172, 175, 
  64}, {25, 31, 65}, {25, 31, 76}, {71, 97, 76}, {205, 211, 76}, {155,
   166, 77}, {169, 181, 77}, {184, 186, 77}, {192, 198, 77}, {200, 
  204, 77}, {61, 71, 78}, {1, 14, 79}, {185, 189, 83}, {191, 192, 
  83}, {204, 207, 84}, {204, 207, 86}, {204, 207, 88}, {150, 155, 
  89}, {210, 225, 89}, {60, 63, 90}, {65, 84, 90}, {87, 92, 90}, {96, 
  148, 90}, {204, 207, 90}, {25, 31, 91}, {204, 207, 92}, {1, 18, 
  93}, {204, 207, 94}, {204, 207, 96}, {204, 207, 97}, {192, 197, 
  103}, {67, 84, 108}, {88, 102, 108}, {104, 115, 108}, {116, 134, 
  108}, {136, 138, 108}, {141, 155, 108}, {1, 15, 109}, {17, 30, 
  109}, {33, 47, 109}, {50, 64, 109}, {155, 226, 117}, {1, 100, 
  139}, {100, 108, 139}, {108, 156, 139}};

static struct vLine vLines1Inds[] = {
  {1, 19, 72}, {1, 72, 140}, {7, 64, 79}, {8, 64, 79}, {9, 64, 
  79}, {10, 64, 79}, {11, 64, 79}, {12, 64, 79}, {13, 64, 79}, {14, 
  49, 52}, {14, 58, 63}, {14, 64, 79}, {14, 92, 100}, {15, 106, 
  109}, {18, 20, 49}, {18, 63, 75}, {18, 81, 92}, {18, 109, 140}, {25,
   65, 67}, {25, 73, 82}, {25, 89, 91}, {31, 65, 90}, {34, 20, 
  49}, {35, 109, 139}, {51, 20, 48}, {51, 109, 139}, {61, 64, 
  90}, {66, 78, 90}, {67, 19, 48}, {68, 108, 139}, {71, 64, 78}, {80, 
  64, 76}, {89, 19, 48}, {89, 108, 139}, {90, 64, 76}, {91, 76, 
  90}, {96, 2, 19}, {97, 16, 48}, {97, 64, 90}, {106, 108, 139}, {114,
   64, 90}, {117, 108, 139}, {123, 63, 90}, {124, 63, 90}, {125, 63, 
  90}, {126, 63, 90}, {127, 63, 90}, {128, 63, 90}, {129, 63, 
  90}, {130, 63, 90}, {131, 63, 90}, {132, 63, 90}, {133, 63, 
  90}, {137, 108, 139}, {144, 1, 19}, {152, 31, 36}, {154, 19, 
  22}, {154, 28, 31}, {154, 39, 57}, {155, 77, 138}, {161, 30, 
  63}, {166, 54, 64}, {169, 45, 54}, {170, 78, 117}, {171, 1, 
  30}, {171, 35, 45}, {173, 45, 64}, {177, 45, 63}, {185, 1, 
  63}, {185, 78, 117}, {189, 47, 53}, {189, 59, 64}, {192, 77, 
  95}, {192, 83, 83}, {192, 101, 104}, {195, 1, 17}, {195, 103, 
  116}, {197, 77, 104}, {198, 47, 64}, {203, 37, 63}, {204, 17, 
  26}, {204, 29, 34}, {204, 76, 117}, {207, 84, 117}, {210, 76, 
  116}, {211, 17, 43}, {213, 43, 50}, {213, 55, 63}, {216, 17, 
  43}, {216, 89, 116}, {220, 17, 43}, {220, 89, 116}, {223, 43, 
  63}, {226, 0, 118}};

static struct hLine hLines2Inds[] = {
  {2, 98, 4}, {2, 157, 4}, {148, 157, 18}, {2, 15, 21}, {140, 143, 
  21}, {145, 148, 21}, {68, 87, 22}, {90, 105, 22}, {108, 117, 
  22}, {120, 137, 22}, {15, 16, 23}, {19, 31, 23}, {34, 48, 23}, {52, 
  65, 23}, {2, 19, 32}, {155, 157, 32}, {26, 33, 33}, {40, 42, 
  33}, {45, 51, 33}, {59, 71, 33}, {74, 86, 33}, {89, 92, 33}, {92, 
  98, 33}, {109, 150, 33}, {33, 41, 35}, {40, 49, 39}, {26, 33, 
  44}, {2, 15, 46}, {59, 70, 46}, {70, 93, 46}, {38, 50, 53}, {109, 
  150, 54}, {156, 157, 54}, {59, 62, 55}, {64, 82, 55}, {85, 88, 
  55}, {91, 93, 55}, {96, 98, 55}, {26, 37, 56}, {49, 51, 56}, {2, 20,
   58}, {140, 143, 68}, {147, 148, 68}, {69, 87, 69}, {90, 104, 
  69}, {108, 117, 69}, {120, 137, 69}, {15, 16, 70}, {19, 31, 
  70}, {35, 48, 70}, {52, 66, 70}, {149, 157, 71}, {2, 15, 72}, {2, 
  157, 107}, {101, 110, 107}, {110, 157, 107}, {2, 101, 108}};

static struct vLine vLines2Inds[] = {
  {2, 5, 41}, {2, 41, 108}, {8, 32, 46}, {9, 32, 46}, {10, 32, 
  46}, {11, 32, 46}, {12, 32, 46}, {13, 32, 46}, {14, 32, 46}, {15, 
  20, 24}, {15, 27, 32}, {15, 32, 46}, {16, 57, 62}, {16, 69, 
  73}, {19, 4, 22}, {19, 32, 35}, {19, 39, 57}, {20, 70, 107}, {26, 
  38, 50}, {33, 33, 56}, {36, 3, 22}, {36, 70, 107}, {38, 53, 
  56}, {40, 33, 40}, {50, 33, 56}, {53, 3, 22}, {53, 70, 107}, {60, 
  33, 46}, {62, 46, 56}, {70, 5, 23}, {70, 32, 46}, {70, 69, 
  107}, {81, 34, 55}, {88, 45, 56}, {91, 33, 45}, {92, 4, 22}, {92, 
  45, 55}, {92, 69, 106}, {98, 3, 4}, {98, 32, 55}, {109, 4, 
  22}, {109, 33, 54}, {109, 70, 108}, {121, 5, 22}, {121, 68, 
  106}, {137, 33, 54}, {138, 33, 54}, {139, 33, 54}, {140, 33, 
  54}, {141, 4, 22}, {141, 33, 54}, {141, 68, 107}, {142, 33, 
  54}, {143, 33, 54}, {144, 33, 54}, {145, 33, 54}, {146, 33, 
  54}, {147, 33, 54}, {148, 17, 22}, {149, 67, 71}, {157, 4, 107}};

static struct classEntranceH classRoomsH0[] = {
    {456, 458, 218, CHEM, -1}, {456, 460, 173, CHEM, +1}, {426, 430, 159, 
   PHYS, -1}, {472, 474, 159, PHYS, -1}, {532, 534, 159, CHEM, -1}, {544, 
   548, 159, CHEM, -1}, {562, 564, 232, BIO, +1}, {502, 504, 232, BIO, +1},
   {456, 460, 232, BIO, +1}, {656, 664, 280, PE, -1}
};

static struct classEntranceH classRoomsH1[] = {
    {458, 464, 155, DUTC, +1}, {504, 508, 155, DUTC, +1}, {380, 384, 189, 
   GERM, +1}, {368, 372, 189, GERM, +1}, {322, 328, 189, ENGL, +1}, {278, 
   282, 189, ENGL, +1}, {232, 238, 189, ENGL, +1}, {188, 194, 190, 
   ENGL, +1}, {54, 58, 119, DUTC, -1}, {98, 102, 118, DUTC, -1}, {142, 148, 
   118, DUTC, -1}, {154, 160, 118, FREN, -1}, {200, 206, 118, FREN, -1}
};

static struct classEntranceH classRoomsH2[] = {
    {390, 392, 77, MATH, -1}, {378, 382, 78, MATH, -1}, {332, 338, 78, 
   MATH, -1}, {286, 290, 78, MATH, -1}, {238, 242, 78, MATH, -1}, {190, 196,
    79, HIST, -1}, {146, 152, 79, MATH, -1}, {100, 104, 79, MATH, -1}, {56, 
   62, 79, COMS, -1}, {146, 152, 143, HIST, +1}, {192, 198, 143, 
   HIST, +1}, {238, 242, 142, MATH, +1}, {282, 290, 142, HIST, +1}, {332, 
   338, 142, DUTC, +1}, {378, 382, 142, GEOG, +1}, {390, 396, 141, 
   GEOG, +1}
};

//Tables containing staircases
static struct stairs stairCases0[] = {
    {341, 219, 342, 243, 399, 151, 0, 0, keyRight, true, false},
    {46, 232, 47, 250, 58, 154, 0, 0, keyLeft, true, false},
    {570, 244, 584, 245, 579, 157, 0, 0, keyDown, true, false}
};

static struct stairs stairCases1[] = {
    {366, 137, 367, 168, 404, 110, 336, 232, keyLeft, true, true},
    {52, 138, 53, 156, 60, 101, 54, 243, keyLeft, true, true},
    {572, 161, 586, 162, 0, 0, 578, 240, keyDown, false, true}
};

static struct stairs stairCases2[] = {
    {398, 95, 399, 124, 0, 0, 399, 151, keyLeft, false, true},
    {54, 94, 55, 112, 0, 0, 58, 154, keyLeft, false, true}
};

struct hLine* horizontals;
struct vLine* verticals;
unsigned char horizontalsLen, verticalsLen;

struct stairs *stairCases;
unsigned char stairsLen;

struct classEntranceH* entrancesH;
unsigned char entrancesHLen;

//Draws a horizontal line using the normal parameters, except using ints and including an "offset" per axis (position of the frame)
void horzLineAtXY(unsigned char x0Index, unsigned char x1Index, unsigned int xPos, unsigned char yIndex, unsigned int yPos){
    unsigned int x0 = xCoords[x0Index];
    unsigned int x1 = xCoords[x1Index];
    unsigned int y  = yCoords[yIndex];

    if (x0 > xPos + rollBRX) return;
    if (x1 < xPos + rollTLX) return;
    if (y < yPos + rollTLY || y > yPos + rollBRY) return;

    if (x0 < xPos + rollTLX) x0 = rollTLX + xPos;
    if (x1 > xPos + rollBRX) x1 = rollBRX + xPos;

    horzLine(x0 - xPos, x1 - xPos, y - yPos, true);

    //horzLineOffset(x0, x1, xPos, y, yPos);
}

//Draws a vertical line using the normal parameters, except using ints and including an "offset" per axis (position of the frame)
void vertLineAtXY(unsigned char xIndex, unsigned int xPos, unsigned char y0Index, unsigned char y1Index, unsigned int yPos){
    unsigned int x  = xCoords[xIndex];
    unsigned int y0 = yCoords[y0Index];
    unsigned int y1 = yCoords[y1Index];

    if (x < xPos + rollTLX || x > xPos + rollBRX) return;
    if (y0 > yPos + rollBRY) return;
    if (y1 < yPos + rollTLY) return;

    if (y0 < yPos + rollTLY) y0 = rollTLY + yPos;
    if (y1 > yPos + rollBRY) y1 = rollBRY + yPos;

    vertLine(x - xPos, y0 - yPos, y1 - yPos, true);

    //vertLineOffset(x, xPos, y0, y1, yPos);
}

//Draws a horizontal line using the normal parameters, except using ints and including an "offset" on the x-axis (position of the frame)
void horzLineAtX(unsigned char x0Index, unsigned char x1Index, unsigned int xPos, unsigned char y){
    unsigned int x0 = xCoords[x0Index];
    unsigned int x1 = xCoords[x1Index];

    if (x0 > xPos + rollBRX) return;
    if (x1 < xPos + rollTLX) return;
    if (y < rollTLY || y > rollBRY) return;

    if (x0 < xPos + rollTLX) x0 = rollTLX + xPos;
    if (x1 > xPos + rollBRX) x1 = rollBRX + xPos;
    
    horzLine(x0 - xPos, x1 - xPos, y, true);
}

//Draws a vertical line using the normal parameters, except using ints and including an "offset" on the y-axis (position of the frame)
void vertLineAtY(unsigned char x, unsigned char y0Index, unsigned char y1Index, unsigned int yPos){
    unsigned int y0 = yCoords[y0Index];
    unsigned int y1 = yCoords[y1Index];

    if (x < rollTLX || x > rollBRX) return;
    if (y0 > yPos + rollBRY) return;
    if (y1 < yPos + rollTLY) return;

    if (y0 < yPos + rollTLY) y0 = rollTLY + yPos;
    if (y1 > yPos + rollBRY) y1 = rollBRY + yPos;

    vertLine(x, y0 - yPos, y1 - yPos, true);
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

//Changes floor level, returns 0 on no staircase (normal case)
unsigned char handleFloorChange(unsigned char* floor, unsigned int* xPos, unsigned int* yPos){
    unsigned char startKey = getKey();
    struct stairs currentStairs;

    for (unsigned char i = 0; i < stairsLen; i++){
        currentStairs = stairCases[i];
        if (currentStairs.TLX <= *xPos && currentStairs.TLY <= *yPos && currentStairs.BRX >= *xPos && currentStairs.BRY >= *yPos)
            break;
        if (i == stairsLen - 1) return 0;
    }

    if (startKey != currentStairs.inputDir) return 0;

    drawText(23, 30, "You reached a staircase", false, WHITETEXT);
    drawText(18, 36, "Press arrows to move up/down", false, WHITETEXT);

    while(getKey() == startKey || (getKey() != keyUp && getKey() != keyDown));

    if (getKey() == keyUp && currentStairs.floorUp){
        *floor += 1;
        *xPos = currentStairs.outXUp;
        *yPos = currentStairs.outYUp;
    }
    else return 1;

    if (getKey() == keyDown && currentStairs.floorDown){
        *floor -= 1;
        *xPos = currentStairs.outXDown;
        *yPos = currentStairs.outYDown;
    }
    else return 1;

    return 1;
}

//Main function for walking through a map
void walker(void){
    unsigned char floor = 0;
    unsigned int posX = 284, posY = 320;

    floorStart: //When there is a floor change, code begins here

    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics();

    //Update lines depending on the floor level
    switch (floor)
    {
    case 0:
        horizontals = hLines0Inds;
        verticals = vLines0Inds;
        horizontalsLen = sizeof(hLines0Inds) / sizeof(struct hLine);
        verticalsLen = sizeof(vLines0Inds) / sizeof(struct vLine);

        stairCases = stairCases0;
        stairsLen = sizeof(stairCases0) / sizeof(struct stairs);

        entrancesH = classRoomsH0;
        entrancesHLen = sizeof (classRoomsH0) / sizeof (struct classEntranceH);
        break;
    
    case 1:
        horizontals = hLines1Inds;
        verticals = vLines1Inds;

        horizontalsLen = sizeof(hLines1Inds) / sizeof(struct hLine);
        verticalsLen = sizeof(vLines1Inds) / sizeof(struct vLine);

        stairCases = stairCases1;
        stairsLen = sizeof(stairCases1) / sizeof(struct stairs);

        entrancesH = classRoomsH1;
        entrancesHLen = sizeof (classRoomsH1) / sizeof (struct classEntranceH);
        break;
    
    case 2:
        horizontals = hLines2Inds;
        verticals = vLines2Inds;

        horizontalsLen = sizeof(hLines2Inds) / sizeof(struct hLine);
        verticalsLen = sizeof(vLines2Inds) / sizeof(struct vLine);

        stairCases = stairCases2;
        stairsLen = sizeof(stairCases2) / sizeof(struct stairs);

        entrancesH = classRoomsH2;
        entrancesHLen = sizeof (classRoomsH2) / sizeof (struct classEntranceH);
        break;
    }

    rectangle(2, 1, 79, 67, true);
    setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

    //Pointers indicating the start and end of the arrays
    struct hLine* topEnd = horizontals - 1;
    struct hLine* botEnd = horizontals + horizontalsLen;
    struct vLine* leftEnd = verticals - 1;
    struct vLine* rightEnd = verticals + verticalsLen;
    
    //Pointers indicating the current position of the screen relative to the lists
    //These pointers are only updated when necessary and only lines between these pointers need to be checked for drawing
    //This method is 10-15x faster than checking all lines
    struct hLine* topHs = topEnd + 1;
    struct hLine* botHs = botEnd - 1;
    struct vLine* leftVs = leftEnd + 1;
    struct vLine* rightVs = rightEnd - 1;

    for (unsigned char i = 0; i < horizontalsLen; i++){
        horzLineAtXY(horizontals[i].x0Index, horizontals[i].x1Index, posX, horizontals[i].yIndex, posY);
    }

    for (unsigned char i = 0; i < verticalsLen; i++){
        vertLineAtXY(verticals[i].xIndex, posX, verticals[i].y0Index, verticals[i].y1Index, posY);
    }

    //Update the pointers
    while (yCoords[topHs->yIndex] < posY + rollTLY) topHs++;
    while (yCoords[botHs->yIndex] > posY + rollBRY) botHs--;
    while (xCoords[leftVs->xIndex] < posX + rollTLX) leftVs++;
    while (xCoords[rightVs->xIndex] > posX + rollBRX) rightVs--;

    setPixel(middleX, middleY, true);

    unsigned char key = 255, prevKey = 255;

    //TODO: cleanup switch
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

        sprintf(vidmem + 1840, "                  ");
        sprintf(vidmem + 1840, "(%i, %i)", posX, posY);

        if (handleFloorChange(&floor, &posX, &posY)) goto floorStart;

        struct classEntranceH entrance = DEFAULTENTRANCE;

        for (unsigned char i = 0; i < entrancesHLen; i++){
            if ((posY == entrancesH[i].posY) && (posX >= entrancesH[i].posX0) && (posX <= entrancesH[i].posX1)){
                memcpy(&entrance, entrancesH + i, sizeof (struct classEntranceH));
            }
        }

        switch (entrance.roomType)
        {
        case MATH:
            tetris();
            posY -= entrance.offSetY;
            goto floorStart;
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
