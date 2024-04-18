// Dump.c
// Your solution to ECE319K Lab 3 Spring 2024
// Author: Shaun Kurian
// Last Modified: 2.12.2024


#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50
uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to MAXBUF (0 is empty, MAXBUF is full)

// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){
// students write this for Lab 3
// This function should also initialize Timer G12, call TimerG12_Init.
    TimerG12_Init();
    DebugCnt = 0;
}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.

    // Fail if buffers are full
    if (DebugCnt >= MAXBUF) { return 0; }

    DataBuffer[DebugCnt] = data;
    TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;

    DebugCnt++;

    return 1; // success
}
// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)
uint32_t Debug_Dump2(uint32_t data){
// optional for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.

  return 1; // success
}
// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
    uint32_t flag = 0;
    uint32_t avg = 0;
    int32_t lastIndex = -1;
    for (uint32_t index = 1; index < DebugCnt; index++)
    {
        // Check if this data is a rising edge
        if (((DataBuffer[index] & mask) != 0) && ((DataBuffer[index - 1] & mask) == 0))
        {
            flag++;

            // Add to average total if this is not the first rising edge
            if (lastIndex != -1)
            {
                // Subtract in this order since timer counts down
                avg += TimeBuffer[lastIndex] - TimeBuffer[index];
            }

            // Set the last known rising edge to current rising edge
            lastIndex = index;
        }
    }

    // Return zero if no more than 1 rising edge, else return average value
    return (flag < 2) ? 0 : (avg / (flag - 1));
}


// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask){
// optional for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
    uint32_t flag = 0;
    uint32_t avg = 0;
    int32_t lastIndex = -1;


    for (uint32_t index = 1; index < DebugCnt; index++)
    {
        // Check if this data is a rising edge
        if (((DataBuffer[index] & mask) != 0) && ((DataBuffer[index - 1] & mask) == 0))
        {
            flag++;
            lastIndex = index;
        }
        else if ((((DataBuffer[index] & mask) == 0) && ((DataBuffer[index - 1] & mask) != 0)) && (flag % 2 == 1))
        {
            // Add to average total if this is not the first rising edge
            if (lastIndex != -1)
            {
                // Subtract in this order since timer counts down
                avg += TimeBuffer[lastIndex] - TimeBuffer[index];
                flag++;
            }
        }
    }

    // Return zero if no more than 2 rising edge, else return average value
    return (flag < 3) ? 0 : (uint32_t) (0.5 + (100.0 * ((double) avg / (((double) flag)/ 2.0)) / (double) Debug_Period(mask)));

}

// Lab2 specific debugging code
uint32_t Theperiod;
uint32_t Theduty;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
  if(result == 0){ // 0 means full
    Theperiod = Debug_Period(1<<18);        // calls your Lab3 function
    Theduty   = Debug_Duty(1<<18);
   __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}




