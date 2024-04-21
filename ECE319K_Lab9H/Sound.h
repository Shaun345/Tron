// Sound.h
// Runs on MSPM0
// Play sounds on 5-bit DAC.
// Your name
// 11/5/2023
#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

void SysTick_IntArm(uint32_t, uint32_t);
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
// This is called once
void Sound_Init(void);

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
void Sound_Start(const uint8_t *pt, uint32_t count);

void Sound_Stop(void);

void Sound_Menu(void);
void Sound_Select(void);
void Sound_Start(void);
void Sound_Success(void);
void Sound_Fail(void);
void Sound_GameWin(void);
void Sound_GameLose(void);

#endif
