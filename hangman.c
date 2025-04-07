#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

unsigned char* words[300] = {
"aan", "aanbod", "aanraken", "aanval", "aap", "aardappel", "aarde",
"aardig", "acht", "achter", "actief", "activiteit", "ademen", "af",
"afgelopen", "afhangen", "afmaken", "afname", "afspraak", "afval",
"al", "algemeen", "alleen", "alles", "als", "alsjeblieft", "altijd",
"ander", "andere", "anders", "angst", "antwoord", "antwoorden",
"appel", "arm", "auto", "avond", "avondeten", "baan", "baby", "bad",
"bal", "bang", "bank", "basis", "bed", "bedekken", "bedreiging",
"bedreven", "been", "beer", "beest", "beetje", "begin", "begrijpen",
"begrip", "behalve", "beide", "beker", "bel", "belangrijk", "bellen",
"belofte", "beneden", "benzine", "berg", "beroemd", "beroep",
"bescherm", "beslissen", "best", "betalen", "beter", "bevatten",
"bewegen", "bewolkt", "bezoek", "bibliotheek", "bieden", "bij",
"bijna", "bijten", "bijvoorbeeld", "bijzonder", "binnen",
"binnenkort", "blad", "blauw", "blazen", "blij", "blijven", "bloed",
"bloem", "bodem", "boek", "boerderij", "boete", "boom", "boon", 
"boord", "boos", "bord", "borstelen", "bos", "bot", "bouwen", 
"boven", "branden", "brandstof", "breed", "breken", "brengen", 
"brief", "broer", "broek", "brood", "brug", "bruikbaar", "bruiloft", 
"bruin", "bui", "buiten", "bureau", "buren", "bus", "buurman", 
"buurvrouw", "cadeau", "chocolade", "cirkel", "comfortabel", 
"compleet", "computer", "conditie", "controle", "cool", "correct", 
"daar", "daarom", "dag", "dak", "dan", "dansen", "dapper", "dat", 
"de", "deel", "deken", "deksel", "delen", "derde", "deze", 
"dichtbij", "dienen", "diep", "dier", "dik", "ding", "dit", 
"dochter", "doen", "dom", "donker", "dood", "door", "doorzichtig", 
"doos", "dorp", "draad", "draaien", "dragen", "drie", "drijven", 
"drinken", "drogen", "dromen", "droog", "druk", "dubbel", "dun", 
"dus", "duur", "duwen", "echt", "een", "een", "eend", "eenheid", 
"eenzaam", "eerste", "eeuw", "effect", "ei", "eigen", "eiland", 
"einde", "eis", "elektrisch", "elk", "en", "enkele", "enthousiast", 
"erg", "eten", "even", "examen", "extreem", "falen", "familie", 
"feest", "feit", "fel", "fijn", "film", "fit", "fles", "foto", 
"fout", "fris", "fruit", "gaan", "gat", "gebeuren", "gebeurtenis", 
"gebied", "geboorte", "geboren", "gebruik", "gebruikelijk", 
"gebruiken", "gedrag", "gedragen", "geel", "geen", "gehoorzamen", 
"geit", "geld", "geliefde", "gelijk", "geloof", "geluid", "geluk", 
"gemak", "gemakkelijk", "gemeen", "genieten", "genoeg", "genot", 
"gerecht", "gereedschap", "geschikt", "gespannen", "geur", "gevaar", 
"gevaarlijk", "gevangenis", "geven", "gevolg", "gewicht", "gewoon", 
"gezicht", "gezond", "gif", "gisteren", "glad", "glas", "glimlach", 
"god", "goed", "goedkoop", "goud", "graf", "grap", "grappig", "gras", 
"grens", "grijs", "groeien", "groen", "groente", "groep", "grof", 
"grond", "groot", "grootmoeder", "grootvader", "haan", "haar", 
"haast", "hal", "halen", "half", "hallo", "hamer", "hand", "hard", 
"hart", "haten", "hebben", "heel", "heet", "helder", "helft", "help", 
"hem", "hemel", "hen", "herfst", "herinneren", "hert", "het", 
"heuvel", "hier", "hij", "hobby", "hoe", "hoed", "hoek", "hoeveel", 
"hoeveelheid", "hoewel", "hond", "honderd", "honger", "hoofd", 
"hoog", "hoogte", "hoop", "horen", "hotel", "houden", "huilen", 
"huis", "hun", "huren", "hut", "huur", "idee", "ieder", "iedereen", 
"iemand", "iets", "ijs", "ijzer", "ik", "in", "instrument", "ja", 
"jaar", "jagen", "jas", "jij", "jong", "jongen", "jouw", "jullie", 
"kaars", "kaart", "kaas", "kamer", "kans", "kant", "kantoor", "kap", 
"kast", "kasteel", "kat", "kennen", "kennis", "keuken", "keus", 
"kiezen", "kijken", "kind", "kip", "kist", "klaar", "klas", "klasse", 
"kleden", "klein", "kleren", "kleur", "klimmen", "klok", "kloppen", 
"klopt", "knie", "knippen", "koers", "koffer", "koffie", "kok", 
"koken", "kom", "komen", "koning", "koningin", "koorts", "kop", 
"kopen", "kort", "kost", "kosten", "koud", "kraam", "kracht", 
"krant", "krijgen", "kruis", "kuil", "kunnen", "kunst", "laag", 
"laat", "laatst", "lach", "lachen", "ladder", "laken", "lamp", 
"land", "lang", "langs", "langzaam", "laten", "leeftijd", "leeg", 
"leerling", "leeuw", "leger", "leiden", "lenen", "lengte", "lepel", 
"leren", "les", "leuk", "leven", "lezen", "lichaam", "licht", 
"liefde", "liegen", "liggen", "lijk", "lijken", "liniaal", "links",
"lip", "list", "lomp", "lood", "lopen", "los", "lot", "lucht", "lui", 
"luisteren", "lunch", "maag", "maal", "maaltijd", "maan", "maand", 
"maar", "maat", "machine", "maken", "makkelijk", "mama", "man", 
"mand", "manier", "map", "markeren", "markt", "me", "medicijn", 
"meel", "meer", "meerdere", "meest", "meisje", "melk", "meneer", 
"mengsel", "mensen", "mes", "met", "meubel", "mevrouw", "middel",
"midden", "mij", "mijn", "miljoen", "min", "minder", "minuut", "mis", 
"missen", "mits", "model", "modern", "moeder", "moeilijk", "moeten", 
"mogelijk", "mogen", "moment", "mond", "mooi", "moord", "moorden", 
"morgen", "munt", "muziek", "na", "naald", "naam", "naar", "naast",
"nacht", "nat", "natuur", "natuurlijk", "nee", "neer", "negen", 
"nek", "nemen", "net", "netjes", "neus", "niet", "niets", "nieuw", 
"nieuws", "nobel", "noch", "nodig", "noemen", "nog", "nood", "nooit", 
"noord", "noot", "normaal", "nu", "nul", "nummer", "object", 
"oceaan", "ochtend", "oefening", "of", "offer", "olie", "olifant", 
"om", "oma", "onder", "onderwerp", "onderzoek", "oneven", "ongeluk", 
"ons", "ontsnappen", "ontbijt", "ontdekken", "ontmoeten", 
"ontvangen", "ontwikkelen", "onze", "oog", "ooit", "ook", "oom", 
"oor", "oorlog", "oorzaak", "oost", "op", "opa", "opeens", "open", 
"openlijk", "opleiding", "opnemen", "oranje", "orde", "oud", "ouder", 
"over", "overal", "overeenkomen", "overleden", "overvallen", "paar", 
"paard", "pad", "pagina", "pan", "papa", "papier", "park", "partner", 
"pas", "passeren", "pen", "peper", "per", "perfect", "periode", 
"persoon", "piano", "pijn", "pistool", "plaat", "plaatje", "plaats", 
"plafond", "plank", "plant", "plastic", "plat", "plattegrond", 
"plein", "plus", "poes", "politie", "poort", "populair", "positie", 
"postzegel", "potlood", "praten", "presenteren", "prijs", "prins", 
"prinses", "prive", "proberen", "probleem", "product", "provincie", 
"publiek", "punt", "raak", "raam", "radio", "raken", "rapport", 
"recht", "rechtdoor", "rechts", "rechtvaardig", "redden", "reeds", 
"regen", "reiken", "reizen", "rekenmachine", "rennen", "repareren", 
"rest", "restaurant", "resultaat", "richting", "rijk", "rijst", 
"rijzen", "ring", "rok", "rond", "rood", "rook", "rots", "roze", 
"rubber", "ruiken", "ruimte", "samen", "sap", "schaap", "schaar", 
"schaduw", "scheiden", "scherp", "schetsen", "schieten", "schijnen", 
"schip", "school", "schoon", "schouder", "schreeuw", "schreeuwen", 
"schrijven", "schudden", "seconde", "sex", "signaal", "simpel", 
"sinds", "slaapkamer", "slapen", "slecht", "sleutel", "slim", "slot", 
"sluiten", "smaak", "smal", "sneeuw", "snel", "snelheid", "snijden", 
"soep", "sok", "soms", "soort", "sorry", "speciaal", "spel", 
"spelen", "sport", "spreken", "springen", "staal", "stad", "stap", 
"start", "station", "steen", "stelen", "stem", "stempel", "ster", 
"sterk", "steun", "stil", "stilte", "stoel", "stof", "stoffig", 
"stom", "stop", "storm", "straat", "straffen", "structuur", 
"student", "studie", "stuk", "succes", "suiker", "taal", "taart", 
"tafel", "tak", "tamelijk", "tand", "tante", "tas", "taxi", "te", 
"team", "teen", "tegen", "teken", "tekenen", "telefoon", "televisie", 
"tellen", "tennis", "terug", "terugkomst", "terwijl", "test", 
"tevreden", "thee", "thuis", "tien", "tijd", "titel", "toekomst", 
"toen", "toename", "totaal", "traan", "tram", "trein", "trekken", 
"trouwen", "trui", "tuin", "tussen", "tweede", "u", "uit", 
"uitleggen", "uitnodigen", "uitvinden", "uitzoeken", "uur", "vaak", 
"vaarwel", "vader", "vak", "vakantie", "vallen", "vals", "van", 
"vandaag", "vangen", "vanmorgen", "vannacht", "varken", "vast", 
"vechten", "veel", "veer", "veilig", "ver", "veranderen", 
"verandering", "verder", "verdienen", "verdrietig", "verenigen", 
"verf", "vergelijkbaar", "vergelijken", "vergelijking", "vergeten", 
"vergeven", "vergissen", "verhaal", "verhoging", "verjaardag", 
"verkeerd", "verkopen", "verlaten", "verleden", "verliezen", 
"vernietigen", "veroveren", "verrassen", "vers", "verschil", 
"verschrikkelijk", "verspreiden", "verstand", "verstoppen", 
"versturen", "vertellen", "vertrekken", "vertrouwen", "verwachten", 
"verwijderen", "verzamelen", "verzameling", "vet", "vier", 
"vierkant", "vies", "vijand", "vijf", "vijver", "vinden", "vinger", 
"vis", "vlag", "vlees", "vlieg", "vliegtuig", "vloer", "voeden", 
"voedsel", "voelen", "voet", "voetbal", "vogel", "vol", "volgende", 
"volgorde", "voor", "voorbeeld", "voorkomen", "voorzichtig", 
"voorzien", "vork", "vorm", "vos", "vouwen", "vraag", "vragen", 
"vrede", "vreemd", "vreemde", "vriend", "vriendelijk", "vriezen", 
"vrij", "vrijheid", "vroeg", "vroeger", "vrouw", "vullen", "vuur", 
"waar", "waarom", "waarschijnlijk", "wachten", "wakker", "wanneer", 
"want", "wapen", "warm", "wassen", "wat", "water", "we", "week", 
"weer", "weg", "welke", "welkom", "wens", "wereld", "werelddeel", 
"werk", "west", "wetenschap", "wie", "wiel", "wij", "wijn", "wijs", 
"wild", "willen", "wind", "winkel", "winnen", "winter", "wissen", 
"wit", "wolf", "wolk", "wonder", "woord", "woud", "wreed", "zaak", 
"zacht", "zak", "zand", "zee", "zeep", "zeer", "zeggen", "zeil", 
"zeker", "zelfde", "zes", "zetten", "zeven", "ziek", "ziekenhuis", 
"ziel", "zien", "zij", "zijn", "zilver", "zingen", "zinken", 
"zitten", "zo", "zoals", "zoeken", "zoet", "zomer", "zon", "zonder", 
"zonnig", "zoon", "zorg", "zorgen", "zou", "zout", "zuid", "zulke", 
"zullen", "zus", "zwaar", "zwak", "zwembad", "zwemmen"
};

//Co-ordinates of the entire window
const static unsigned char windowTLX = 2, windowTLY = 0, windowBRX = 79, windowBRY = 70;

static void drawInfoScreen(void){
    drawText(10, 0, "Hangman (Dutch)", true, WHITETEXT);

    drawText(4, 9,  "Hangman, in the Netherlands known as", false, WHITETEXT);
    drawText(4, 12, "galgje, is a truely classic game with", false, WHITETEXT);
    drawText(4, 15, "extremely simple rules:", false, WHITETEXT);
    drawText(4, 18, "The computer will pick a random word", false, WHITETEXT);
    drawText(4, 24, "and it is your job to try to guess", false, WHITETEXT);
    drawText(4, 27, "that word. Choose a letter from the", false, WHITETEXT);
    drawText(4, 30, "alphabet and the computer will show", false, WHITETEXT);
    drawText(4, 33, "whether that letter exists in the", false, WHITETEXT);
    drawText(4, 36, "mystery word. If it does, you will ", false, WHITETEXT);
    drawText(4, 39, "also be shown where.", false, WHITETEXT);
    drawText(4, 42, "11 strikes and you're out!", false, WHITETEXT);

    drawText(30, 70, "Press Enter to continue", false, WHITETEXT);

    while (getKey() != keyEnter);
}

void hangman(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);
    drawInfoScreen();

    start:

    startGraphics(WHITEGFS);

    drawText(10, 0, "Hangman", true, WHITETEXT);

    unsigned char guessedList[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0};

    srand(getTime());
    unsigned char* secretWord = words[rand() % (sizeof(words) / sizeof(unsigned char*))];
    unsigned char wordLen = strlen(secretWord);

    unsigned char* baseAdr = vidmem + 401;
    unsigned char* adr = baseAdr;

    for (unsigned char i = 0; i < wordLen; i++)
        *adr++ = '_';

    drawText(4, 10, "Mystery word:", false, WHITETEXT);
    drawText(4, 24, "Your guess:", false, WHITETEXT);
    drawText(4, 45, "Not in word:", false, WHITETEXT);

    unsigned char guessed = false;
    unsigned char score = 0;

    while (score < 10 && !guessed){
        while (getKey() == keyEnter);

        unsigned char key = 0;
        unsigned char prevKey = 0;
        unsigned char newKey;

        while ((newKey = getNiceKeyL()) != '\n'){
            if (newKey == '\0') continue;
            if (newKey == '0') return;

            prevKey = key;
            key = newKey;

            drawChar(4, 30, key, false, WHITETEXT);
        }

        unsigned char invalidGuess = false;
        for (unsigned char i = 0; i < sizeof(guessedList); i++)
            if (guessedList[i] == key) invalidGuess = true;
        
        if (invalidGuess) continue;

        key = prevKey;

        *(baseAdr - 1) = WHITETEXT;

        adr = baseAdr;

        guessed = true;
        unsigned char scoreInc = true;

        for (unsigned char i = 0; i < wordLen; i++){
            if (secretWord[i] == key){
                *adr = key;
                scoreInc = false;
            }
            else
                if (*adr != secretWord[i]) guessed = false;

            adr++;
        }

        if (scoreInc){
            guessedList[score] = key;
            score++;
        }

        drawText(6, 48, guessedList, false, WHITETEXT);

        //sprintf(vidmem + 1840, "Score: %d", score);

        switch (score)
        {
        case 1: horzLine(75, 40, 65, true);  break;
        case 2: vertLine(40, 65, 20, true);  break;
        case 3: horzLine(40, 65, 20, true);  break;
        case 4: vertLine(65, 20, 26, true);  break;
        case 5: drawCircle(65, 30, 4, true); break;
        case 6: vertLine(65, 34, 42, true);  break;
        case 7: drawLine(65, 42, 61, 51, true);  break;
        case 8: drawLine(65, 42, 69, 51, true);  break;
        case 9: drawLine(65, 36, 60, 34, true);  break;
        case 10: drawLine(65, 36, 70, 34, true); break;
        }
    }

    fillRectangle(windowTLX, windowTLY, windowBRX, windowBRY, false);

    if(guessed)
        drawText(20, 10, "You won!", true, GREENTEXT);
    else
        drawText(20, 10, "You lost!", true, REDTEXT);

    drawText(4, 21, "Press the 0 key to exit", false, WHITETEXT);
    drawText(4, 24, "Press the spacebar to play again", false, WHITETEXT);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;

    return;
}