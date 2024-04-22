// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 1/1/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "../inc/FIFO2.h"
//#include "UART2.h"
//#include "IRxmt.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "menu.h"
#include "Joystick2.h"
#include "GamePlay.h"
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void)
{ // set phase lock loop (PLL)
    // Clock_Init40MHz(); // run this line for 40MHz
    Clock_Init80MHz(0); // run this line for 80MHz
}

struct State
{
    void (*init_func)(void);
    void (*per_func)(int);
    bool (*isDone)(void);
    State *next[2];
};

#define menuInd &MainFSM[0]
#define gameStartInd &MainFSM[1]
#define gamePlayInd &MainFSM[2]
#define gameDoneInd &MainFSM[3]
#define replayInd &MainFSM[4]

State MainFSM[] = {
    {tronMenuInit, periodic_menu_update, menuIsDone, {gameStartInd, gameStartInd}},
    {gameInit, gameUpdate, gameFinished, {menuInd, gameStartInd}}};

State *currState = menuInd;

uint32_t M = 1;
uint32_t Random32(void)
{
    M = 1664525 * M + 1013904223;
    return M;
}
uint32_t Random(uint32_t n)
{
    return (Random32() >> 16) % n;
}

Joystick2 axes(1, 2);

int buttonInput = 0;
uint8_t joystickPacket = 0; // Up Down Left Right
int32_t xInput = 0;
int32_t yInput = 0;
// Change this to a FSM
short gameState = 0; // 0 is menu 1 is game 2 is win/loss 3 is replay screen
bool semaphore = 0;

#define threshold 65

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void)
{
    uint32_t pos, msg;
    if ((TIMG12->CPU_INT.IIDX) == 1)
    {                               // this will acknowledge
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
                                    // game engine goes here
        // 1) sample slide pot
        // Get raw data
        axes.Convert(&xInput, &yInput);
        // Convert to -100 to 100

        // 2) read input switches
        buttonInput = Switch_In();
        joystickPacket = ((yInput > threshold) << 3) | ((yInput < -threshold) << 2) | ((xInput > threshold) << 1) | ((xInput < -threshold));

        // 3) move sprites
        // 4) start sounds
        // 5) transmit UART data
        // IRxmt_OutChar(joystickPacket |= 0x80);
        // 6) set semaphore
        semaphore = true;
        // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void)
{
    return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void)
{ // final main
    __disable_irq();
    PLL_Init(); // set bus speed
    LaunchPad_Init();
    ST7735_InitPrintf();
    ST7735_SetRotation(1);
    // note: if you colors are weird, see different options for
    //  ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
    axes.JoyStick_Init();
    Switch_Init();                                   // initialize switches
    LED_Init();                                      // initialize LED
    Sound_Init();                                    // initialize sound
    //UART2_Init();                                    // initialize Receiving
    //IRxmt_Init();                                    // initialize Transmitting
    TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
                                                     // initialize interrupts on TimerG12 at 30 Hz
    TimerG12_IntArm(80000000 / 30, 2);
    SysTick_IntArm(7272, 3);
    // initialize all data structures
    __enable_irq();

    // Start the menu
    tronMenuInit();

    while (1)
    {
        // wait for semaphore
        if (semaphore)
        {
            // clear semaphore
            semaphore = false;

            // update ST7735R
            currState->per_func((joystickPacket << 8) | buttonInput);

            // check for end game or level switch
            if (currState->isDone())
            {
                currState = currState->next[0];
                currState->init_func();
            }
        }
    }
}
