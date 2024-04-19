#include "menu.h"
#include "../inc/ST7735.h"
#include "images/images.h"
#include <string>
/*/
#define startgame   &men[0]
#define language    &men[1]
#define settings    &men[2]

#define sellang     &men[3]
#define langback    &men[4]

#define color       &men[5]
#define colchoose   &men[6]
#define speed       &men[7]
#define setback     &men[8]

#define begin       &men[9]

struct MenuFSM
{
    short menu;
    short select;
    void (*func)();
    MenuFSM* next[5];
    bool rerun;
};

MenuFSM men[] =
{
    //                                 none         down        up          select
    new MenuFSM{0, 0, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{0, 1, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{0, 2, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{1, 0, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{1, 0, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{1, 0, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{0, 0, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
    new MenuFSM{0, 0, &drawStartMenu, {startgame,   language,   settings,   begin, startgame}, false},
};*/

using namespace std;

// Offsets for text
int col = 16;
int row = 4;
int off = 2;

struct Menu
{
    string text[2][4];
    int mod;
    int highlight;
    int next[4];

    Menu(string text[2][4], int mod, int *next)
    {
        for (int val = 0; val < mod; val++)
        {
            for (int lang = 0; lang < 2; lang++)
            {
                this->text[lang][val] = text[lang][val];
            }
            this->next[val] = next[val];
        }

        this->mod = mod;
        this->highlight = 0;
    }
};

string mainText[2][4] = {{"Start", "Language", "Settings"},
                        {"Start", "Sprache", "Einstellungen"}};
int mainChange[] = {10, 1, 2};

string langText[2][4] = {{"English", "German", "Back"},
                        {"Englisch", "Deutsch", "Zuruck"}};
int langChange[] = {1, 1, 0};

string settText[2][4] = {{"Color:", "Speed:", "Toggle", "Back"},
                        {"Farbe:", "Speed:", "Umschalten", "Zuruck"}};
int settChange[] = {2, 2, 2, 0};

Menu menuFSM[] = {
    Menu(mainText, 3, mainChange),
    Menu(langText, 3, langChange),
    Menu(settText, 4, settChange)};

int currLang = 0;
int currInd = 0;


int speed = 1;
int tog = 0;

void up()
{
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65289);
    if (menuFSM[currInd].highlight == 0)
    {
        menuFSM[currInd].highlight = menuFSM[currInd].mod - 1;
    }
    else
    {
        menuFSM[currInd].highlight--;
    }
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65535);
}

void down()
{
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65289);
    menuFSM[currInd].highlight = (menuFSM[currInd].highlight + 1) % menuFSM[currInd].mod;
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65535);
}

void menu_update()
{
    ST7735_FillRect(89, 32, 67, 91, 0);

    for (int text = 0; text < menuFSM[currInd].mod; text++)
    {
        int color = menuFSM[currInd].highlight == text ? 65535 : 65289;
        ST7735_DrawString(col, row + off*text, menuFSM[currInd].text[currLang][text].c_str(), color);
    }
    if(currInd == 2)
    {
        ST7735_DrawString(col + 8, row + off, speed == 1 ? "1" : speed == 2 ? "2" : "3", 65289);
        ST7735_DrawString(col + 8, row + off + off, tog ? "T" : "F", 65289);
    }
}

void tronMenuInit()
{
    ST7735_DrawBitmap(0, 127, tron_menu_img, 160, 128);
    menu_update();
}

void changeSet()
{
    if(currInd == 1)
    {
        if(menuFSM[currInd].highlight == 0)
            currLang = 0;
        else if (menuFSM[currInd].highlight == 1)
            currLang = 1;
    }
    else if(currInd == 2)
    {
        switch (menuFSM[currInd].highlight)
        {
        case 0:
            break;
        case 1:
            speed = ++speed > 3 ? 1 : speed;
            break;
        case 2:
            tog = !tog;
            break;
        
        default:
            break;
        }
    }
}

void periodic_update(int input)
{
    switch (input)
    {
    case 0x02:
        up();
        break;

    case 0x01:
        down();
        break;

    case 0x10:
        changeSet();
        currInd = menuFSM[currInd].next[menuFSM[currInd].highlight];
        menu_update();

    default:
        break;
    }
}
