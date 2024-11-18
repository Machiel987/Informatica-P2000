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

//Function declarations
void startGraphics(void);
inline void setPixel(unsigned char x, unsigned char y, unsigned char wt);
void drawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt);
//void drawLineColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
void horzLine(unsigned char x0, unsigned char x1, unsigned char y, unsigned char wt);
void horzLineColor(unsigned char x0, unsigned char x1, unsigned char y, unsigned char color);
void vertLine(unsigned char x, unsigned char y0, unsigned char y1, unsigned char wt);
void vertLineColor(unsigned char x, unsigned char y0, unsigned char y1, unsigned char color);
void rectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt);
void rectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
void fillRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char wt);
void fillRectangleColor(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
void drawText(unsigned char x, unsigned char y, char* text, unsigned char textLen, unsigned char dblH, unsigned char color);
void circle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt);
void fillCircle(unsigned char xm, unsigned char ym, unsigned char r, unsigned char wt);
struct sprite* mallocSprite(unsigned char width, unsigned char height);
void freeSprite(struct sprite *s);
void getSprite(struct sprite* buf, unsigned char x0, unsigned char y0);
void rollLeft(unsigned char ln, unsigned char start, unsigned char end);
void rollRight(unsigned char ln, unsigned char start, unsigned char end);
