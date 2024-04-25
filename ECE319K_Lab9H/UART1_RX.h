
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

#ifndef __UART1_RX_H__
#define __UART1_RX_H__
// standard ASCII symbols
/**
 * \brief CR is carriage return
 */
#define CR 0x0D
/**
 * \brief LF is line feed
 */
#define LF 0x0A
/**
 * \brief BS is back space
 */
#define BS 0x08
/**
 * \brief ESC is escape character
 */
#define ESC 0x1B
/**
 * \brief SP is space
 */
#define SP 0x20
/**
 * \brief DEL is delete
 */
#define DEL 0x7F
/*
 * Derived from uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 */

/**
 * initialize 0 for 115200 baud rate.
 * - PA10 = UART0 Tx to XDS Rx
 * - PA11 = UART0 Rx from XDS Tx
 *
 * There are two implementations:
 * - UART_Init in <b>UARTbusywait.c</b> implements busy-wait synchronization
 * - UART_Init in <b>UARTints.c</b> implements interrupt synchronization
 *
 * @param none
 * @return none
 * @brief  Initialize UART0
 */
void UART1_Init(void);

/**
 * Wait for new serial port input
 * @param none
 * @return char ASCII code for key typed
 * @brief input from UART0
 */
char UART1_InChar(void);

#endif // __UART_H__
       /** @}*/