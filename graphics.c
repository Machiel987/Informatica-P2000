#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "utils.h"

//Create reference to video memory
__at (0x5000) char VIDMEM[];
char* vidmem = VIDMEM;

//LUT where first 12 bits show address of first character in line of pixels and
//last 4 bits show 0, 2 or 4 depending on 'parity' of pixel line
unsigned short yAdrLUT[75];
unsigned char rollTableX[128]; //Table with flipped version of characters
unsigned char rollTableYUp[128]; //Table with up-shifted version of charecters
unsigned char rollTableYDn[128]; //Table with down-shifted version of characters

unsigned char pxNumToChar[] = {1,2,4,8,16,64};

unsigned char windowTLX = 2;
unsigned char windowTLY = 0;
unsigned char windowBRX = 79;
unsigned char windowBRY = 71;

void initializeScreen(unsigned char color){
    for (unsigned int i = 0; i < 1919; i++) vidmem[i] = 32;
    for (unsigned int i = windowTLY / 3; i <= windowBRY / 3; i ++) vidmem[80*i + windowTLX / 2 - 1] = color;
}

//Fills all LUTs and initializes screen, must be called before attempting graphics routines
void startGraphics(unsigned char color){
    initializeScreen(color);
    for (unsigned char i = 0; i < 75; i++) yAdrLUT[i] = 0x5000 + 80 * (i/3) + 2 * (i%3);

    //Deeply magical bit shi(f)t
    for (unsigned char i = 0; i < 128; i++){
        unsigned char lft = ((i >> 1) & 0x1F) & ((i >> 2) | 0xF);
        unsigned char rgt = ((i << 1) & 0x5F) & ((i << 2) | 0x3F);
        rollTableX[i] = (lft & 0x15) | (rgt & 0x4A) | 0x20;
    }
    for (unsigned char i = 0; i < 128; i++){
        unsigned char top = ((i >> 2) & 0xF) & (((i & 0x40) >> 3) | 7);
        unsigned char bot = ((i & 1) << 4) | ((i & 2) << 5);
        rollTableYUp[i] = bot | top | 0x20;
    }
    for (unsigned char i = 0; i < 128; i++){
        rollTableYDn[i] = rollTableYUp[rollTableYUp[i]];
    }

    return;
}

//Sets drawing window
void setWindow(unsigned char TLX, unsigned char TLY, unsigned char BRX, unsigned char BRY){
    windowTLX = TLX;
    windowTLY = TLY;
    windowBRX = BRX;
    windowBRY = BRY;
}
#if 0
unsigned char* getWindow(void){
    unsigned char out[4] = {windowTLX, windowTLY, windowBRX, windowBRY};
    return out;
}
#endif

inline static unsigned char inRange(unsigned char x, unsigned char y){
    return (x >= windowTLX && x <= windowBRX && y >= windowTLY && y <= windowBRY);
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
#if 0
inline static void unsafeSetPixelOn(unsigned char x, unsigned char y){
    unsigned char* charAdr  = (unsigned char*) (yAdrLUT[y] & 0xFFF0) + (x >> 1);
    unsigned char  pixelNum = (x & 1) + (yAdrLUT[y] & 0xF);

    unsigned char newChar = *charAdr | pxNumToChar[pixelNum];

    *charAdr = newChar;

    return;
}
#endif

//Set pixel to OFF, without screen check
#if 0
inline static void unsafeSetPixelOff(unsigned char x, unsigned char y){
    unsigned char* charAdr  = (unsigned char*) (yAdrLUT[y] & 0xFFF0) + (x >> 1);
    unsigned char  pixelNum = (x & 1) + (yAdrLUT[y] & 0xF);

    unsigned char newChar = *charAdr & ~pxNumToChar[pixelNum];

    *charAdr = newChar;
}
#endif

//Sets a given pixel. Coordinates are in pixel, not character coordinates
//wt = white (1 if pixel will be set to white, 0 if set to black)
void setPixel(unsigned char x, unsigned char y, unsigned char wt){
    if (!inRange(x, y)) return;

    unsafeSetPixel(x, y, wt);

    return;
}

unsigned char getPixel(unsigned char x, unsigned char y){
    unsigned char* charAdr = (unsigned char*) (yAdrLUT[y] & 0xFFF0) + (x >> 1);
    unsigned char  pixelNum = (x & 1) + (yAdrLUT[y] & 0xF);

    return ((*charAdr) & pxNumToChar[pixelNum]) != 0;
}

//Draws or erases a line between any 2 screen coordinates.
//Uses a generalized version of Bresenham's line algorithm
void drawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt){
    signed char dx = abs(x1 - x0);
    signed char sx = x0 < x1 ? 1 : -1;

    signed char dy = -abs(y1 - y0);
    signed char sy = y0 < y1 ? 1 : -1;

    int error = dx + dy;
    int e2;

    while(true){
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

//Draws or erases a horizontal line without window checking
void unsafeHorzLine(unsigned char xmin, unsigned char xmax, unsigned char y, unsigned char wt){
    unsigned char* bgnAdr = (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + ((xmin + 1) >> 1));
    //unsigned char* endAdr = (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + ((xmax + 1) >> 1));

    unsigned char maxI = ((xmax + 1) >> 1) - ((xmin + 1) >> 1);
    unsigned char* adr = bgnAdr;

    if (wt){
        switch (yAdrLUT[y] & 0xF)
        {
        case 0:
            for (unsigned char i = 0; maxI - i > 0; i++){
                *adr |= 0b00100011;
                adr++;
            }
            break;
        
        case 2:
            for (unsigned char i = 0; maxI - i > 0; i++){
                *adr |= 0b00101100;
                adr++;
            }
            break;

        case 4:
            for (unsigned char i = 0; maxI - i > 0; i++){
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
            for (unsigned char i = 0; maxI - i > 0; i++){
                *adr &= ~0b00000011;
                adr++;
            }
            break;
        
        case 2:
            for (unsigned char i = 0; maxI - i > 0; i++){
                *adr &= ~0b00001100;
                adr++;
            }
            break;

        case 4:
            for (unsigned char i = 0; maxI - i > 0; i++){
                *adr &= ~0b01010000;
                adr++;
            }
            break;
        }
    }

    setPixel(xmin, y, wt);
    setPixel(xmax, y, wt);
}

//Draws or erases a horizontal line
void horzLine(unsigned char x0, unsigned char x1, unsigned char y, unsigned char wt){
    if (y < windowTLY || y > windowBRY) return;

    unsigned char xmin = MIN(x0, x1);
    xmin = MAX(xmin, windowTLX);
    unsigned char xmax = MAX(x0, x1);
    xmax = MIN(xmax, windowBRX);

    unsafeHorzLine(xmin, xmax, y, wt);
}

//Draws a horizontal line in the given color (use _COLOR_GFS)
#if 0
void horzLineColor(unsigned char x0, unsigned char x1, unsigned char y, unsigned char color){
    unsigned char xmin = MIN(x0, x1);
    xmin = MAX(xmin, windowTLX) + 1;
    unsigned char xmax = MAX(x0, x1);
    xmax = MIN(xmax, windowBRX) - 1;

    if (y < windowTLY || y > windowBRY) return;

    * (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + (xmin >> 1) - 1) = color;
    * (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + (xmax >> 1) + 1) = WHITEGFS;

    for (unsigned char i = xmin; i <= xmax; i++){
        setPixel(i, y, true);
    }
}
#endif

//Draws or erases a vertical line without window checking
void unsafeVertLine(unsigned char x, unsigned char ymin, unsigned char ymax, unsigned char wt){
    unsigned char* bgnAdr = (unsigned char*) ((yAdrLUT[ymin + 2] & 0xFFF0) + (x >> 1));
    unsigned char* endAdr = (unsigned char*) ((yAdrLUT[ymax + 1] & 0xFFF0) + (x >> 1));

    setPixel(x, ymin, wt);
    setPixel(x, ymax, wt);

    if (ymin != ymax){
        setPixel(x, ymin + 1, wt);
        setPixel(x, ymax - 1, wt);
    }

    if (bgnAdr > endAdr) return;

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
                *adr &= ~0b00010101;
                adr += 80;
            }
            break;
        
        case 1:
            while (endAdr - adr > 0){
                *adr &= ~0b01001010;
                adr += 80;
            }
            break;
        }
    }
    
    return;
}

//Draws or erases a vertical line
void vertLine(unsigned char x, unsigned char y0, unsigned char y1, unsigned char wt){
    if (x < windowTLX || x > windowBRX) return;

    unsigned char ymin = MIN(y0, y1);
    ymin = MAX(ymin, windowTLY);
    unsigned char ymax = MAX(y0, y1);
    ymax = MIN(ymax, windowBRY);

    if (ymax < ymin) return; //Really unusual, but this does occasionally happen due to weird clipping at the window

    unsafeVertLine(x, ymin, ymax, wt);
}

//Draws a vertical line in the given color (use _COLOR_GFS)
#if 0
void vertLineColor(unsigned char x, unsigned char y0, unsigned char y1, unsigned char color){
    unsigned char ymin = MIN(y0, y1);
    ymin = MAX(ymin, windowTLY);
    unsigned char ymax = MAX(y0, y1);
    ymax = MIN(ymax, windowBRY);

    if (x < windowTLX - 1 || x > windowBRX) return;

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (x >> 1) - 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (x >> 1) - 1; i += 80){
        * (unsigned char*) i = color;
    }

    vertLine(x, y0, y1, true);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (x >> 1) + 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (x >> 1) + 1; i += 80)
        * (unsigned char*) i = WHITEGFS;
}
#endif

//Draws or erases a hollow rectangle
void rectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt){
    horzLine(x0, x1, y0, wt);
    horzLine(x0, x1, y1, wt);
    vertLine(x0, y0, y1, wt);
    vertLine(x1, y0, y1, wt);
}

//Draws a hollow rectange in the given color (use _COLOR_GFS)
#if 0
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
#endif

//Draws or erases a filled rectangle
#if 1
void fillRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt){
    unsigned char xmin = MIN(x0, x1);
    xmin = MAX(xmin, windowTLX);
    unsigned char xmax = MAX(x0, x1);
    xmax = MIN(xmax, windowBRX);
    unsigned char ymin = MIN(y0, y1);
    ymin = MAX(ymin, windowTLY);
    unsigned char ymax = MAX(y0, y1);
    ymax = MIN(ymax, windowBRY);

    unsigned char xminOl = xmin % 2;
    unsigned char xmaxOl = xmax % 2;

    if (xminOl) vertLine(xmin, ymin, ymax, wt);
    if (!xmaxOl) vertLine(xmax, ymin, ymax, wt);

    unsigned char yminOl = yAdrLUT[ymin] & 0xF;
    unsigned char ymaxOl = yAdrLUT[ymax] & 0xF;

    switch (yminOl)
    {
    case 2:
        horzLine(xmin, xmax, ymin + 1, wt);

    case 4:
        horzLine(xmin, xmax, ymin, wt);
    }

    switch (ymaxOl)
    {
    case 2:
        horzLine(xmin, xmax, ymax - 1, wt);
    
    case 0:
        horzLine(xmin, xmax, ymax, wt);
    }

    for (unsigned int yAdr = yAdrLUT[ymin + 2] & 0xFFF0; yAdr <= (yAdrLUT[ymax - 2] & 0xFFF0); yAdr += 80){
        for (unsigned int xAdr = ((1 + xmin) >> 1); xAdr < ((xmax + 1) >> 1); xAdr++){
            unsigned char* p_char = (unsigned char*) (yAdr + xAdr);
            if (wt) *p_char = 127;
            else *p_char = 32;
        }
    }
}
#endif

//Draws a filled rectangle in the given color (use _COLOR_GFS)
#if 0
void fillRectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color){
    unsigned char xmin = MIN(x0, x1);
    xmin = MAX(xmin, windowTLX);
    unsigned char xmax = MAX(x0, x1);
    xmax = MIN(xmax, windowBRX);
    unsigned char ymin = MIN(y0, y1);
    ymin = MAX(ymin, windowTLY);
    unsigned char ymax = MAX(y0, y1);
    ymax = MIN(ymax, windowBRY);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (xmin >> 1) - 1; i <= (yAdrLUT[ymax] & 0xFFF0) + (xmin >> 1) - 1; i += 80)
        * (unsigned char*) i = color;

    fillRectangle(x0, y0, x1, y1, true);

    for (unsigned int i = (yAdrLUT[ymin] & 0xFFF0) + (xmax >> 1) + 1; i <= (yAdrLUT[ymax] & 0xFF0C) + (xmax >> 1) + 1; i += 80)
        * (unsigned char*) i = WHITEGFS;
}
#endif

//Prints text in a given color to the screen. This text can be normal or double height
//Due to the text being drawn on pixel coordinates, the text might not match the desired location perfectly
void drawText(unsigned char x, unsigned char y, char* text, unsigned char dblH, unsigned char color){
    unsigned char* startAdr = (unsigned char*) ((yAdrLUT[y] & 0xFFF0) + (x >> 1));
    size_t textLen = strlen(text);

    if ((!inRange(x, y)) || (!inRange(x + 2 * textLen - 2, y))) return;

    if (dblH) {*(startAdr - 2) = color; *(startAdr - 1) = 0xD;}
    else *(startAdr - 1) = color;

    for (size_t i = 0; i < textLen; i++){
        *(startAdr + i) = text[i];
    }
    
    if (dblH) {{*(startAdr + textLen) = 0xC; *(startAdr + textLen + 1) = WHITEGFS;}}
    else {*(startAdr + textLen) = WHITEGFS;}
}

//Draws or erases a safe/unsafe circle depending on function pointer
#if 0
void circleSU(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt, void (*f)(unsigned char, unsigned char, unsigned char)){
    unsigned char t1 = r / 16;
    unsigned char x = r;
    unsigned char y = 0;

    while (x - y >= 0){
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
#endif

//Draws or erases a hollow circle using the midpoint circle algorithm
#if 0
void circle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt){
    if (inRange(xm - r, ym - r) && inRange(xm + r, ym + r))
        circleSU(xm, ym, r, wt, unsafeSetPixel);
    else
        circleSU(xm, ym, r, wt, setPixel);
}
#endif

//Draws a circle in the given color usign the midpoint circle algorithm
#if 0
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
#endif

//Reserve memory space for a sprite of determined dimensions
//Dimensions are in CHARACTER coordinates
#if 0
struct sprite* mallocSprite(unsigned char width, unsigned char height){
    struct sprite *out = malloc(sizeof(struct sprite)) ;
    out->width = width;
    out->height = height;
    out->chars = (unsigned char*)malloc(width * height);

    return out;
}
#endif

//Frees the memory reserved for given sprite
#if 0
void freeSprite(struct sprite *s){
    free(s->chars) ;
    free(s) ;
}
#endif

//Saves a sprite from the characters on screen in given location
//Dimensions are in CHARACTER coordinates
#if 0
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
#endif

//Draws a previously saves sprite on the screen
//Dimensions are in CHARACTER coordinates
#if 0
void drawSprite(struct sprite* buf, unsigned char x0, unsigned char y0){
    if ((!inRange(x0, y0)) || (!inRange(x0 + buf->width, y0 + buf->height))) return;

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
#endif

//Rolls a line from characters start to end one pixel to the left, going from startAdr to endAdr
void rollLeft(unsigned char length, unsigned char* startAdr) __sdcccall(1);
#if 0
void rollLeft(unsigned char length, unsigned char* startAdr){
    unsigned char* currentAdr = startAdr;
    unsigned char flip = rollTableX[*currentAdr];

    for (unsigned char i = 0; i < length; i++){
        currentAdr++;
        unsigned char nextFlip = rollTableX[*currentAdr];
        currentAdr--;
        *currentAdr = (flip & 0b00110101) | (nextFlip & 0b01101010);

        flip = nextFlip;
        currentAdr++;
    }

    currentAdr--;
    *currentAdr &= 0b00110101;
}
#endif

//Rolls a line from characters start to end one pixel to the right, going from startAdr to endAdr
void rollRight(unsigned char length, unsigned char* endAdr) __sdcccall(1);
#if 0
void rollRight(unsigned char length, unsigned char* endAdr) __sdcccall(1) {
    unsigned char* currentAdr = endAdr;
    unsigned char flip = rollTableX[*currentAdr];

    for (unsigned char i = 0; i < length; i++){
        currentAdr--;
        unsigned char nextFlip = rollTableX[*currentAdr];
        currentAdr++;
        *currentAdr = (flip & 0b01101010) | (nextFlip & 0b00110101);

        flip = nextFlip;
        currentAdr--;
    }

    currentAdr++;
    *currentAdr &= 0b01101010;
}
#endif

//Rolls a column from characters start to end one pixel up, going from startAdr to endAdr
void rollUp(unsigned char length, unsigned char* startAdr) __sdcccall(1);
#if 0
void rollUp(unsigned char length, unsigned char* startAdr){
    unsigned char* currentAdr = startAdr;
    unsigned char flip = rollTableYUp[*currentAdr];

    for (unsigned char i = 0; i < length; i++){
        currentAdr += 80;
        unsigned char nextFlip = rollTableYUp[*currentAdr];
        currentAdr -= 80;
        *currentAdr = (flip & 0xF) | (nextFlip & 0x70);
        
        flip = nextFlip;
        currentAdr += 80;
    }

    currentAdr -= 80;
    *currentAdr &= 0x2F;
}
#endif

//Rolls a column from characters start to end one pixel down, going from startAdr to endAdr
void rollDown(unsigned char length, unsigned char* endAdr) __sdcccall(1);
#if 0
void rollDown(unsigned char length, unsigned char* endAdr){
    unsigned char* currentAdr = endAdr;
    unsigned char flip = rollTableYDn[*currentAdr];

    for (unsigned char i = 0; i < length; i++){
        currentAdr -= 80;
        unsigned char nextFlip = rollTableYDn[*currentAdr];
        currentAdr += 80;
        *currentAdr = (flip & 0x7C) | (nextFlip & 3);
        
        flip = nextFlip;
        currentAdr -= 80;
    }

    currentAdr += 80;
    *currentAdr &= 0x7C;
}
#endif

//Rolls a given area to the left
void rollAreaLeft(void){
    if ((!inRange(windowTLX, windowTLY)) || (!inRange(windowBRX, windowBRY))) return;

    unsigned char* startAdr = (unsigned char*) (yAdrLUT[windowTLY] & (0xFFF0)) + (windowTLX >> 1);
    unsigned char length = (windowBRX - windowTLX + 1) >> 1;

    unsigned char* endLineAdr = (unsigned char*) (yAdrLUT[windowBRY] & (0xFFF0)) + (windowTLX >> 1);

    while (startAdr <= endLineAdr){
        rollLeft(length, startAdr);
        startAdr += 80;
    }
}

//Rolls a given area to the right
void rollAreaRight(void){
    if ((!inRange(windowTLX, windowTLY)) || (!inRange(windowBRX, windowBRY))) return;

    unsigned char* endAdr = (unsigned char*) (yAdrLUT[windowTLY] & (0xFFF0)) + (windowBRX >> 1);
    unsigned char length = (windowBRX - windowTLX + 1) >> 1;

    unsigned char* endLineAdr = (unsigned char*) (yAdrLUT[windowBRY] & (0xFFF0)) + (windowBRX >> 1);

    while (endAdr <= endLineAdr){
        rollRight(length, endAdr);
        endAdr += 80;
    }
}

//Rolls a given area up
void rollAreaUp(void){
    if ((!inRange(windowTLX, windowTLY)) || (!inRange(windowBRX, windowBRY))) return;

    unsigned char* startAdr = (unsigned char*) (yAdrLUT[windowTLY] & (0xFFF0)) + (windowTLX >> 1);;
    unsigned char length = (windowBRY - windowTLY + 1) / 3;

    unsigned char* endColAdr = (unsigned char*) (yAdrLUT[windowTLY] & (0xFFF0)) + (windowBRX >> 1);

    while (startAdr <= endColAdr){
        rollUp(length, startAdr);
        startAdr++;
    }
}

//Rolls a given area down
void rollAreaDown(void){
    if ((!inRange(windowTLX, windowTLY)) || (!inRange(windowBRX, windowBRY))) return;

    unsigned char* endAdr = (unsigned char*) (yAdrLUT[windowBRY] & (0xFFF0)) + (windowTLX >> 1);;
    unsigned char length = (windowBRY - windowTLY + 1) / 3;

    unsigned char* endColAdr = (unsigned char*) (yAdrLUT[windowBRY] & (0xFFF0)) + (windowBRX >> 1);

    while (endAdr <= endColAdr){
        rollDown(length, endAdr);
        endAdr++;
    }
}