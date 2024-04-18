//****************** NoteGate.s ***************
// Assembly project implementing a not gate
// Author: Jonathan Valvano
// Last Modified: March 12, 2023
// Brief description of the program:
// This is a simple C language project that creates a not gate
// If SW is pressed, the LED is off.
// If SW is not pressed, the LED is on.
       .data
       .align 2
// Declare global variables here if needed
// with the .space assembly directive
      .equ   SIZE,32
Buf:  .space SIZE
DBuf: .space SIZE
TBuf: .space SIZE*2
Cnt:  .space 4


// Instructions go in FLASH ROM
       .text
       .thumb
       .align 2
// I/O port addresses
   .equ GPIOA_RSTCTL,   0x400A0804
   .equ GPIOA_PWREN,    0x400A0800
   .equ GPIOA_DOE31_0,  0x400A12C0
   .equ GPIOA_DOUT31_0, 0x400A1280
   .equ GPIOA_DIN31_0,  0x400A1380
   .equ GPIOA_DOUTSET31_0, 0x400A1290
   .equ GPIOA_DOUTCLR31_0, 0x400A12A0
   .equ GPIOA_DOUTTGL31_0, 0x400A12B0
   .equ GPIOB_RSTCTL,   0x400A2804
   .equ GPIOB_PWREN,    0x400A2800
   .equ GPIOB_DOE31_0,  0x400A32C0
   .equ GPIOB_DOUT31_0, 0x400A3280
   .equ GPIOB_DIN31_0,  0x400A3380
   .equ GPIOB_DOUTSET31_0, 0x400A3290
   .equ GPIOB_DOUTCLR31_0, 0x400A32A0
   .equ GPIOB_DOUTTGL31_0, 0x400A32B0
   .equ IOMUXPA0,       0x40428004+4*0
   .equ IOMUXPA18,      0x40428004+4*39
   .equ IOMUXPB21,      0x40428004+4*48
   .equ IOMUXPB22,      0x40428004+4*49
   .equ IOMUXPB26,      0x40428004+4*56
   .equ IOMUXPB27,      0x40428004+4*57
   .equ IOMUXPB0,       0x40428004+4*11
   .equ IOMUXPB1,       0x40428004+4*12
   .equ IOMUXPB2,       0x40428004+4*14
   .equ IOMUXPB3,       0x40428004+4*15

       .global main
main:
//1) Initialize input pin and output pin
   BL Init
// *********test code************
//   MOVS R5,#8
//   LDR R4,=GPIOB_DOUTTGL31_0
//loop8:
//   STR R5,[R4]
//   LDR R0,=16000000 // 1sec
//   BL  Delay
//   B   loop8
// ***************************
  LDR R6,=GPIOB_DIN31_0
  LDR R7,=GPIOB_DOUT31_0
  MOVS R5,#1 // bit 0
  MOVS R4,#2 // bit 1
loop12:
  LDR  R1,[R6]  // all of GPIOB
  ANDS R1,R1,R5 // PB0
  EORS R1,R1,R5 // flip bit 0
  LSLS R1,R1,#1 //
  LDR  R2,[R7]  // 1) read
  BICS R2,R2,R4 // 2) clear
  ORRS R1,R1,R2 // 3) combine
  STR  R1,[R7]  // 4) write
  B    loop12


// make PB2 input, PB3 output
InitNot:
  PUSH {LR}
//  Reset, activate, wait, PINCM, DOE
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOB_RSTCTL
  STR  R1,[R0]  // reset PortB
  LDR  R1,=0x26000001
  LDR  R0,=GPIOB_PWREN
  STR  R1,[R0]  // power PortB
  MOVS R0,#24
  BL   Delay    // stabilize
  MOVS R1,#0x81
  LDR  R0,=IOMUXPB3
  STR  R1,[R0]  // PB3 is out
  LDR  R1,=0x00040081
  LDR  R0,=IOMUXPB2
  STR  R1,[R0]  // PB2 is in
  LDR  R0,=GPIOB_DOE31_0
  MOVS R1,#0x08 // PB3
  STR  R1,[R0]  // enable out
  POP {PC}      // return

// Example 1.10.2, Program 1.10.2 NOT gate
main9:
   BL   Init // make PB1 output, PB0 input
   LDR  R6,=GPIOB_DIN31_0
   LDR  R7,=GPIOB_DOUT31_0
   MOVS R5,#1
loop:
   LDR  R1,[R6]  // contents of GPIOB
   ANDS R1,R1,R5 // PB0
   EORS R1,R1,R5 // flip bit 0
   LSLS R1,R1,#1 //
   STR  R1,[R7]  // output to PB1
   B    loop
   .align 2
// turn on PortB
// make PB0 input, PB1 output
// turn on PortB, PB1 output
Init:  // PB0 input
  PUSH {LR}
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOB_RSTCTL
  STR  R1,[R0]  // reset PortB
  LDR  R1,=0x26000001
  LDR  R0,=GPIOB_PWREN
  STR  R1,[R0]  // power PortB
  MOVS R0,#24
  BL   Delay    // stabilize
  MOVS R1,#0x81
  LDR  R0,=IOMUXPB1
  STR  R1,[R0]  // PB1 is out
  LDR  R1,=0x00040081
  LDR  R0,=IOMUXPB0
  STR  R1,[R0]  // PB0 is in
  LDR  R0,=GPIOB_DOE31_0
  MOVS R1,#0x02 // PB1
  STR  R1,[R0]  // enable out
  POP  {PC}


// Example 1.10.1, Program 1.10.1
// Toggles red LED
main0:
   BL   InitA // make PA0 output
   MOVS R6,#1
   LDR  R7,=GPIOA_DOUT31_0
loopA:
   LDR  R1,[R7]  // contents of GPIOA
   EORS R1,R1,R6 // flip bit 0
   STR  R1,[R7]  // output to PA0
   LDR  R0,=16000000 // 500ms
   BL   Delay
   B    loopA
   .align 2
// turn on PortA
// make PA0 output
InitA:
   PUSH {LR}
   LDR  R1,=0xB1000003
   LDR  R0,=GPIOA_RSTCTL
   STR  R1,[R0]  // reset PortA
   LDR  R1,=0x26000001
   LDR  R0,=GPIOA_PWREN
   STR  R1,[R0]  // power PortA
   MOVS R0,#24
   BL   Delay    // stabilize
   MOVS R1,#0x81
   LDR  R0,=IOMUXPA0
   STR  R1,[R0]  // PA0 is out
   LDR  R0,=GPIOA_DOE31_0
   MOVS R1,#0x01 // PA0
   STR  R1,[R0]  // enable out
   POP  {PC}

Switch_Init:  // PB3 input
  PUSH {LR}
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOB_RSTCTL
  STR  R1,[R0]  // reset PortB
  LDR  R1,=0x26000001
  LDR  R0,=GPIOB_PWREN
  STR  R1,[R0]  // power PortB
  MOVS R0,#24
  BL   Delay    // stabilize
  LDR  R1,=0x00040081
  LDR  R0,=IOMUXPB3
  STR  R1,[R0]  // PB3 is in
  POP  {PC}
Switch_Input:  // read PB3 input
  LDR  R1,=GPIOB_DIN31_0
  MOVS R3,#0x08
  LDR  R0,[R1]  // contents of GPIOB
  ANDS R0,R0,R3
  BX   LR
mainB:
   BL   Switch_Init // make PB3 input
loopB:
   BL   Switch_Input //read PB3
   MOVS R4,R0
   B    loopB

// Program 3.2.9
LED_Init:  // PB1 output
  PUSH {LR}
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOB_RSTCTL
  STR  R1,[R0]  // reset PortB
  LDR  R1,=0x26000001
  LDR  R0,=GPIOB_PWREN
  STR  R1,[R0]  // power PortB
  MOVS R0,#24
  BL   Delay    // stabilize
  LDR  R1,=0x00100081
  LDR  R0,=IOMUXPB1
  STR  R1,[R0]  // PB1 is out
  LDR  R0,=GPIOB_DOE31_0
  LDR  R1,[R0]
  MOVS R2,#0x02 // PB1
  ORRS R1,R1,R2
  STR  R1,[R0]  // enable out
  POP  {PC}
LED_Off:  // PB1=0
  LDR  R1,=GPIOB_DOUTCLR31_0
  MOVS R0,#0x02
  STR  R0,[R1]
  BX   LR
LED_On:  // PB1=1
  LDR  R1,=GPIOB_DOUTSET31_0
  MOVS R0,#0x02
  STR  R0,[R1]
  BX   LR
LED_Toggle:  // flip PB1
  LDR  R1,=GPIOB_DOUTTGL31_0
  MOVS R0,#0x02
  STR  R0,[R1]
  BX   LR
mainC:
   BL   LED_Init // make PB1 output
   BL   LED_On   // PB1=1
   BL   LED_Off  // PB1=0
loopC:
   BL   LED_Toggle // toggle
   LDR  R0,=16000000 // 500ms
   BL   Delay
   B    loopC

 .macro LEDOff  // PB1=0
  LDR  R1,=GPIOB_DOUTCLR31_0
  MOVS R0,#0x02
  STR  R0,[R1]
 .endm
 .macro LEDOn  // PB1=1
  LDR  R1,=GPIOB_DOUTSET31_0
  MOVS R0,#0x02
  STR  R0,[R1]
 .endm
 .macro LEDToggle  // flip PB1
  LDR  R1,=GPIOB_DOUTTGL31_0
  MOVS R0,#0x02
  STR  R0,[R1]
 .endm
mainC1:
   BL   LED_Init // make PB1 output
   LEDOn   // PB1=1
   LEDOff  // PB1=0
loopC1:
   LEDToggle // toggle
   LDR  R0,=16000000 // 500ms
   BL   Delay
   B    loopC1
   // Program 3.2.10
mainD:  // PB1, PB0 outputs
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOB_RSTCTL
  STR  R1,[R0]  // reset PortB
  LDR  R1,=0x26000001
  LDR  R0,=GPIOB_PWREN
  STR  R1,[R0]  // power PortB
  MOVS R0,#24
  BL   Delay    // stabilize
  LDR  R1,=0x00100081
  LDR  R0,=IOMUXPB0
  STR  R1,[R0]  // PB0 is out
  LDR  R0,=IOMUXPB1
  STR  R1,[R0]  // PB1 is out
  LDR  R0,=GPIOB_DOE31_0
  LDR  R1,[R0]
  MOVS R2,#0x03 // PB1,PB0
  ORRS R1,R1,R2
  STR  R1,[R0]  // enable out
  LDR  R1,=GPIOB_DOUTCLR31_0
  MOVS R0,#0x01
  STR  R0,[R1]
  LDR  R1,=GPIOB_DOUTSET31_0
  MOVS R0,#0x02
  STR  R0,[R1]
  LDR  R1,=GPIOB_DOUTTGL31_0
  MOVS R0,#0x03
loopD:
  STR  R0,[R1]
  B    loopD


   // Program 3.2.11
StepperInit:  // PB3-PB0 outputs
  PUSH {LR}
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOB_RSTCTL
  STR  R1,[R0]  // reset PortB
  LDR  R1,=0x26000001
  LDR  R0,=GPIOB_PWREN
  STR  R1,[R0]  // power PortB
  MOVS R0,#24
  BL   Delay    // stabilize
  LDR  R1,=0x00100081
  LDR  R0,=IOMUXPB0
  STR  R1,[R0]  // PB0 is out
  LDR  R0,=IOMUXPB1
  STR  R1,[R0]  // PB1 is out
  LDR  R0,=IOMUXPB2
  STR  R1,[R0]  // PB2 is out
  LDR  R0,=IOMUXPB3
  STR  R1,[R0]  // PB3 is out
  LDR  R0,=GPIOB_DOE31_0
  LDR  R1,[R0]
  MOVS R2,#0x0F // PB3-PB0
  ORRS R1,R1,R2
  STR  R1,[R0]  // enable out
  LDR  R1,=GPIOB_DOUTCLR31_0
  MOVS R0,#0x06
  STR  R0,[R1]
  LDR  R1,=GPIOB_DOUTSET31_0
  MOVS R0,#0x09
  STR  R0,[R1]
  POP  {PC}
StepperToggle:
  LDR  R1,=GPIOB_DOUTTGL31_0
  STR  R0,[R1]
  BX   LR
mainE:
  BL   StepperInit
loopE:
  MOVS R0,#0x0C
  BL   StepperToggle
  LDR  R0,=665600 // 665,600*31.25ns=20.8ms
  BL   Delay
  MOVS R0,#0x03
  BL   StepperToggle
  LDR  R0,=665600 // 20.8ms
  BL   Delay
  B    loopE

// Program 2.4.1
mainF: BL   LED_Init // make PB1 output
      LDR  R0,=2240000 // 2240000*31.25ns=7ms
      LDR  R1,=96000 // 96000*31.25ns = 3ms
      BL   PWM
done: B    done
// R0 is H, time LED is on
// R1 is L, time LED is off
// H+L should be constant
PWM:  MOV  R4,R0 // copy of H
      MOV  R5,R1 // copy of L
LoopF: BL   LED_On
      MOV  R0,R4 // H
      BL   Delay
      BL   LED_Off
      MOV  R0,R5 // L
      BL   Delay
      B    LoopF

   // profiling udiv32_16
   .equ SysTick_CTRL,  0xE000E010
   .equ SysTick_LOAD,  0xE000E014
   .equ SysTick_VAL,   0xE000E018
SysTickInit2:
    LDR  R2, =SysTick_CTRL
    MOVS R0, #0
    STR  R0, [R2]   // disable SysTick during setup
    LDR  R1, =SysTick_LOAD
    LDR  R0,=0xFFFFFF
    STR  R0, [R1]
    LDR  R1, =SysTick_VAL
    MOVS R0, #0
    STR  R0, [R1]    // any write to VAL clears it
    MOVS R0, #0x05
    STR  R0, [R2]    // ENABLE and CLK_SRC bits set
    BX   LR
// unsigned 32-bit dividend by 16-bit
// Inputs: R0 is 32-bit dividend
//         R1 is 16-bit divisor
// quotient*divisor + remainder = dividend
// Output: R0=quotient
//         R1=remainder
udiv32_16:
   PUSH {R4}
   LDR  R4,=0x00010000
   MOVS R3,#0  // quotient
   MOVS R2,#16 // loop counter
   LSLS R1,#15 // move divisor under dividend
udiv32_16_loop:
   LSRS R4,R4,#1 // bit mask 15 to 0
   CMP  R0,R1    // need to subtract?
   BLO  udiv32_16_next
   SUBS R0,R0,R1 // subtract divisor
   ORRS R3,R3,R4 // set bit
udiv32_16_next:
   LSRS R1,R1,#1
   SUBS R2,R2,#1
   BNE  udiv32_16_loop
   MOVS R1,R0  // remainder
   MOVS R0,R3  // quotient
   POP  {R4}
   BX   LR
       .global main
mainI2:
      MOVS R0,#0
      BL   Clock_Init80MHz
      BL   SysTickInit2
      LDR  R4,=SysTick_VAL
      LDR  R5,[R4]  // start
      LDR  R0,=0xFFFF // 175 bus cycles worst case 65535/1 = 65535
  //    LDR R0,=0 // 159 fastest
      MOVS R1,#1
      BL   udiv32_16
      LDR  R6,[R4]  // R6=end
      SUBS R7,R5,R6 // start-end
      LDR  R2,=0xFFFFFF
      ANDS R7,R7,R2
loopI2: B    loopI2

// delay
// input: R0 bus cycles
// output: none
Delay:
   SUBS R0,R0,#2
dloop:
   SUBS R0,R0,#4 // C=1 if no overflow
   NOP           // C=0 on pass through 0
   BHS  dloop
   BX   LR

    .global SysTick_Init
    .global SysTick_Wait
    .global SysTick_Wait10ms
mainG: BL   LED_Init // make PB1 output
      BL   SysTick_Init
      LDR  R4,=224000
      LDR  R5,=96000

// R4 is H, time LED is on
// R5 is L, time LED is off
// H+L should be constant
LoopG: BL   LED_On
      MOV  R0,R4 // H
      BL   SysTick_Wait
      BL   LED_Off
      MOV  R0,R5 // L
      BL   SysTick_Wait
      B    LoopG

mainH: BL   LED_Init // make PB1 output
      BL   SysTick_Init
      LDR  R4,=70
      LDR  R5,=30

// R4 is H, time LED is on
// R5 is L, time LED is off
// H+L should be constant
LoopH: BL   LED_On
      MOV  R0,R4 // H
      BL   SysTick_Wait10ms
      BL   LED_Off
      MOV  R0,R5 // L
      BL   SysTick_Wait10ms
      B    LoopH


    .global Clock_Init80MHz
    .global TimerG8_Init
    .global SysTick_Init
    .global SysTick_Wait
    .global SysTick_Wait10ms
myfunction:
     PUSH {LR}
     MOVS R0,#5
     BL   SysTick_Wait10ms
     POP {PC}

  .equ TIMG8_CTR, 0x40097800
mainI: MOVS R0,#0
      BL   Clock_Init80MHz
      BL   SysTick_Init
      MOVS R0,#5
      MOVS R1,#80 // G8 on power domain PD0, 40MGz SysClk for G8
      BL   TimerG8_Init // 10us
      LDR  R4,=TIMG8_CTR
      LDR  R5,[R4]  // start
      BL   myfunction
      LDR  R6,[R4]  // R6=end
      SUBS R7,R5,R6 // start-end
      LDR  R2,=0xFFFF
      ANDS R7,R7,R2 // R7=elapsed
loopI: B    loopI

Save: PUSH {R0-R3,LR}
      LDR  R0,=Cnt  //R0 = &Cnt
      LDR  R1,[R0]  //R1 = Cnt
      CMP  R1,#SIZE
      BHS  done2     //full?
      LDR  R3,=GPIOB_DOUT31_0
      LDR  R3,[R3]  //R3 is Port B out
      LDR  R2,=GPIOB_DIN31_0
      LDR  R2,[R2]  //R2 is Port B in
      ORRS R3,R3,R2
      LDR  R2,=Buf
      STRB R3,[R2,R1] //dump
      ADDS R1,#1
      STR  R1,[R0] //save Cnt
done2: POP  {R0-R3,PC}

Save2: PUSH {R0-R3,LR}
      LDR  R0,=Cnt  //R0 = &Cnt
      LDR  R1,[R0]  //R1 = Cnt
      LDR  R3,=GPIOB_DOUT31_0
      LDR  R3,[R3]  //R3 is Port B out
      LDR  R2,=GPIOB_DIN31_0
      LDR  R2,[R2]  //R2 is Port B in
      ORRS R3,R3,R2
      LDR  R2,=Buf
      STRB R3,[R2,R1] //dump
      ADDS R1,#1
      MOVS R3,#SIZE-1
      ANDS R1,R3     //0 to SIZE-1
      STR  R1,[R0]   //save Cnt
      POP  {R0-R3,PC}
Save3: PUSH {R0-R4,LR}
      LDR  R0,=Cnt  //R0 = &Cnt
      LDR  R1,[R0]  //R1 = Cnt
      CMP  R1,#SIZE-1
      BHS  done3     //full?
      LDR  R3,=GPIOB_DOUT31_0
      LDR  R3,[R3]  //R3 is Port B out
      LDR  R2,=GPIOB_DIN31_0
      LDR  R2,[R2]  //R2 is Port B in
      ORRS R3,R3,R2
      LDR  R2,=Buf
      LDRB R4,[R2,R1]
      CMP  R3,R4
      BEQ  done3
      ADDS R1,#1
      STR  R1,[R0] //save Cnt
      STRB R3,[R2,R1] //dump
done3: POP  {R0-R4,PC}

mainJ:
   BL   Init // make PB1 output, PB0 input
   MOVS R0,#0
   LDR  R1,=Cnt
   STR  R0,[R0]
   LDR  R6,=GPIOB_DIN31_0
   LDR  R7,=GPIOB_DOUT31_0
   MOVS R5,#1
loopJ:
   LDR  R1,[R6]  // contents of GPIOB
   ANDS R1,R1,R5 // PB0
   EORS R1,R1,R5 // flip bit 0
   LSLS R1,R1,#1 //
   STR  R1,[R7]  // output to PB1
   BL   Save3
   B    loopJ

Save4: PUSH {R0-R4,LR}
      LDR  R0,=Cnt  //R0 = &Cnt
      LDR  R1,[R0]  //R1 = Cnt
      CMP  R1,#SIZE
      BHS  done4    //full?
      LDR  R3,=GPIOB_DOUT31_0
      LDR  R3,[R3]  //Port B out
      LDR  R2,=DBuf
      STRB R3,[R2,R1] //dump
      LDR  R3,=TIMG8_CTR
      LDR  R3,[R3]  // Time
      LDR  R2,=TBuf
      LSLS R4,R1,#1  // 16bit
      STRH R3,[R2,R4] //dump
      ADDS R1,#1
      STR  R1,[R0] //save Cnt
done4: POP  {R0-R4,PC}
StepperToggle2:
      PUSH {LR}
      LDR  R1,=GPIOB_DOUTTGL31_0
      STR  R0,[R1]
      BL   Save4
      POP  {PC}
maink: MOVS R0,#0
      BL   Clock_Init80MHz
      MOVS R0,#5
      MOVS R1,#160
      MOVS R0,#0
      LDR  R1,=Cnt
      STR  R0,[R0]
      BL   TimerG8_Init // 12.5ns
      BL   SysTick_Init
      BL   StepperInit
loop4: MOVS R0,#0x0C
      BL   StepperToggle2
      LDR  R0,=166400// 20.8ms
      BL   SysTick_Wait
      MOVS R0,#0x03
      BL   StepperToggle2
      LDR  R0,=166400// 20.8ms
      BL   SysTick_Wait
      B    loop4

    .end          // end of file
