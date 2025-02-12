#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "games.h"
#include "utils.h"
#include "keyboard.h"

unsigned char* words[1] = {
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

void hangman(void){
    setWindow(windowTLX, windowTLY, windowBRX, windowBRY);
    startGraphics(WHITEGFS);

    start:

    //drawInfoScreen();

    startGraphics(WHITEGFS);

    srand(getTime());
    unsigned char* secretWord = words[rand() % 500];
    unsigned char wordLen = strlen(secretWord);

    drawText(4, 10, secretWord, false, WHITETEXT);

    unsigned char guessed = false;
    unsigned char guessNum = 0;

    while ((++guessNum < 15) && !guessed){
        while (getKey() == keyEnter);

        unsigned char key = 0;
        unsigned char prevKey = 0;
        while (getKey() != keyEnter){
            if (getNiceKeyL() == 0xFF) continue;
            prevKey = key;
            key = getNiceKeyL();
        }

        key = prevKey;

        unsigned char* adr = vidmem + 1681;

        *(vidmem + 1680) = WHITETEXT;

        guessed = true;
        for (unsigned char i = 0; i < wordLen; i++){
            if (secretWord[i] == key)
                *adr = key;
            else
                if (*adr != secretWord[i]) guessed = false;

            adr++;
        }
    }

    if(guessed)
        drawText(20, 20, "You won!", true, GREENTEXT);
    else
        drawText(20, 20, "You lost!", false, REDTEXT);

    while (getKey() != key0 && getKey() != keySpace);

    if (getKey() == keySpace) goto start;

    return;
}