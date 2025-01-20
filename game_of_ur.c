#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

struct boardState{
    //Position of the stones, 0 is not yet played, 15 is done
    unsigned char start0 : 4; //Number of stones on starting square
    unsigned char end0 : 4; //Number of sqaures on end square
    unsigned char start1 : 4; //Computer plays player 1
    unsigned char end1 : 4;
    unsigned int stones0; //Int representing the positions of the stones. LSB is starting location
    unsigned int stones1;
};

//Co-ordinates of the entire window
const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;

static unsigned char xPos0[] = {28, 28, 28, 28, 28, 36, 36, 36, 36, 36, 36, 36, 36, 28, 28, 28};
static unsigned char xPos1[] = {44, 44, 44, 44, 44, 36, 36, 36, 36, 36, 36, 36, 36, 44, 44, 44};
static unsigned char yPos[] = {34, 26, 18, 10, 2, 2, 10, 18, 26, 34, 42, 50, 58, 58, 50, 42};

unsigned char rollDice(void){
    unsigned char out = 0;

    for (unsigned char i = 0; i < 4; i++){
        out += rand() & 1;
    }

    unsigned char locX = 60, locY = 15;
    fillRectangle(locX, locY, locX + 16, locY + 4, false);
    for (unsigned char i = 0; i < out; i++){
        rectangle(locX, locY, locX + 4, locY + 4, true);
        locX += 4;
    }

    return out;
}

void showBoard(struct boardState board){
    for (unsigned char i = 1; i < 15; i++){
        fillRectangle(xPos0[i] + 2, yPos[i] + 2, xPos0[i] + 6, yPos[i] + 6, false);
        fillRectangle(xPos1[i] + 2, yPos[i] + 2, xPos1[i] + 6, yPos[i] + 6, false);
    }

    fillRectangle(15, 30, 25, 56, false);
    fillRectangle(55, 30, 65, 56, false);

    unsigned char locX = 15, locY = 30;
    for (unsigned char i = 0; i < board.start0; i++){
        rectangle(locX, locY, locX + 3, locY + 3, true);
        locY += 3;
    }

    locX = 20;
    locY = 30;
    for (unsigned char i = 0; i < board.end0; i++){
        rectangle(locX, locY, locX + 3, locY + 3, true);
        locY += 3;
    }

    locX = 55;
    locY = 30;
    for (unsigned char i = 0; i < board.start1; i++){
        rectangle(locX, locY, locX + 3, locY + 3, true);
        locY += 3;
    }

    locX = 60;
    locY = 30;
    for (unsigned char i = 0; i < board.end1; i++){
        rectangle(locX, locY, locX + 3, locY + 3, true);
        locY += 3;
    }

    for (unsigned char i = 1; i < 15; i++){
        if (!(board.stones0 & (1 << i))) continue;
        rectangle(xPos0[i] + 2, yPos[i] + 2, xPos0[i] + 6, yPos[i] + 6, true);
    }

    for (unsigned char i = 1; i < 15; i++){
        if (!(board.stones1 & (1 << i))) continue;
        fillRectangle(xPos1[i] + 2, yPos[i] + 2, xPos1[i] + 6, yPos[i] + 6, true);
    }
}

void showNextBoard(struct boardState board){
    for (unsigned char i = 1; i < 16; i++){
        setPixel(xPos0[i] + 1, yPos[i] + 1, false);
        setPixel(xPos1[i] + 1, yPos[i] + 1, false);
    }

    for (unsigned char i = 1; i < 16; i++){
        if (!(board.stones0 & (1 << i))) continue;

        setPixel(xPos0[i] + 1, yPos[i] + 1, true);
    }

    for (unsigned char i = 1; i < 16; i++){
        if (!(board.stones1 & (1 << i))) continue;

        setPixel(xPos1[i] + 1, yPos[i] + 1, true);
    }
}

unsigned char doMove(struct boardState *board, unsigned char dice, unsigned int pos, unsigned char player){
    unsigned int nextPos = (1 << dice) * pos;

    if (nextPos == 0) return 1; //Really weird way to figure out whether next position is out of bounds

    switch (player)
    {
    case 0:
        if (!(board->stones0 & pos)) return 1;
        if ((board->stones0 & nextPos) && nextPos != 0x8000) return 1;
        board->stones0 &= ~pos;
        board->stones0 |= nextPos;
        if (nextPos > 0x10 && nextPos < 0x2000){
            if (board->stones1 & nextPos) board->start1++;
            board->stones1 &= ~nextPos;
        }
        if (pos == 1){
            board->start0--;
            if (board->start0) board->stones0 |= 1;
        }
        if (nextPos == 0x8000) board->end0++;
        break;
    
    case 1:
        if (!(board->stones1 & pos)) return 1;
        if ((board->stones1 & nextPos) && nextPos != 0x8000) return 1;
        board->stones1 &= ~pos;
        board->stones1 |= nextPos;
        if (nextPos > 0x10 && nextPos < 0x2000){
            if (board->stones0 & nextPos) board->start0++;
            board->stones0 &= ~nextPos;
        }
        if (pos == 1){
            board->start1--;
            if (board->start1) board->stones1 |= 1;
        }
        if (nextPos == 0x8000) board->end1++;
        break;
    }

    return 0;
}

//Generates all possible moves for a player, returns number of possible moves
unsigned char moveGen(struct boardState board, unsigned char dice, struct boardState* outStates, unsigned char player){
    unsigned char moveNum = 0;

    if (dice == 0){
        *outStates = board;
        return 1;
    }

    struct boardState currentBoard;

    for (unsigned int i = 1; i != 0x8000; i <<= 1){
        currentBoard = board;
        unsigned char inpossible = doMove(&currentBoard, dice, i, player);
        if (inpossible) continue;

        *outStates = currentBoard;
        outStates++;
        moveNum++;
    }

    return moveNum;
}

//Returns value > 0x80 if player 0 is better, < 0x80 if player 1 is better
unsigned char staticAnalysis(struct boardState board){
    unsigned char score = 0x80;
    //score += 5 * board.start1;
    //score -= 5 * board.start0;

    unsigned char counter = 1;
    for (unsigned int i = 2; i != 0x8000; i <<= 1){
        unsigned int stone0 = board.stones0 & i, stone1 = board.stones1 & i;
        if (stone0 && !stone1) score -= counter;
        if (!stone0 && stone1) score += counter;
        counter++;
    }

    return score;
}

//Generates new board after AI plays for player 1
void playAI(struct boardState* board, unsigned char dice){
    struct boardState initBoard;
    initBoard = *board;

    struct boardState posMoves[10];

    unsigned char moveNum = moveGen(initBoard, dice, posMoves, 1);

    struct boardState bestMove;
    unsigned char bestScore = 0xFF;

    for (unsigned char i = 0; i < moveNum; i++){
        unsigned char analysis = staticAnalysis(posMoves[i]);
        if (analysis <= bestScore){
            bestMove = posMoves[i];
            bestScore = analysis;
        }
    }

    *board = bestMove;
}

static void drawInfoScreen(void){
    drawText(10, 0, "The Royal Game of Ur", true, WHITETEXT);

    drawText(4, 9,  "As an ancient 2-player race game, the", false, WHITETEXT);
    drawText(4, 12, "Royal Game of Ur is one of the oldest", false, WHITETEXT);
    drawText(4, 15, "games ever discovered, dating back to", false, WHITETEXT);
    drawText(4, 18, "around 3000 BC. It originated in", false, WHITETEXT);
    drawText(4, 24, "ancient Mesopotamia and remained ", false, WHITETEXT);
    drawText(4, 27, "popular in certain parts of the world", false, WHITETEXT);
    drawText(4, 30, "until the 1950s. Each player is given", false, WHITETEXT);
    drawText(4, 33, "7 stones, with the goal of moving all", false, WHITETEXT);
    drawText(4, 36, "to the end of the board. Your stones ", false, WHITETEXT);
    drawText(4, 39, "move clockwise while the computers", false, WHITETEXT);
    drawText(4, 42, "stones move counter-clockwise. Dice ", false, WHITETEXT);
    drawText(4, 45, "are rolled every move and a single ", false, WHITETEXT);
    drawText(4, 48, "stone may be moved that number of", false, WHITETEXT);
    drawText(4, 51, "squares. Stones from the opponent", false, WHITETEXT);
    drawText(4, 54, "may be captured, but not your own.", false, WHITETEXT);
    drawText(6, 57, "-Use up & down arrows to cycle", false, WHITETEXT);
    drawText(8, 60, "through options", false, WHITETEXT);
    drawText(6, 63, "-Press Enter to play that option", false, WHITETEXT);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void gameOfUr(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics();

    start:

    drawInfoScreen();

    startGraphics();

    struct boardState board = {7, 0, 7, 0, 1, 1};
    struct boardState posMoves[10];

    drawText(60, 12, "Dice:", false, WHITETEXT);

    for (unsigned char i = 1; i < 15; i++){
        rectangle(xPos0[i], yPos[i], xPos0[i] + 8, yPos[i] + 8, true);
        rectangle(xPos1[i], yPos[i], xPos1[i] + 8, yPos[i] + 8, true);
    }

    showBoard(board);

    while (board.end0 != 7 && board.end1 != 7){
        unsigned char dice = rollDice();
        unsigned char moveNum = moveGen(board, dice, posMoves, 0);

        struct boardState* currentMove = posMoves;
        struct boardState* minMove = posMoves;
        struct boardState* maxMove = posMoves + moveNum - 1;

        while (getKey() != keyEnter){
            switch (getKey())
            {
            case keyUp:
                if (currentMove < maxMove) currentMove++;
                showNextBoard(*currentMove);
                break;

            case keyDown:
                if (currentMove > minMove) currentMove--;
                showNextBoard(*currentMove);
                break;
            }

            unsigned char prevKey = getKey();

            while (getKey() == prevKey);
        }

        board = *currentMove;

        showBoard(board);

        playAI(&board, rollDice());

        unsigned int startTime = getTime();
        while (getTime() - startTime < 50);

        showBoard(board);
    }

    if (!board.end0)
        drawText(30, 30, "You lost!", true, REDTEXT);
    else
        drawText(30, 30, "You won!", true, GREENTEXT);

    drawText(30, 67, "Press Space to restart", false, WHITETEXT);
    drawText(30, 70, "Press 0 to return to LCL", false, WHITETEXT);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;

    return;
}