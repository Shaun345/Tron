/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/SlidePot.h"
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT 0x200

uint32_t SlidePot::In(void)
{
    ADC1->ULLMEM.CTL0 |= 0x000000001;
    ADC1->ULLMEM.CTL1 |= 0x000000100;
    uint32_t volatile delay = ADC1->ULLMEM.STATUS;
    while ((ADC1->ULLMEM.STATUS & 0x01) == 0x01)
    {
    };
    return ADC1->ULLMEM.MEMRES[index];
}

// constructor, invoked on creation of class
// m and b are linear calibration coefficents
SlidePot::SlidePot(uint32_t m, uint32_t b, uint32_t channel)
{
    slope = m;
    offset = b;
    this->channel = channel;
}

void SlidePot::Init(void)
{
    index = count;
    count++;

    ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003;
    ADC1->ULLMEM.GPRCM.PWREN = 0x26000001;
    Clock_Delay(24);
    ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000;
    ADC1->ULLMEM.CLKFREQ = 7;
    ADC1->ULLMEM.CTL0 = 0x03010000;
    ADC1->ULLMEM.CTL1 = 0x00000000;
    ADC1->ULLMEM.CTL2 = 0x00000000;
    ADC1->ULLMEM.MEMCTL[index] = channel;
    ADC1->ULLMEM.SCOMP0 = 0;
    ADC1->ULLMEM.CPU_INT.IMASK = 0;
}

void SlidePot::Save(uint32_t n)
{
    this->data = n;
    this->flag = 1;
}

uint32_t SlidePot::Convert(uint32_t n)
{
    long long position = ((slope * n) >> 12) - offset;
    position = position < 0 ? 0 : position;
    position = position > 10000 ? 0 : position;

    return (uint32_t)position;
}

uint32_t SlidePot::JoystickConvert(uint32_t n)
{
    long long position = ((slope * n) >> 12) - offset;
    position = position < -100 ? 100 : position;
    position = position > 100 ? 100 : position;

    return (uint32_t)position;
}
// do not use this function
// it is added just to show you how SLOW floating point in on a Cortex M0+
float SlidePot::FloatConvert(uint32_t input)
{
    return 0.00048828125 * input - 0.0001812345;
}

void SlidePot::Sync(void)
{
    while (!this->flag)
    {
    }
    this->flag = 0;
}

uint32_t SlidePot::Distance(void)
{ // return distance value (0 to 2000), 0.001cm
    return Convert(In());
}
