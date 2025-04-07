#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"
/*
char sentenceDutch[] = 
    "het is ook een mythe dat we door \n"
    "de digitalisering daadwerkelijk met \n"
    "anderen verbonden zijn. hoewel de \n"
    "communicatiefrequentie via facebook, \n"
    "e-mail, twitter, en sms vertienvoudigd \n"
    "is, blijkt uit onderzoek van onder \n"
    "anderen de britse neuroloog susan \n"
    "greenfield dat we minder in staat \n"
    "zijn subtiele, typische menselijke \n"
    "vormen van empathie en compassie voor \n"
    "anderen te voelen. echter betekent \n"
    "dit niet direct dat onze sociale \n"
    "vaardigheden zijn afgenomen t.o.v. \n"
    "een aantal jaren terug.";
*/

char sentenceEnglish[] =
    "it does not help that these spats \n"
    "have occurred while mr trump \n"
    "threatens to place sanctions on \n" 
    "the european union over its digital \n"
    "policies. these could trigger the \n"
    "repatriation of irish jobs and, \n"
    "especially, of taxes paid by us \n" 
    "multinationals whose european arms \n"
    "are headquartered in ireland, lured \n"
    "there by dublins low corporate tax \n"
    "regime.";


void multiLineText(unsigned char x, unsigned char y, unsigned char* text){
    unsigned char* disp_p = vidmem + x + 80*y;
    unsigned char lineNum = 1;

    while (*text){
        if (*text == '\n'){
            disp_p = vidmem + x + 80 * (y+lineNum);
            lineNum++;
        }
        else {
            *disp_p = *text;
            disp_p++;
        }

        text++;
    }
}

static void drawInfoScreen(void){
    drawText(10, 0, "Typing test", true, WHITETEXT);

    drawText(4, 9,  "As an essential part of controlling ", false, WHITETEXT);
    drawText(4, 12, "a computer, typing is undeniably", false, WHITETEXT);
    drawText(4, 15, "an important 21st-century skill.", false, WHITETEXT);
    drawText(4, 18, "Therefore, you will now try to type", false, WHITETEXT);
    drawText(4, 24, "a small snippet of a text from a main", false, WHITETEXT);
    drawText(4, 27, "newspaper as quickly as possible.", false, WHITETEXT);
    drawText(4, 30, "The shift-key on this particular", false, WHITETEXT);
    drawText(4, 33, "Philips P2000T computer is somewhat", false, WHITETEXT);
    drawText(4, 36, "hard to press, so the whole text ", false, WHITETEXT);
    drawText(4, 39, "is to be typed using lowercase only.", false, WHITETEXT);
    drawText(4, 42, "Press the 0 key at any point to stop", false, WHITETEXT);
    drawText(4, 45, "the mini-game.", false, WHITETEXT);
    drawText(4, 51, "Enjoy!", false, WHITETEXT);

    drawText(20, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void typing(void){
    unsigned char* sentence;
    sentence = sentenceEnglish;

    setWindow(2, 0, 79, 71);

    startGraphics(WHITEGFS);
    drawInfoScreen();

    start:

    startGraphics(WHITEGFS);

    drawText(10, 0, "Typing test", true, WHITETEXT);

    unsigned int startTime = getTime();

    unsigned char x = 0, y = 3;
    multiLineText(x, y, sentence);

    const unsigned char* sentence_p = sentence;
    unsigned char* disp_p = vidmem + x + 80*y;
    unsigned char lineNum = 1;

    while (*sentence_p != '\0'){
        if (getKey() == key0) return;

        if (*sentence_p == '\n'){
            disp_p = vidmem + x + 80 * (y+lineNum);
            lineNum++;
            sentence_p++;
        }

        if (getNiceKeyL() == *sentence_p){
            *disp_p += 0x80;
            disp_p++;
            sentence_p++;
        }
    }

    drawText(30, 30, "Done!", true, GREENTEXT);
    sprintf(vidmem + 1600, "Your score: %d", getTime() - startTime);

    drawText(30, 67, "Press Space to restart", false, WHITETEXT);
    drawText(30, 70, "Press 0 to return to LCL", false, WHITETEXT);

    unsigned char prevKey = getKey();

    while (getKey() == prevKey);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;

    return;
}