/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void)
{
    // Disable all seven buttons as outputs
    GPIOA->DOE31_0 &= ~((1<<24)|(1<<25)|(1<<26)|(1<<27)|(1<<28));
    GPIOB->DOE31_0 &= ~((1<<23)|(1<<24));

    // Set inputs to have input operation enabled, passive pull-up resistor, and software connected to pin
    IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00050081;
    IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00050081;
    IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00050081;
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00050081;
    IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00050081;
    IOMUX->SECCFG.PINCM[PB23INDEX] = 0x00050081;
    //IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00050081;
  
}
// return current state of switches
uint32_t Switch_In(void)
{
    // Order of bits from most to least significant
    // Joystick, Right, Down, Left, Up, Select, Start
    unsigned int bIn = 0x03 & (GPIOB->DIN31_0 >> 23);
    unsigned int aIn = 0x3C & (GPIOA->DIN31_0 >> 22);

    return aIn | bIn;
}
