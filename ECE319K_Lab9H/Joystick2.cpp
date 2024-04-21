/* JoyStick.c
 * MKII BoosterPack
 * Jonathan Valvano
 * November 21, 2022
 * Derived from gpio_toggle_output_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_vref_internal_LP_MSPM0G3507_nortos_ticlang
 *              adc12_single_conversion_LP_MSPM0G3507_nortos_ticlang
 */
#include <ti/devices/msp/msp.h>
#include "Joystick2.h"
#include "../inc/Clock.h"
#include "../inc/ADC.h"
// Analog MKII  Joystick
// J1.5 joystick Select button (digital) PA26
// J1.2 joystick horizontal (X) (analog) PA25_ADC0.2
// J3.26 joystick vertical (Y) (analog)  PA18_ADC1.3
// remove J1 J2 J3 on LaunchPad to use PA26

// user buttons
// J4.33 user Button1 (bottom) (digital) PA12
// J4.32 user Button2 (top) (digital)    PA11 (conflicts with UART0 Rx from XDS Tx)

#define PA26INDEX 58
#define PA11INDEX 21
#define PA12INDEX 33

Joystick2::Joystick2(uint32_t channel1, uint32_t channel2)
{
    this->channel1 = channel1;
    this->channel2 = channel2;
}

// Initialize MKII JoyStick and JoyStick button
void Joystick2::JoyStick_Init()
{
    ADC1->ULLMEM.GPRCM.RSTCTL = (uint32_t)0xB1000003;
    ADC1->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;
    Clock_Delay(24);                        // time for ADC and VREF to power up
    ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
    ADC1->ULLMEM.CLKFREQ = 7;               // 40 to 48 MHz
    ADC1->ULLMEM.CTL0 = 0x03010000;
    ADC1->ULLMEM.CTL1 = 0x00010000;
    ADC1->ULLMEM.CTL2 = 0x02010000;
    ADC1->ULLMEM.MEMCTL[1] = channel1;
    ADC1->ULLMEM.MEMCTL[2] = channel2;
    ADC1->ULLMEM.SCOMP0 = 0;          // 8 sample clocks
    ADC1->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
}

// Read JoyStick position
// Inputs: *x pointer to empty 32 bit unsigned variable
//         *y pointer to empty 32 bit unsigned variable
// Output: none
void Joystick2::JoyStick_In(uint32_t *x, uint32_t *y)
{
    ADC1->ULLMEM.CTL0 |= 0x00000001;               // enable conversions
    ADC1->ULLMEM.CTL1 |= 0x00000100;               // start ADC
    uint32_t volatile delay = ADC1->ULLMEM.STATUS; // time to let ADC start
    while ((ADC1->ULLMEM.STATUS & 0x01) == 0x01)
    {
    }; // wait for completion
    *x = ADC1->ULLMEM.MEMRES[1];
    *y = ADC1->ULLMEM.MEMRES[2];
}

void Joystick2::Convert(int32_t *x, int32_t *y)
{
    ADC1->ULLMEM.CTL0 |= 0x00000001;               // enable conversions
    ADC1->ULLMEM.CTL1 |= 0x00000100;               // start ADC
    uint32_t volatile delay = ADC1->ULLMEM.STATUS; // time to let ADC start

    while ((ADC1->ULLMEM.STATUS & 0x01) == 0x01)
    {
    }; // wait for completion

    *x = 100 - ((int32_t) (200 * ADC1->ULLMEM.MEMRES[1]) >> 12);
    *y = ((int32_t) (200 * ADC1->ULLMEM.MEMRES[2]) >> 12) - 100;
}
