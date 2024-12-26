#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

//Co-ordinates of the entire window
const unsigned char walkerTLX = 2, walkerTLY = 0, walkerBRX = 79, walkerBRY = 68;
//Co-ordinates of the rolling window
const unsigned char rollTLX = 4, rollTLY = 3, rollBRX = 77, rollBRY = 65;

//Table with all co-ordinates of the horizontal lines needed
const unsigned int horizontals[][4] = {
    {80, 115, 95, 1},
    {90, 110, 105, 1}
};
//Table with all co-ordinates of the vertical lines needed
const unsigned int verticals[][4] = {
    {95, 85, 100, 1},
    {120, 80, 120, 1}
};

//Length of the specified tables
const unsigned char horizontalsLen = sizeof(horizontals) / sizeof(int) / 4, verticalsLen = sizeof(verticals) / sizeof(int) / 4;

//Draws a horizontal line using the normal parameters, except using ints and including an "offset" per axis (position of the frame)
void horzLineAtXY(unsigned int x0, unsigned int x1, unsigned int xPos, unsigned int y, unsigned int yPos, unsigned char wt){
    if (x0 > xPos + rollBRX) return;
    if (x1 < xPos + rollTLX) return;
    if (y < yPos + rollTLY || y > yPos + rollBRY) return;

    unsigned char startX = x0 - xPos;
    unsigned char endX = x1 - xPos;
    
    if (x0 < xPos + rollTLX) startX = rollTLX;
    if (x1 > xPos + rollBRX) endX = rollBRX;

    unsafeHorzLine(startX, endX, y - yPos, wt);
}

//Draws a vertical line using the normal parameters, except using ints and including an "offset" per axis (position of the frame)
void vertLineAtXY(unsigned int x, unsigned int xPos, unsigned int y0, unsigned int y1, unsigned int yPos, unsigned char wt){
    if (x < xPos + rollTLX || x > xPos + rollBRX) return;
    if (y0 > yPos + rollBRY) return;
    if (y1 < yPos + rollTLY) return;

    unsigned char startY = y0 - yPos;
    unsigned char endY = y1 - yPos;

    if (y0 < yPos + rollTLY) startY = rollTLY;
    if (y1 > yPos + rollBRY) endY = rollBRY;

    unsafeVertLine(x - xPos, startY, endY, wt);
}

//Draws a horizontal line using the normal parameters, except using ints and including an "offset" on the x-axis (position of the frame)
void horzLineAtX(unsigned int x0, unsigned int x1, unsigned int xPos, unsigned char y, unsigned char wt){
    if (x0 > xPos + rollBRX) return;
    if (x1 < xPos + rollTLX) return;
    if (y < rollTLY || y > rollBRY) return;
    
    unsigned char startX = x0 - xPos;
    unsigned char endX = x1 - xPos;
    
    if (x0 < xPos + rollTLX) startX = rollTLX;
    if (x1 > xPos + rollBRX) endX = rollBRX;

    unsafeHorzLine(startX, endX, y, wt);
}

//Draws a vertical line using the normal parameters, except using ints and including an "offset" on the y-axis (position of the frame)
void vertLineAtY(unsigned char x, unsigned int y0, unsigned int y1, unsigned int yPos, unsigned char wt){
    if (x < rollTLX || x > rollBRX) return;
    if (y0 > yPos + rollBRY) return;
    if (y1 < yPos + rollTLY) return;

    unsigned char startY = y0 - yPos;
    unsigned char endY = y1 - yPos;

    if (y0 < yPos + rollTLY) startY = rollTLY;
    if (y1 > yPos + rollBRY) endY = rollBRY;

    unsafeVertLine(x, startY, endY, wt);
}

//Sets a given pixel using the normal parameters, and including an offset on the x-axis
void setPixelAtX(unsigned int x, unsigned int xPos, unsigned char y, unsigned char wt){
    if (x < xPos || x > xPos + 255) return;
    setPixel(x - xPos, y, wt);
}

//Sets a given pixel using the normal parameters, and including an offset on the y-axis
void setPixelAtY(unsigned char x, unsigned int y, unsigned int yPos, unsigned char wt){
    if (y < yPos || y > yPos + 255) return;
    setPixel(x, y - yPos, wt);
}

//Main function for walking through a map
void walker(void){
    setWindow(walkerTLX, walkerTLY, walkerBRX, walkerBRY);
    startGraphics();

    rectangle(2, 1, 79, 67, true);

    unsigned int posX = 70, posY = 70;

    for (unsigned char i = 0; i < horizontalsLen; i++){
        horzLineAtXY(horizontals[i][0], horizontals[i][1], posX, horizontals[i][2], posY, horizontals[i][3]);
    }

    for (unsigned char i = 0; i < verticalsLen; i++){
        vertLineAtXY(verticals[i][0], posX, verticals[i][1], verticals[i][2], posY, verticals[i][3]);
    }

    setWindow(rollTLX, rollTLY, rollBRX, rollBRY);

    unsigned char key = 255, prevKey = 255;
    while (true){
        unsigned int startTime = getTime();

        switch (key)
        {
        case keyUp:
            posY--;
            rollAreaDown(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][2] == rollTLY + posY)
                    horzLineAtX(horizontals[i][0], horizontals[i][1], posX, rollTLY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][1] <= rollTLY + posY && verticals[i][2] >= rollTLY + posY)
                    setPixelAtX(verticals[i][0], posX, rollTLY, verticals[i][3]);
            }
            break;
        
        case keyDown:
            posY++;
            rollAreaUp(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][2] == rollBRY + posY)
                    horzLineAtX(horizontals[i][0], horizontals[i][1], posX, rollBRY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][1] <= rollBRY + posY && verticals[i][2] >= rollBRY + posY)
                    setPixelAtX(verticals[i][0], posX, rollBRY, verticals[i][3]);
            }
            break;

        case keyLeft:
            posX--;
            rollAreaRight(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][0] <= rollTLX + posX && horizontals[i][1] >= rollTLX + posX)
                    setPixelAtY(rollTLX, horizontals[i][2], posY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][0] == rollTLX + posX)
                    vertLineAtY(rollTLX, verticals[i][1], verticals[i][2], posY, verticals[i][3]);
            }
            break;

        case keyRight:
            posX++;
            rollAreaLeft(rollTLX, rollTLY, rollBRX, rollBRY);

            for (unsigned char i = 0; i < horizontalsLen; i++){
                if (horizontals[i][0] <= rollBRX + posX && horizontals[i][1] >= rollBRX + posX)
                    setPixelAtY(rollBRX, horizontals[i][2], posY, horizontals[i][3]);
            }

            for (unsigned char i = 0; i < verticalsLen; i++){
                if (verticals[i][0] == rollBRX + posX)
                    vertLineAtY(rollBRX, verticals[i][1], verticals[i][2], posY, verticals[i][3]);
            }
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