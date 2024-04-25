#include <ti/devices/msp/msp.h>
#include "UART0_TX.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "FIFO2.h"

void UART0_Init(void)
{
    UART0->GPRCM.RSTCTL = 0xB1000003;
    UART0->GPRCM.PWREN = 0x26000001;
    Clock_Delay(24); // time for uart to power up
    IOMUX->SECCFG.PINCM[PA10INDEX] = 0x00000082;


    UART0->CLKSEL = 0x08; // bus clock
    UART0->CLKDIV = 0x00; // no divide
    UART0->CTL0 &= ~0x01; // disable UART0
    UART0->CTL0 = 0x00020018;

    UART0->IBRD = 800;
    UART0->FBRD = 0; // baud =2,500,000/1250 = 2000
    UART0->LCRH = 0x00000030;
    UART0->CTL0 |= 0x01; // enable UART0
}

/**
 * Output 8-bit to serial port
 * @param data is an 8-bit ASCII character to be transferred
 * @return none
 * @brief output character to UART0
 */
void UART0_OutChar(char data)
{
    UART0->TXDATA = data;
}
