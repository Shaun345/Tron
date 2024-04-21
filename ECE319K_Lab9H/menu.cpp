#include "menu.h"
#include "../inc/ST7735.h"
#include "images/images.h"
#include <string>
#include <map>
#include "Sound.h"

using namespace std;

#include "GameSettings.h"
extern int bikeSpeed;
extern bool abilityEnabled;
extern int currLang;

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

    Menu(string text[2][4], int mod, const int *next)
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
                         {"Start", "Sprache", "Aufbau"}};
int mainChange[] = {10, 1, 2};

string langText[2][4] = {{"English", "German", "Back"},
                         {"Englisch", "Deutsch", "Zuruck"}};
int langChange[] = {0, 0, 0};

string settText[2][4] = {{"Color:", "Speed:", "Special:", "Back"},
                         {"Farbe:", "Tempo:", "Spezial:", "Zuruck"}};
int settChange[] = {2, 2, 2, 0};

Menu menuFSM[] = {
Menu(mainText, 3, mainChange),
Menu(langText, 3, langChange),
Menu(settText, 4, settChange)};

int lasInd = 1;
int currInd = 0;

void up()
{
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65289);
    if (menuFSM[currInd].highlight == 0)
        menuFSM[currInd].highlight = menuFSM[currInd].mod - 1;
    else
        menuFSM[currInd].highlight--;
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65535);
}

void down()
{
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65289);
    menuFSM[currInd].highlight = (menuFSM[currInd].highlight + 1) % menuFSM[currInd].mod;
    ST7735_DrawString(col, row + off * menuFSM[currInd].highlight, menuFSM[currInd].text[currLang][menuFSM[currInd].highlight].c_str(), 65535);
}

const char* speeds[] = {"1", "2", "3"};

void menu_update()
{
    if (lasInd != currInd)
        ST7735_FillRect(89, 32, 67, 91, 0);

    for (int text = 0; text < menuFSM[currInd].mod; text++)
    {
        int color = menuFSM[currInd].highlight == text ? 65535 : 65289;
        ST7735_DrawString(col, row + off * text, menuFSM[currInd].text[currLang][text].c_str(), color);
    }
    if (currInd == 2)
    {
        ST7735_DrawString(col + 8, row + off, speeds[bikeSpeed - 1], 65289);
        ST7735_DrawString(col + 8, row + off + off, abilityEnabled ? "T" : "F", 65289);
    }
}

void tronMenuInit()
{
    ST7735_DrawBitmap(0, 127, tron_menu_img, 160, 128);
    menu_update();
}

void changeSet()
{
    if (currInd == 1)
    {
        if (menuFSM[currInd].highlight == 0)
            currLang = 0;
        else if (menuFSM[currInd].highlight == 1)
            currLang = 1;
    }
    else if (currInd == 2)
    {
        switch (menuFSM[currInd].highlight)
        {
        case 0:
            break;
        case 1:
            bikeSpeed = ++bikeSpeed > 3 ? 1 : bikeSpeed;
            break;
        case 2:
            abilityEnabled = !abilityEnabled;
            break;

        default:
            break;
        }
    }
}

int lastInput = 0;
void periodic_update(int input)
{
    if (input != lastInput)
    {
        lastInput = input;

        switch (input)
        {
        case 0x0800:
        case 0x0004:
            up();
            break;

        case 0x0400:
        case 0x0010:
            down();
            break;

        case 0x0020:
            changeSet();
            lasInd = currInd;
            currInd = menuFSM[currInd].next[menuFSM[currInd].highlight];
            menu_update();

        default:
            break;
        }

        switch (input)
        {
            case 0x0800:
            case 0x0004:
            case 0x0400:
            case 0x0010:
                Sound_Menu();
                break;

            case 0x0020:
                Sound_Select();
                break;

            default:
                break;
        }

    }
}
