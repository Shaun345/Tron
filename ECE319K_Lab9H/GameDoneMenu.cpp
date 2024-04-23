#include <ti/devices/msp/msp.h>
#include "GameDoneMenu.h"
#include "GameSettings.h"
#include "images/images.h"
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "GamePlay.h"

bool ready = false;
bool lastIn = 0xFFFF;

void printWinLose(void)
{
    ready = false;
    ST7735_FillScreen(ST7735_BLACK);
    if(gameWinner() == !host)
        ST7735_DrawBitmap(15, 75, currLang == ENGLISH ? YouWin : YouWinGerman, 130, 30);
    else
        ST7735_DrawBitmap(10, 80, currLang == ENGLISH ? YouLose : YouLoseGerman, 139, 42);
}

void updateScreen(int input)
{
    if(!lastIn && (input & 0x00FF) != 0)
        ready = true;
    
    lastIn = input;
}

bool readyForMenu(void)
{
    return ready;
}
