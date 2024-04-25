
/*!
 * @defgroup UART
 * @brief Asynchronous serial communication
 <table>
<caption id="UARTpins">UART pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA10 <td>UART0 Tx to XDS Rx
<tr><td>PA11 <td>UART0 Rx from XDS Tx
</table>
 * @{*/
/**
 * @file      UART.h
 * @brief     Initialize UART0
 * @details   UART0 initialization. 115200 baud,
 * 1 start, 8 data bits, 1 stop, no parity.<br>

 * @version   ECE319K v1.0
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2023 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      August 13, 2023
 <table>
<caption id="UARTpins2">UART pins on the MSPM0G3507</caption>
<tr><th>Pin  <th>Description
<tr><td>PA10 <td>UART0 Tx to XDS Rx
<tr><td>PA11 <td>UART0 Rx from XDS Tx
</table>
  ******************************************************************************/

#ifndef __UART0_TX__H__
#define __UART0_TX__H__
// standard ASCII symbols
/**
 * \brief CR is carriage return
 */
#define CR   0x0D
/**
 * \brief LF is line feed
 */
#define LF   0x0A
/**
 * \brief BS is back space
 */
#define BS   0x08
/**
 * \brief ESC is escape character
 */
#define ESC  0x1B
/**
 * \brief SP is space
 */
#define SP   0x20
/**
 * \brief DEL is delete
 */
#define DEL  0x7F
/* 
 * Derived from UART0_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 */
 

/**
 * initialize 0 for 115200 baud rate.
 * - PA10 = UART0 Tx to XDS Rx
 * - PA11 = UART0 Rx from XDS Tx
 *
 * There are two implementations:
 * - UART0_Init in <b>UARTbusywait.c</b> implements busy-wait synchronization
 * - UART0_Init in <b>UARTints.c</b> implements interrupt synchronization
 *
 * @param none
 * @return none
 * @brief  Initialize UART0
*/
void UART0_Init(void);


/**
 * Output 8-bit to serial port
 * @param data is an 8-bit ASCII character to be transferred
 * @return none
 * @brief output character to UART0
 */
void UART0_OutChar(char data);


/**
 * Output String with NULL termination
 * @param pt is pointer to a NULL-terminated string to be transferred
 * @return none
 * @brief output string to UART0
 */
void UART0_OutString(char *pt);

/**
 * Output a 32-bit number in unsigned decimal format
 * @param n 32-bit number to be transferred
 * @return none
 * @note Variable format 1-10 digits with no space before or after
 * @brief output a number to UART0
 */
void UART0_OutUDec(uint32_t n);

/**
 * Output a 32-bit number in signed decimal format
 * @param n 32-bit number to be transferred
 * @return none
 * @note Variable format 1-10 digits with no space before or after
 * @brief output a signed number to UART0
 */
void UART0_OutSDec(int32_t n);

/**
 * Output a 32-bit number in unsigned hexadecimal format
 * @param number 32-bit number to be transferred
 * @return none
 * @note Variable format 1 to 8 digits with no space before or after
 * @brief output a hex number to UART0
 */
void UART0_OutUHex(uint32_t number);

/**
 * Initialize the UART for 115,200 baud rate (assuming 48 MHz bus clock),
 * 8 bit word length, no parity bits, one stop bit.
 * Calls UART0_Init()
 * @param none
 * @return none
 * @brief Initialize UART0 to use printf
 */
void UART0_InitPrintf(void);


#endif // __UART0_H__
/** @}*/
