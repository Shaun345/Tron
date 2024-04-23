/* UART2.cpp
 * Your name
 * Data:
 * PA22 UART2 Rx from other microcontroller PA8 IR output<br>
 */


#include <ti/devices/msp/msp.h>
#include "UART2.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "FIFO2.h"

uint32_t LostData;
Queue FIFO2;

// power Domain PD0
// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 2375 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void){
// write this
    LostData = 0;

    UART2->GPRCM.RSTCTL = 0xB1000003;
    UART2->GPRCM.PWREN = 0x26000001;
    Clock_Delay(24); // time for uart to power up
    IOMUX->SECCFG.PINCM[PA22INDEX]  = 0x00040082;


    UART2->CLKSEL = 0x08; // bus clock
    UART2->CLKDIV = 0x00; // no divide
    UART2->CTL0 &= ~0x09; // disable UART0
    UART2->CTL0 = 0x00020018;
    // assumes an 80 MHz bus cloc
    UART2->IBRD = 1052;//   divider = 21+45/64 = 21.703125
    UART2->FBRD = 40; // baud =2,500,000/21.703125 = 115,191
    UART2->LCRH = 0x00000030;
    UART2->CPU_INT.IMASK = 0x0001;
    UART2->IFLS = 0x0422;
    NVIC->ICPR[0] = 1<<14; // UART0 is IRQ 15
    NVIC->ISER[0] = 1<<14;
    NVIC->IP[3] = (NVIC->IP[3]&(~0xFF000000))|(2<<22);    // priority (bits 31,30)
    UART2->CTL0 |= 0x09; // enable UART

}
//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void){
    char out;
    // write this
    if (FIFO2.IsEmpty())
          return 0;
    FIFO2.Get(&out);
    return out;
}

extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ 
// acknowledge, clear RTOUT
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
// read all data, putting in FIFO
    if(FIFO2.IsFull())
    {
        LostData++;
        return;
    }

    uint32_t RxCounter = 0;

    if(UART2->CPU_INT.IIDX == 1)
    {
        while(!(UART2->STAT&(1<<2)))
        {
            FIFO2.Put(UART2->RXDATA);
        }
    }

    RxCounter++;
// finish writing this
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
  
}
