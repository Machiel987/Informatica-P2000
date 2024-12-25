#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"

int main(void){
    gameOfLife();
    //walker();

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

    return 0;
}