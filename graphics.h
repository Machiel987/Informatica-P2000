//Header file for graphics

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

//Colors for using pixels on the screen
#define REDGFS 17
#define GREENGFS 18
#define YELLOWGFS 19
#define BLUEGFS 20
#define MAGENTAGFS 21
#define CYANGFS 22
#define WHITEGFS 23

//Data types
struct sprite{
    unsigned char width;
    unsigned char height;
    unsigned char* chars;
};

//Constants
extern char* vidmem;
extern unsigned int yAdrLUT[75];
extern unsigned char pxNumToChar[];
#define pxNumToChar0 ((unsigned char) 1)
#define pxNumToChar1 ((unsigned char) 2)
#define pxNumToChar2 ((unsigned char) 4)
#define pxNumToChar3 ((unsigned char) 8)
#define pxNumToChar4 ((unsigned char)16)
#define pxNumToChar5 ((unsigned char)64)

//Function declarations
//void initializeScreen(void);
void startGraphics(unsigned char color);
void setWindow(unsigned char TLX, unsigned char TLY, unsigned char BRX, unsigned char BRY);
//unsigned char* getWindow(void);
void setPixel(unsigned char x, unsigned char y, unsigned char wt) __sdcccall(1) ;
//void unsafeSetPixelOn(unsigned char x, unsigned char y) __sdcccall(1) ;
//void unsafeSetPixelOff(unsigned char x, unsigned char y) __sdcccall(1) ;
unsigned char getPixel(unsigned char x, unsigned char y);
void drawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt);
//void drawLineColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
void unsafeHorzLine(unsigned char xmin, unsigned char xmax, unsigned char y, unsigned char wt);
void horzLine(unsigned char x0, unsigned char x1, unsigned char y, unsigned char wt);
//void horzLineColor(unsigned char x0, unsigned char x1, unsigned char y, unsigned char color);
void unsafeVertLine(unsigned char x, unsigned char ymin, unsigned char ymax, unsigned char wt);
void vertLine(unsigned char x, unsigned char y0, unsigned char y1, unsigned char wt);
//void vertLineColor(unsigned char x, unsigned char y0, unsigned char y1, unsigned char color);
void rectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt);
//void rectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
void fillRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt);
void unsafeFillRectangle(unsigned char xmin, unsigned char ymin, unsigned char xmax, unsigned char ymax, unsigned char wt);
//void fillRectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
void drawChar(unsigned char x, unsigned char y, unsigned char in, unsigned char dblH, unsigned char color);
void drawText(unsigned char x, unsigned char y, char* text, unsigned char dblH, unsigned char color);
void drawCircle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt);
//void fillCircle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt);
//struct sprite* mallocSprite(unsigned char width, unsigned char height);
//void freeSprite(struct sprite *s);
//void getSprite(struct sprite* buf, unsigned char x0, unsigned char y0);
void rollAreaLeft(void);
void rollAreaRight(void);
void rollAreaUp(void);
void rollAreaDown(void);
