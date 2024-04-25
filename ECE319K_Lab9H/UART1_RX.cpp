/* UART1.c
 * Your name
 * Data:
 * PA22 UART1 Rx from other microcontroller PA8 UART1 Tx<br>
 */

#include <ti/devices/msp/msp.h>
#include "UART1_RX.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/FIFO2.h"

uint32_t LostData;

Queue FIFO;
// power Domain PD0
// for 80MHz bus clock, UART clock is ULPCLK 40MHz
// initialize UART1 for 2000 baud rate
// no transmit, interrupt on receive timeout, RTOUT
void UART1_Init(void)
{
    UART1->GPRCM.RSTCTL = 0xB1000003;
    UART1->GPRCM.PWREN = 0x26000001;
    Clock_Delay(24); // time for uart to power up
    IOMUX->SECCFG.PINCM[PA18INDEX] = 0x00040082;
    UART1->CLKSEL = 0x08; // bus clock
    UART1->CLKDIV = 0x00; // no divide
    UART1->CTL0 &= ~0x01; // disable UART0
    UART1->CTL0 = 0x00020018;
    // assumes an 80 MHz bus clock
    UART1->IBRD = 800;
    UART1->FBRD = 0; // baud =2,500,000/1250 = 2000
    UART1->LCRH = 0x00000030;
    UART1->CPU_INT.IMASK = 0x0001;
    // bit 11 TXINT
    // bit 10 RXINT
    // bit 0  Receive timeout
    UART1->IFLS = 0x0422;
    // bits 11-8 RXTOSEL receiver timeout select 4 (0xF highest)
    // bits 6-4  RXIFLSEL 2 is greater than or equal to half
    // bits 2-0  TXIFLSEL 2 is less than or equal to half
    NVIC->ICPR[0] = 1 << 14; // UART0 is IRQ 15
    NVIC->ISER[0] = 1 << 14;
    NVIC->IP[3] = (NVIC->IP[3] & (~0xFF000000)) | (2 << 22); // priority (bits 31,30)
    UART1->CTL0 |= 0x01;                                     // enable UART0
}
//------------UART1_InChar------------
// Get new serial port receive data from FIFO1
// Input: none
// Output: Return 0 if the FIFO1 is empty
//         Return nonzero data from the FIFO1 if available
char UART1_InChar(void)
{
    //    while((UART1->STAT&0x04) == 0x04){}; // wait while not input
    //    return((char)(UART1->RXDATA));
    if(FIFO.IsEmpty())
        return 0;
    
    char data;
    FIFO.Get(&data);
    return data;
}

void static copyHardwareToSoftware(void)
{
    char letter;
    while (((UART1->STAT & 0x04) == 0))
    {
        letter = UART1->RXDATA;
        FIFO.Put(letter);
    }
}

extern "C" void UART1_IRQHandler(void);
void UART1_IRQHandler(void)
{
    uint32_t status;
    char letter;
    // acknowledge RTOUT
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)

    status = UART1->CPU_INT.IIDX; // reading clears bit in RIS
    copyHardwareToSoftware();
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    // read all data and put in FIFO1
    if (FIFO.IsFull())
    {
        LostData++;
        return;
    }

    uint32_t RxCounter = 0;

    if (UART1->CPU_INT.IIDX == 1)
    {
        while (!(UART1->STAT & (1 << 2)))
        {
            FIFO.Put(UART1->RXDATA);
        }
    }

    RxCounter++;
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
}
