//****************** InputOutput.s ***************
// Programs 3.2.1, 3.2.2, 3.2.3, 3.2.4, and 3.2.5 from the book
// Author: Jonathan Valvano
// Last Modified: April 17, 2023
// Brief description of the program:
// This is a simple assembly language project that reads the two switches
// on the LaunchPad and sets the red/green/blue LED.
// The red LED1 is on.
// If SW2 is pressed, the LED2 blue color toggles.
// If SW3 is pressed, the LED2 red color toggles.

       .data
       .align 2
// Declare global variables here if needed
// with the .space assembly directive

// I/O port addresses
   .equ GPIOA_RSTCTL,   0x400A0804
   .equ GPIOA_PWREN,    0x400A0800
   .equ GPIOA_DOE31_0,  0x400A12C0
   .equ GPIOA_DOUT31_0, 0x400A1280
   .equ GPIOA_DOUTSET31_0, 0x400A1290
   .equ GPIOA_DOUTCLR31_0, 0x400A12A0
   .equ GPIOA_DOUTTGL31_0, 0x400A12B0
   .equ GPIOA_DIN31_0,  0x400A1380
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

// Instructions go in FLASH ROM
       .section .text
       .thumb
       .align 2
       .global main
LED1_Init:  // PA0 output
  PUSH {LR}
  LDR  R1,=0xB1000003
  LDR  R0,=GPIOA_RSTCTL
  STR  R1,[R0]  // reset A
  LDR  R1,=0x26000001
  LDR  R0,=GPIOA_PWREN
  STR  R1,[R0]  // power A
  MOVS R0,#24
  BL   Delay    // stabilize
  LDR  R1,=0x00000081
  LDR  R0,=IOMUXPA0 // PINCM register for PA0
  STR  R1,[R0]      // PA0 is output
  LDR  R0,=GPIOA_DOE31_0
  LDR  R1,[R0]  // previous
  MOVS R2,#0x01 // Bit 0 means PA0
  ORRS R1,R1,R2 // friendly
  STR  R1,[R0]  // enable out
  POP  {PC}

LED1_On:
  LDR  R0,=GPIOA_DOUT31_0
  LDR  R1,[R0]  // previous
  MOVS R2,#1
  BICS R1,R1,R2 // PA0=0
  STR  R1,[R0]  // LED on
  BX   LR
LED1_Off:
  LDR  R0,=GPIOA_DOUT31_0
  LDR  R1,[R0]  // previous
  MOVS R2,#1
  ORRS R1,R1,R2 // PA0=1
  STR  R1,[R0]  // LED off
  BX   LR
main7: BL  LED1_Init
loop7: BL  LED1_On
      LDR R0,=16000000 // 1sec
      BL  Delay
      BL  LED1_Off
      LDR R0,=16000000
      BL  Delay
      B   loop7
main: BL  LED1_Init
     MOVS R5,#1
     LDR R4,=GPIOA_DOUTTGL31_0
loop8:
      STR R5,[R4]
      LDR R0,=16000000 // 1sec
      BL  Delay
      B   loop8
main2: BL   LaunchPadInit

      MOVS R0,#1
      BL   LaunchPad_LED1
      LDR  R4,=1<<22  // blue
      LDR  R5,=1<<26  // red
      MOVS R6,R4  // data
loop: LDR  R0,=16000000 // 31.25ns*16000000 = 500ms
      BL   Delay
      BL   LaunchPad_InS2
      CMP  R0,#0
      BEQ  skip
      EORS R6,R6,R4
skip: BL   LaunchPad_InS3
      CMP  R0,#0
      BEQ  out
      EORS R6,R6,R5
out:  MOVS R0,R6
      BL   LaunchPad_LED
      B    loop


// turn on both PortA and PortB
Init:
   PUSH {LR}
   LDR  R0,=GPIOA_RSTCTL
   LDR  R1,=0xB1000003
   STR  R1,[R0] // GPIOA->GPRCM.RSTCTL = 0xB1000003;
   LDR  R0,=GPIOB_RSTCTL
   STR  R1,[R0] // GPIOB->GPRCM.RSTCTL = 0xB1000003;
   LDR  R0,=GPIOA_PWREN
   LDR  R1,=0x26000001
   STR  R1,[R0] // GPIOA->GPRCM.PWREN = 0x26000001;
   LDR  R0,=GPIOB_PWREN
   STR  R1,[R0] // GPIOB->GPRCM.PWREN = 0x26000001;
   MOVS R0,#24
   BL   Delay   // stabilize, delay=24 cycles
   POP  {PC}


// make PB22, PB26, and PB27 outputs
LaunchPadInit:
   PUSH {LR}
   BL   Init
   LDR  R1, =0x00000081   // regular output pin
   LDR  R0,=IOMUXPA0
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PA0INDEX]  =  0x00000081;
   LDR  R1,=0x00050081
   LDR  R0,=IOMUXPA18
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PA18INDEX] = 0x00050081; // input, pull down
   LDR  R1,=0x00060081
   LDR  R0,=IOMUXPB21
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB21INDEX] = 0x00060081; // input, pull up
   LDR  R1,=0x00100081
   LDR  R0,=IOMUXPB22
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB22INDEX] = 0x00100081;
   LDR  R0,=IOMUXPB26
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB26INDEX] = 0x00100081;
   LDR  R0,=IOMUXPB27
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB27INDEX] = 0x00100081;
   LDR  R0,=GPIOA_DOE31_0
   LDR  R2,[R0]
   MOVS R3,#1
   ORRS R2,R2,R3
   STR  R2,[R0]  // PA0 output enable
   LDR  R0,=GPIOB_DOE31_0
   LDR  R2,[R0]
   LDR  R3,=((1<<27)|(1<<26)|(1<<22))
   ORRS R2,R2,R3
   STR  R2,[R0] // PB27 PB26 PB22 output enable
   POP  {PC}


   // return R0 bit 18
LaunchPad_InS2:  // positive logic
  LDR  R1,=GPIOA_DIN31_0
  LDR  R0,[R1]     // value of port A
  LDR  R3,=(1<<18) // PA18
  ANDS R0,R0,R3
  BX   LR


   // return R0 bit 21
LaunchPad_InS3:  // negative logic
  LDR  R1,=GPIOB_DIN31_0
  LDR  R0,[R1]     // value of port B
  LDR  R3,=(1<<21) // PB21
  EORS R0,R0,R3    // convert to positive logic
  ANDS R0,R0,R3
  BX   LR


// R0 bit 0 is RED1
LaunchPad_LED1:
  LDR  R1,=GPIOA_DOUT31_0
  LDR  R2,[R1]     // value of port A
  MOVS R3,#1
  EORS R0,R0,R3 // convert to positive logic
  BICS R2,R2,R3
  ORRS R2,R2,R0
  STR  R2,[R1]
  BX   LR

TogglePA0:
  LDR  R1, =GPIOA_DOUTTGL31_0
  MOVS R0, #1
  STR  R0, [R1]
  BX   LR

// Set LED2
// input R0 bit 22 is blue, bit 26 is red, bit 27 is green
LaunchPad_LED:
  LDR  R1,=GPIOB_DOUT31_0
  LDR  R2,[R1]     // value of port B
  LDR  R3,=((1<<22)|(1<<26)|(1<<27))
  BICS R2,R2,R3
  ORRS R2,R2,R0
  STR  R2,[R1]
  BX   LR
// Set LED2 to white
LEDwhite:
  LDR  R1,=GPIOB_DOUTSET31_0
  LDR  R0,=0x0C400000
  STR  R0,[R1]
  BX   LR
// Turn LED2  off
LEDoff:
  LDR  R1,=GPIOB_DOUTCLR31_0
  LDR  R0,=0x0C400000
  STR  R0,[R1]
  BX   LR


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

    .end          // end of file
