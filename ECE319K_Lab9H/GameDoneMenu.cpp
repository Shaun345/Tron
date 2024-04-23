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
    bool ready = false;
    ST7735_FillScreen(ST7735_BLACK);
    if(gameWinner() == !host)
        ST7735_DrawBitmap(15, 75, YouWin, 130, 30);
    else
        ST7735_DrawBitmap(10, 80, YouLose, 139, 42);
}

void updateScreen(int input)
{
    if(!lastIn && (input & 0x000F))
        ready = true;
    
    lastIn = input;
}

bool readyForMenu(void)
{
    return ready;
}
