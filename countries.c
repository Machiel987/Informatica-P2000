#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

#define MAXLEN 38
/*
const unsigned char* allCountries[] = {
"afghanistan", "aland islands", "albania", "algeria", "american samoa", 
"andorra", "angola", "anguilla", "antigua and barbuda",
"argentina", "armenia", "aruba", "australia", "austria",
"azerbaijan", "bahamas", "bahrain", "bangladesh", "barbados",
"belarus", "belgium", "belize", "benin", "bermuda", "bhutan",
"bolivia", "caribbean netherlands", "bosnia and herzegovina",
"botswana", "bouvet island", "brazil", "british indian ocean territory", 
"british virgin islands", "brunei", "bulgaria", "burkina faso", 
"burundi", "cambodia", "cameroon", "canada", "cape verde",
"cayman islands", "central african republic", "chad", "chile",
"china", "christmas island", "cocos keeling islands", "colombia",
"comoros", "cook islands", "costa rica", "croatia", "cuba" "curacao", 
"cyprus", "czech republic", "democratic republic of the congo", 
"denmark", "djibouti", "dominica", "dominican republic",
"east timor", "ecuador", "egypt", "el salvador", "equatorial guinea",
"eritrea", "estonia", "ethiopia", "falkland islands", "faroe islands", 
"fiji", "finland", "france", "french guiana", "french polynesia", 
"gabon", "gambia", "gaza strip", "georgia", "germany",
"ghana", "gibraltar", "greece", "greenland", "grenada", "guadeloupe",
"guam", "guatemala", "guernsey", "guinea", "guinea-bissau", "guyana",
"haiti", "honduras", "hong kong", "hungary", "iceland", "india",
"indonesia", "iran", "iraq", "ireland", "isle of man", "israel",
"italy", "ivory coast", "jamaica", "japan", "jersey", "jordan",
"kazakhstan", "kenya", "kiribati", "kosovo", "kuwait", "kyrgyzstan",
"laos", "latvia", "lebanon", "lesotho", "liberia", "libya",
"liechtenstein", "lithuania", "luxembourg", "macau", "north macedonia", 
"madagascar", "malawi", "malaysia", "maldives", "mali",
"malta", "marshall islands", "martinique", "mauritania", "mauritius",
"mayotte", "mexico", "micronesia", "moldova", "monaco", "mongolia",
"montenegro", "montserrat", "morocco", "mozambique", "myanmar",
"namibia", "nauru", "nepal", "netherlands", "new caledonia", "new zealand", 
"nicaragua", "niger", "nigeria", "niue", "norfolk island",
"northern mariana islands", "north korea", "norway", "oman",
"pakistan", "palau", "panama", "papua new guinea", "paraguay",
"peru", "philippines", "pitcairn islands", "poland", "portugal",
"puerto rico", "qatar", "republic of the congo", "réunion",
"romania", "russia", "rwanda", "saint barthelemy", "saint kitts and nevis", 
"saint lucia", "saint martin", "saint pierre and miquelon",
"saint vincent and the grenadines", "samoa", "san marino", "são tomé and príncipe", 
"saudi arabia", "senegal", "serbia", "seychelles",
"sierra leone", "singapore", "sint maarten", "slovakia", "slovenia",
"solomon islands", "somalia", "south africa", "south korea", "south sudan", 
"spain", "sri lanka", "sudan", "suriname", "svalbard",
"eswatini", "sweden", "switzerland", "syria", "taiwan", "tajikistan",
"tanzania", "thailand", "togo", "tokelau", "tonga", "trinidad and tobago", 
"tunisia", "turkey", "turkmenistan", "turks and caicos islands", 
"tuvalu", "uganda", "ukraine", "united arab emirates",
"united kingdom", "united states", "united states virgin islands",
"uruguay", "uzbekistan", "vanuatu", "vatican city", "venezuela",
"vietnam", "wallis and futuna islands", "west bank", "western sahara", 
"yemen", "zambia", "zimbabwe"};
*/

unsigned short allCountries[] = {3551,
17319,
37160,
20108,
22821,
39996,
20409,
42272,
51987,
36145,
41991,
12203,
47428,
64677,
33797,
41933,
7394,
27199,
38645,
35983,
60283,
10312,
5492,
36846,
36402,
10211,
15965,
29260,
20763,
61375,
44922,
28405,
65184,
62535,
45102,
43910,
14740,
13358,
55907,
49375,
16207,
35331,
62076,
24637,
19885,
27467,
37034,
16960,
15796,
22111,
27137,
56411,
52788,
55181,
26455,
50688,
15896,
13958,
16076,
63087,
50425,
53584,
47446,
21419,
25293,
52000,
62139,
58604,
51812,
13285,
13575,
58911,
40131,
18586,
38118,
47245,
8981,
29358,
47180,
18303,
5400,
19436,
24323,
14138,
53306,
17277,
54788,
15063,
9548,
41717,
1295,
4984,
61027,
6515,
60025,
18270,
9393,
31959,
53690,
42603,
11646,
52896,
9203,
56745,
45069,
9610,
40023,
39910,
3171,
55461,
63210,
34814,
20372,
44144,
49098,
40153,
37661,
27547,
50815,
26761,
27084,
50384,
4627,
11051,
51939,
30102,
53982,
31018,
57653,
31292,
64384,
24478,
64263,
20700,
2969,
1186,
52320,
49811,
30454,
9404,
23606,
23091,
63373,
56481,
54809,
47840,
59383,
43609,
21073,
49369,
39919,
28630,
62571,
32573,
59298,
54530,
55596,
34660,
45341,
60317,
30985,
62769,
34990,
26037,
53057,
56271,
44579,
16000,
7098,
63015,
38943,
64032,
35093,
38759,
31984,
60347,
53174,
24342,
54728,
27609,
44795,
58505,
29684,
2307,
12853,
57473,
51078,
29591,
23667,
43775,
42153,
2333,
10436,
43027,
41120,
18445,
36166,
27623,
19174,
12248,
969,
61923,
14887,
33716,
12460,
39477,
61104,
56667,
48820,
52831,
9397,
56587,
10247,
48366,
16785,
25486,
47948,
46314,
50917,
26336,
38445,
33352,
44058,
54780,
14727,
55870,
25312,
63245,
29438,
43901,
41431,
3743,
4263,
13600,
41090,
178,
22255,
29701,
61679,
15719,
14052,
57836,
9833,
45884
};

unsigned char countriesLen = sizeof (allCountries) / sizeof (char*);

static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 71;

//Function generating 16-bit hashcodes
unsigned short crc16(const unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}

static void drawInfoScreen(void){
    drawText(10, 0, "Countries of the world", true, WHITETEXT);

    drawText(4, 9,  "For todays geography lesson, the aim ", false, WHITETEXT);
    drawText(4, 12, "is simple: Name as many countries ", false, WHITETEXT);
    drawText(4, 15, "as you can in 3 minutes of time.", false, WHITETEXT);
    drawText(4, 18, "Although it is not the best", false, WHITETEXT);
    drawText(4, 24, "reflaction of what we do in the ", false, WHITETEXT);
    drawText(4, 27, "lessons, it can be a really fun way", false, WHITETEXT);
    drawText(4, 30, "to test your geography skills.", false, WHITETEXT);
    drawText(4, 33, "Make sure to type the names of the ", false, WHITETEXT);
    drawText(4, 36, "counrties is lowercase letters, ", false, WHITETEXT);
    drawText(4, 39, "use (enter) to input the name of", false, WHITETEXT);
    drawText(4, 42, "the country, and have fun!", false, WHITETEXT);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void countries(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    drawInfoScreen();

    start:
    startGraphics(WHITEGFS);

    drawText(2, 62, "Enter country name:", false, WHITETEXT);

    unsigned int startTime = getTime();
    unsigned char counter = 0;

    unsigned char* guesses = malloc(countriesLen);

    while (getTime() - startTime < 10000){
        while (getKey() == keyEnter);

        unsigned char inputBuf[40];
        memset(inputBuf, '\0', 40);

        for (unsigned int i = 1681; i < 1720; i++)
            *(vidmem + i) = '\0';

        unsigned char prevKey = keyNone;

        while (getKey() != keyEnter){
            unsigned char appendKey = getNiceKeyL();
            if (appendKey)
                strncat(inputBuf, &appendKey, 1);
            else if (getKey() == keyBack && inputBuf[0] != '\0')
                inputBuf[strlen(inputBuf) - 1] = '\0';

            if (strlen(inputBuf) >= MAXLEN)
                inputBuf[strlen(inputBuf) - 1] = '\0';

            drawText(4, 65, inputBuf, true, WHITETEXT);

            while (getKey() == prevKey);
            prevKey = getKey();
        }

        unsigned short inputHash = crc16(inputBuf, strlen(inputBuf));

        unsigned char prevInput = false;
        for (unsigned char i = 0; i < counter; i++){
            if (inputHash == allCountries[guesses[i]])
                prevInput = true;
        }

        if (prevInput){
            drawText(10, 10, "Already saved", true, REDTEXT);
            continue;
        }

        unsigned char countryNum = 255;
        for (unsigned char i = 0; i < countriesLen; i++){
            if (allCountries[i] == inputHash)
                countryNum = i;
        }

        if (countryNum != 255){
            guesses[counter] = countryNum;
            counter++;
            drawText(10, 10, "Country added", true, GREENTEXT);
        }
        else
            drawText(10, 10, "Not A country", true, REDTEXT);

        
        sprintf(vidmem + 800, "Score: %d", counter);
    }

    free(guesses);

    fillRectangle(windowTLX, windowTLY, windowBRX, windowBRY, true);
    fillRectangle(windowTLX, windowTLY, windowBRX, windowBRY, false);

    drawText(10, 16, "Out of time", true, REDTEXT);
    sprintf(vidmem + 800, "Your score was: %d", counter);

    drawText(30, 67, "Press Space to restart", false, WHITETEXT);
    drawText(30, 70, "Press 0 to return to LCL", false, WHITETEXT);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;

    return;
}