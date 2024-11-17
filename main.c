#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Implement booleans
#define false 0
#define true 1

//Colors for printing text to the screen
#define REDTEXT 1
#define GREENTEXT 2
#define YELLOWTEXT 3
#define BLUETEXT 4
#define MAGENTATEXT 5
#define CYANTEXT 6
#define WHITETEXT 7

//Collors for using pixels on the screen
#define REDGFS 17
#define GREENGFS 18
#define YELLOWGFS 19
#define BLUEGFS 20
#define MAGENTAGFS 21
#define CYANGFS 22
#define WHITEGFS 23

//Functions for finding min and max (defined in macro for better performance)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

//Create reference to video memory
__at (0x5000) char VIDMEM[];
char* vidmem = VIDMEM;

//LUT where first 12 bits show address of first character in line of pixels and
//last 4 bits show 0, 2 or 4 depending on 'parity' of pixel line
unsigned short yAdrLUT[75];
unsigned char flipTable[128]; //Table with flipped version of characters
unsigned char pxNumToChar[6]; //Convert pixel num [0..5] to actual character

//Data type for storing small portion of screen
struct sprite{
    unsigned char width;
    unsigned char height;
    unsigned char* chars;
};

//Function for calculating 2^n
unsigned char pwr2 (unsigned char e){
    unsigned char res = 1;
    for (int i = 0; i < e; i++) res <<= 1;
    return res;
}

//Fills all LUTs and initializes screen, must be called before attempting graphics routines
void startGraphics(void){
    for (unsigned char i = 0; i < 6; i++) pxNumToChar[i] = pwr2(i < 5 ? i : i + 1);

    for (unsigned char i = 0; i < 75; i++) yAdrLUT[i] = 0x5000 + 80 * (i/3) + 2 * (i%3);
    for (unsigned int i = 0; i < 1919; i++) vidmem[i] = 32;
    for (unsigned int i = 0; i <= 1760; i += 80) vidmem[i] = WHITEGFS;

    for (unsigned char i = 0; i < 128; i++){
        unsigned char lft = ((i >> 1) & 0x1F) & ((i >> 2) | 0xF);
        unsigned char rgt = ((i << 1) & 0x5F) & ((i << 2) | 0x3F);
        flipTable[i] = (lft & 0x15) | (rgt & 0x4A) | 0x20;
    }

    return;
}

inline static unsigned char inRange(unsigned char x, unsigned char y){
    return (x < 160 && y < 75);
}

//Set pixel without checking whether the pixel fits within the screen
//Use with extreme caution
inline static void unsafeSetPixel(unsigned char x, unsigned char y, unsigned char wt){
    unsigned char* charAdr  = (unsigned char*) (yAdrLUT[y] & 0xFFF0) + (x >> 1);
    unsigned char  pixelNum = (x & 1) + (yAdrLUT[y] & 0xF);

    unsigned char newChar;

    if (wt) newChar = *charAdr | pxNumToChar[pixelNum];
    else newChar = *charAdr & ~pxNumToChar[pixelNum];

    *charAdr = newChar;

    return;
}

//Set pixel to ON, without screen check
inline static void unsafeSetPixelOn(unsigned char x, unsigned char y){
    unsigned char* charAdr  = (unsigned char*) (yAdrLUT[y] & 0xFFF0) + (x >> 1);
    unsigned char  pixelNum = (x & 1) + (yAdrLUT[y] & 0xF);

    unsigned char newChar = *charAdr | pxNumToChar[pixelNum];

    *charAdr = newChar;

    return;
}

//Set pixel to OFF, without screen check
inline static void unsafeSetPixelOff(unsigned char x, unsigned char y){
    unsigned char* charAdr  = (unsigned char*) (yAdrLUT[y] & 0xFFF0) + (x >> 1);
    unsigned char  pixelNum = (x & 1) + (yAdrLUT[y] & 0xF);

    unsigned char newChar = *charAdr & ~pxNumToChar[pixelNum];

    *charAdr = newChar;
}

//Sets a given pixel. Coordinates are in pixel, not character coordinates
//wt = white (1 if pixel will be set to white, 0 if set to black)
inline static void setPixel(unsigned char x, unsigned char y, unsigned char wt){
    if (x < 2 || x >= 160 || y >= 75) return;

    unsafeSetPixel(x, y, wt);

    return;
}

//Draws or erases a line between any 2 screen coordinates.
//Uses a generalized version of Bresenham's line algorithm
void drawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt){
    signed char dx = abs(x1 - x0);
    signed char sx = x0 < x1 ? 1 : -1;

    signed char dy = -abs(y1 - y0);
    signed char sy = y0 < y1 ? 1 : -1;

    int error = dx + dy;
    int e2 ;

    while(1){
        setPixel(x0, y0, wt);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * error;

        if (e2 - dy >= 0){
            error += dy;
            x0 += sx;
        }

        if (e2 - dx < 0){
            error += dx;
            y0 += sy;
        }
    }

    return;
}

//Draws a line between 2 points, in the given color (use _COLOR_GFS)
//TODO: Colors
void drawLineColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color){
    signed char dx = abs(x1 - x0);
    signed char sx = x0 < x1 ? 1 : -1;

    signed char dy = -abs(y1 - y0);
    signed char sy = y0 < y1 ? 1 : -1;

    int error = dx + dy;
    int e2 ;

    unsigned char xUp;

    * (unsigned char*) (yAdrLUT[y0] & 0xFFF0 + (x0 >> 1) - 1) == color;
    * (unsigned char*) (yAdrLUT[y0] & 0xFFF0 + (x0 >> 1) + 1) == WHITEGFS;
    * (unsigned char*) (yAdrLUT[y1] & 0xFFF0 + (x1 >> 1) - 1) == color;
    * (unsigned char*) (yAdrLUT[y1] & 0xFFF0 + (x1 >> 1) + 1) == WHITEGFS;

    while(1){
        setPixel(x0, y0, true);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * error;

        if (e2 - dy >= 0){
            error += dy;
            x0 += sx;
            xUp = true;
        }

        if (e2 - dx < 0){
            error += dx;
            y0 += sy;
            xUp = false;
        }
    }

    return;
}

//Draws or erases a horizontal line
void horzLine(unsigned char x0, unsigned char x1, unsigned char y, unsigned char wt){
    unsigned char xmin = MIN(x0, x1);
    unsigned char xmax = MAX(x0, x1);
    if (xmax - 160 >= 0) xmax = 159;

    unsigned char* bgnAdr = (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + ((xmin + 1) >> 1));
    unsigned char* endAdr = (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + ((xmax + 1) >> 1));

    unsigned char* adr = bgnAdr;

    if (wt){
        switch (yAdrLUT[y] & 0xF)
        {
        case 0:
            while (endAdr - adr > 0){
                *adr |= 0b00100011;
                adr++;
            }
            break;
        
        case 2:
            while (endAdr - adr > 0){
                *adr |= 0b00101100;
                adr++;
            }
            break;

        case 4:
            while (endAdr - adr > 0){
                *adr |= 0b01110000;
                adr++;
            }
            break;
        }
    }
    else{
        switch (yAdrLUT[y] & 0xF)
        {
        case 0:
            while (endAdr - adr > 0){
                *adr &= ~0b00100011;
                adr++;
            }
            break;
        
        case 2:
            while (endAdr - adr > 0){
                *adr &= ~0b00101100;
                adr++;
            }
            break;

        case 4:
            while (endAdr - adr > 0){
                *adr &= ~0b01110000;
                adr++;
            }
            break;
        }
    }

    setPixel(x0, y, wt);
    setPixel(x1, y, wt);
}

//Draws a horizontal line in the given color (use _COLOR_GFS)
void horzLineColor(unsigned char x0, unsigned char x1, unsigned char y, unsigned char color){
    unsigned char xmin = MIN(x0, x1);
    unsigned char xmax = MAX(x0, x1);

    * (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + (xmin >> 1) - 1) = color;
    * (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + (xmax >> 1) + 1) = WHITEGFS;

    for (unsigned char i = xmin; i <= xmax; i++){
        setPixel(i, y, true);
    }
}

//Draws or erases a vertical line
void vertLine(unsigned char x, unsigned char y0, unsigned char y1, unsigned char wt){
    unsigned char ymin = MIN(y0, y1);
    unsigned char ymax = MAX(y0, y1);
    if (ymax - 72 >= 0) ymax = 71;

    unsigned char* bgnAdr = (unsigned char*) ((yAdrLUT[ymin + 2] & 0xFFF0) + (x >> 1));
    unsigned char* endAdr = (unsigned char*) ((yAdrLUT[ymax + 1] & 0xFFF0) + (x >> 1));

    unsigned char* adr = bgnAdr;

    if(wt){
        switch (x & 1)
        {
        case 0:
            while (endAdr - adr > 0){
                *adr |= 0b00110101;
                adr += 80;
            }
            break;
        
        case 1:
            while (endAdr - adr > 0){
                *adr |= 0b01101010;
                adr += 80;
            }
            break;
        }
    }
    else{
        switch (x & 1)
        {
        case 0:
            while (endAdr - adr > 0){
                *adr &= ~0b00110101;
                adr += 80;
            }
            break;
        
        case 1:
            while (endAdr - adr > 0){
                *adr &= ~0b01101010;
                adr += 80;
            }
            break;
        }
    }

    setPixel(x, ymin, wt);
    setPixel(x, ymin + 1, wt);
    setPixel(x, ymax, wt);
    setPixel(x, ymax - 1, wt);
    
    return;
}

//Draws a vertical line in the given color (use _COLOR_GFS)
void vertLineColor(unsigned char x, unsigned char y0, unsigned char y1, unsigned char color){
    unsigned char ymin = MIN(y0, y1);
    unsigned char ymax = MAX(y0, y1);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (x >> 1) - 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (x >> 1) - 1; i += 80){
        * (unsigned char*) i = color;
    }

    vertLine(x, y0, y1, true);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (x >> 1) + 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (x >> 1) + 1; i += 80)
        * (unsigned char*) i = WHITEGFS;
}

//Draws or erases a hollow rectangle
void rectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt){
    horzLine(x0, x1, y0, wt);
    horzLine(x0, x1, y1, wt);
    vertLine(x0, y0, y1, wt);
    vertLine(x1, y0, y1, wt);
}

//Draws a hollow rectange in the given color (use _COLOR_GFS)
void rectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color){
    unsigned char xmin = MIN(x0, x1);
    unsigned char xmax = MAX(x0, x1);
    unsigned char ymin = MIN(y0, y1);
    unsigned char ymax = MAX(y0, y1);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (xmin >> 1) - 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (xmin >> 1) - 1; i += 80){
        * (unsigned char*) i = color;
    }

    rectangle(x0, y0, x1, y1, true);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (xmax >> 1) + 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (xmax >> 1) + 1; i += 80)
        * (unsigned char*) i = WHITEGFS;
}

//Draws or erases a filled rectangle
void fillRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt){
    unsigned char xmin = MIN(x0, x1);
    unsigned char xmax = MAX(x0, x1);
    unsigned char ymin = MIN(y0, y1);
    unsigned char ymax = MAX(y0, y1);

    unsigned char xminOl = xmin % 2;
    unsigned char xmaxOl = xmax % 2;

    if (xminOl) vertLine(xmin, ymin, ymax, wt);
    if (!xmaxOl) vertLine(xmax, ymin, ymax, wt);

    unsigned char yminOl = yAdrLUT[ymin] & 0xF;
    unsigned char ymaxOl = yAdrLUT[ymax] & 0xF;

    switch (yminOl)
    {
    case 2:
        horzLine(xmin, xmax, ymin, wt);
        horzLine(xmin, xmax, ymin + 1, wt);
        break;

    case 4:
        horzLine(xmin, xmax, ymin, wt);
        break;
    
    default:
        break;
    }

    switch (ymaxOl)
    {
    case 0:
        horzLine(xmin, xmax, ymax, wt);
        break;

    case 2:
        horzLine(xmin, xmax, ymax, wt);
        horzLine(xmin, xmax, ymax - 1, wt);
        break;
    
    default:
        break;
    }

    for (unsigned int yAdr = yAdrLUT[ymin + 2] & 0xFFF0; yAdr <= (yAdrLUT[ymax - 2] & 0xFFF0); yAdr += 80){
        for (unsigned int xAdr = (1 + xmin) >> 1; xAdr < (xmax + 1) >> 1; xAdr++){
            unsigned char* p_char = (unsigned char*) (yAdr + xAdr);
            if (wt) *p_char = 127;
            else *p_char = 32;
        }
    }
}

//Draws a filled rectangle in the given color (use _COLOR_GFS)
void fillRectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color){
    unsigned char xmin = MIN(x0, x1);
    unsigned char xmax = MAX(x0, x1);
    unsigned char ymin = MIN(y0, y1);
    unsigned char ymax = MAX(y0, y1);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (xmin >> 1) - 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (xmin >> 1) - 1; i += 80)
        * (unsigned char*) i = color;

    fillRectangle(x0, y0, x1, y1, true);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (xmax >> 1) + 1; i <= (yAdrLUT[ymax] & 0xFF0C) + (xmax >> 1) + 1; i += 80)
        * (unsigned char*) i = WHITEGFS;
}

//Prints text in a given color to the screen. This text can be normal or double height
//Due to the text being drawn on pixel y coordinates divisible by 3, the text might not match the desired location perfectly
void drawText(unsigned char x, unsigned char y, char* text, unsigned char textLen, unsigned char dblH, unsigned char color){
    unsigned char* startAdr = (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + (x >> 1));

    if (dblH) {*(startAdr - 2) = color; *(startAdr - 1) = 0xD;}
    else *(startAdr - 1) = color;

    for (unsigned char i = 0; i < textLen; i++){
        *(startAdr + i) = text[i];
    }
    
    if (dblH) {{*(startAdr + textLen) = 0xC; *(startAdr + textLen + 1) = WHITEGFS;}}
    else {*(startAdr + textLen) = WHITEGFS;}
}

//Draws or erases a safe/unsafe circle depending on function pointer
void circleSU(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt, void (*f)(unsigned char, unsigned char, unsigned char)){
    unsigned char t1 = r / 16;
    unsigned char x = r;
    unsigned char y = 0;

    while (x - y >= 0){
        /*
        setPixel(xm + x, ym + y, wt);
        setPixel(xm - x, ym + y, wt);
        setPixel(xm + x, ym - y, wt);
        setPixel(xm - x, ym - y, wt);
        setPixel(xm + y, ym + x, wt);
        setPixel(xm - y, ym + x, wt);
        setPixel(xm + y, ym - x, wt);
        setPixel(xm - y, ym - x, wt);
        */
        f(xm + x, ym + y, wt);
        f(xm - x, ym + y, wt);
        f(xm + x, ym - y, wt);
        f(xm - x, ym - y, wt);
        f(xm + y, ym + x, wt);
        f(xm - y, ym + x, wt);
        f(xm + y, ym - x, wt);
        f(xm - y, ym - x, wt);

        y += 1;
        t1 += y;
        char t2 = t1 - x;
        if (t2 >= 0){
            t1 = t2;
            x -= 1;
        }
    }

    return;
}

//Draws or erases a hollow circle using the midpoint circle algorithm
void circle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt){
    if (inRange(xm - r, ym - r) && inRange(xm + r, ym + r)) circleSU(xm, ym, r, wt, unsafeSetPixel);
    else circleSU(xm, ym, r, wt, setPixel);
}

//Draws a circle in the given color usign the midpoint circle algorithm
void fillCircle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt){
    unsigned char t1 = r / 16;
    unsigned char x = r;
    unsigned char y = 0;

    while (x - y >= 0){
        horzLine(xm - x, xm + x, ym + y, wt);
        horzLine(xm - x, xm + x, ym - y, wt);
        horzLine(xm - y, xm + y, ym + x, wt);
        horzLine(xm - y, xm + y, ym - x, wt);
        y += 1;
        t1 += y;
        char t2 = t1 - x;
        if (t2 >= 0){
            t1 = t2;
            x -= 1;
        }
    }

    return;
}

//Reserve memory space for a sprite of determined dimensions
//Dimensions are in CHARACTER coordinates
struct sprite* mallocSprite(unsigned char width, unsigned char height){
    struct sprite *out = malloc(sizeof(struct sprite)) ;
    out->width = width;
    out->height = height;
    out->chars = (unsigned char*)malloc(width * height);

    return out;
}

//Frees the memory reserved for given sprite
void freeSprite(struct sprite *s){
    free(s->chars) ;
    free(s) ;
}

//Saves a sprite from the characters on screen in given location
//Dimensions are in CHARACTER coordinates
void getSprite(struct sprite* buf, unsigned char x0, unsigned char y0){
    unsigned int count = 0;

    for (unsigned char j = y0; j < (y0 + buf->height); j++){
        for (unsigned char i = x0; i < (x0 + buf->width); i++){
            unsigned char charXY = *(unsigned char*) (vidmem + 80*j + i);
            buf->chars[count] = charXY;
            count++;
        }
    }

    return;
}

//Draws a previously saves sprite on the screen
//Dimensions are in CHARACTER coordinates
void drawSprite(struct sprite* buf, unsigned char x0, unsigned char y0){
    unsigned int count = 0;

    for (unsigned char j = y0; j < y0 + buf->height; j++){
        for (unsigned char i = x0; i < x0 + buf->width; i++){
            unsigned char *charPos = (unsigned char*) (vidmem + 80*j + i);
            *charPos = buf->chars[count];
            count++;
        }
    }

    return;
}

//Rolls on line from characters start to end on pixel to the left.
//Does not work for test
void rollLeft(unsigned char ln, unsigned char start, unsigned char end){
    unsigned char *adr = (unsigned char*) (vidmem + 80*ln + start);
    unsigned char flip = flipTable[*adr];

    for(unsigned char i = start; i <= end; i++){
        adr++;
        unsigned char nextFlip = flipTable[*adr];
        adr--;
        *adr = (flip & 0b00110101) | (nextFlip & 0b01101010);

        flip = nextFlip;
        adr++;
    }

    adr--;
    *adr &= 0b00110101;
}


//Rolls on line from characters start to end on pixel to the right.
//Does not work for test
void rollRight(unsigned char ln, unsigned char start, unsigned char end){
    unsigned char *adr = (unsigned char*) (vidmem + 80*ln + end);
    unsigned char flip = flipTable[*adr];

    for(unsigned char i = end; i >= start; i--){
        adr--;
        unsigned char nextFlip = flipTable[*adr];
        adr++;
        *adr = (flip & 0b01101010) | (nextFlip & 0b00110101);

        flip = nextFlip;
        adr--;
    }

    adr++;
    *adr &= 0b01101010;
}

int main(void){
    startGraphics();

    unsigned short *p_counter = (unsigned short*) 0x6010;
    short startTime = *p_counter;

#if 1

    fillCircle(10, 10, 7, true);

    drawLine(10, 10, 40, 50, true);
    horzLine(10, 50, 10, true);

    rectangle(8, 35, 20, 50, true);

    circle(50, 35, 10, true);

    vertLine(30, 0, 20, true);
    vertLine(60, 0, 20, true);

    fillRectangle(55, 10, 70, 30, true);

    drawLine(55, 10, 70, 30, false);
    drawLine(70, 10, 55, 30, false);

    vertLine(25, 35, 45, true);

    drawText(60, 50, "hai", 3, true, YELLOWTEXT);
    drawText(47, 35, "Mooi", 4, false, BLUETEXT);

/*
    for (unsigned char i = 0; i < 40; i++){
        for (unsigned char j = 0; j < 20; j++){
            rollLeft(j, 1, 40);
        }
    }
*/


#endif

#if 0
    struct sprite *testSprite = mallocSprite(5, 5);

    getSprite(testSprite, 5, 5);
    drawSprite(testSprite, 9, 5);

    freeSprite(testSprite) ;

#endif
#if 0

    for (unsigned char x = 2; x < 75; x++)
        for (unsigned char y = 0; y < 60; y++)
            setPixel(x, y, true);

#endif

    short endTime = *p_counter;

    sprintf(vidmem + 1840, "done in %d * 0.02 secs", (endTime - startTime));

    return 0;
}