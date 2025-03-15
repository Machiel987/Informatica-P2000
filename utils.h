//Header file for utility functions

#define GT_INT(a, b) (((a) ^ 0x8000) > ((b) ^ 0x8000))
#define LT_INT(a, b) (((a) ^ 0x8000) < ((b) ^ 0x8000))

//Functions for finding min and max (defined in macro for better performance)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define MAX_INT(a, b) (((a) ^ 0x8000) > ((b) ^ 0x8000) ? (a) : (b))
#define MIN_INT(a, b) (((a) ^ 0x8000) < ((b) ^ 0x8000) ? (a) : (b))

#define CLIP(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

unsigned int getTime(void);
