// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

#define Kz11 0

uint32_t period = Kz11;
uint32_t priority = 0;

uint32_t size = 0;
uint32_t ind = 0;
const unsigned char *curSound;

void SysTick_IntArm(uint32_t periodIn, uint32_t priorityIn)
{
    // write this
    period = periodIn;
    priority = priorityIn;
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void)
{
    DAC5_Init();

    SysTick->CTRL &= 0;
    SysTick->LOAD = period - 1;
    SCB->SHP[1] = (SCB->SHP[1] & (~0xC0000000)) | priority << 30;
    SysTick->VAL = 0;
    SysTick->CTRL |= 0xFFFFFFF7;
}
extern "C" void SysTick_Handler(void);
void SysTick_Handler(void)
{ // called at 11 kHz
    // output one value to DAC if a sound is active
    if (ind < size)
    {
        DAC5_Out(curSound[ind]);
        ind++;
    }
    else
        Sound_Stop();
}

//******* Sound_Start ************
// This function does not output to the DAC.
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count)
{
    ind = 0;
    curSound = pt;
    size = count;
    SysTick->LOAD = period - 1;
    SysTick->VAL = 0;
}

void Sound_Stop(void)
{
    SysTick->LOAD = 0;
}

void Sound_Menu(void)
{
    Sound_Start(MenuSound, sizeof(MenuSound));
}

void Sound_Explosion(void)
{
    Sound_Start(explosion, sizeof(explosion));
}

void Sound_Select(void)
{
    Sound_Start(TronMenuSelect, sizeof(TronMenuSelect));
}

void Sound_Start(void)
{
    Sound_Start(TronStart, sizeof(TronStart));
}

void Sound_Success(void)
{
    Sound_Start(TronSuccess, sizeof(TronSuccess));
}

void Sound_Fail(void)
{
    Sound_Start(TronGameFail, sizeof(TronGameFail));
}

void Sound_GameWin(void)
{
    Sound_Start(TronGameWin, sizeof(TronGameWin));
}

void Sound_GameLose(void)
{
    Sound_Start(TronGameOver, sizeof(TronGameOver));
}
