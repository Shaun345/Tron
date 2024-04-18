//****************** Toggle.s ***************
// Program initially written by: Yerraballi and Valvano
// Author: Jonathan Valvano
// Last Modified: 1/16/2023
// Brief description of the program: Toggle LED
// Hardware connections:
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
       .data
       .align 2
// Declare global variables here if needed
// with the .space assembly directive

// I/O port addresses
   .equ GPIOA_RSTCTL,   0x400A0804
   .equ GPIOA_PWREN,    0x400A0800
   .equ GPIOA_DOE31_0,  0x400A12C0
   .equ GPIOA_DOUT31_0, 0x400A1280
   .equ GPIOA_DIN31_0,  0x400A1380
   .equ GPIOB_RSTCTL,   0x400A2804
   .equ GPIOB_PWREN,    0x400A2800
   .equ GPIOB_DOE31_0,  0x400A32C0
   .equ GPIOB_DOUT31_0, 0x400A3280
   .equ GPIOB_DIN31_0,  0x400A3380
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
main:
   BL   Init // make PA0 output
   LDR  R5,=GPIOA_DOUT31_0
   MOVS R4,#1  // PA0
loop:
   LDR  R2,[R5]  // contents of GPIOA
   EORS R2,R2,R4 // flip bit 0
   STR  R2,[R5]  // GPIOA->DOUT31_0 ^= 1
   LDR  R0,=16000000 // 500ms
   BL   Delay
   B    loop

// PA0 is output
Init:
   PUSH {LR}
   LDR  R0,=GPIOA_RSTCTL
   LDR  R1,=0xB1000003
   STR  R1,[R0] // GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003;
   LDR  R0,=GPIOA_PWREN
   LDR  R1,=0x26000001
   STR  R1,[R0] // GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;
   MOVS R0,#24
   BL   Delay
   LDR  R0,=IOMUXPA0
   MOVS R1, #0x81
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PA0INDEX]  = (uint32_t) 0x00000081;
   LDR  R0,=GPIOA_DOE31_0
   MOVS R4,#1  // PA0
   LDR  R2,[R0]
   ORRS R2,R2,R4
   STR  R2,[R0] //GPIOA->DOE31_0 |= 1;
   POP  {PC}


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
