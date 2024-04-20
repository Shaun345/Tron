/*
 * LED.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

// initialize your LEDs
void LED_Init(void)
{
    // Initialize the IR LED as an output
    GPIOA->DOE31_0 |= (1 << 8);
    // Initialize the 3 color LEDS as output
    GPIOB->DOE31_0 |= (1 << 16) | (1 << 17) | (1 << 19);

    // regular output
    IOMUX->SECCFG.PINCM[PA8INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00000081;
}
// data specifies which LED to turn on
// Bit 0 Green LED
// Bit 1 Yellow LED
// Bit 2 Red LED
// Bit 3 IR LED
void LED_On(uint32_t data)
{
    GPIOA->DOUTSET31_0 = (0x03 & data) << 16 | (0x04 & data) << 19;
    GPIOB->DOUTSET31_0 = (0x08 & data) << 8;
}

// data specifies which LED to turn off
void LED_Off(uint32_t data)
{
    GPIOA->DOUTCLR31_0 = (0x03 & data) << 16 | (0x04 & data) << 19;
    GPIOB->DOUTCLR31_0 = (0x08 & data) << 8;
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data)
{
    GPIOA->DOUTTGL31_0 = (0x03 & data) << 16 | (0x04 & data) << 19;
    GPIOB->DOUTTGL31_0 = (0x08 & data) << 8;
}
