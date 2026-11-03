/* 
 * File:   uart.h
 * Author: LALITH
 *
 * Created on 22 May, 2025, 10:36 AM
 */


#ifndef UART_H
#define	UART_H

#define FOSC                20000000

void init_uart(unsigned long baud);
unsigned char getchar(void);
void putchar(unsigned char data);
void puts(const char *s);

#endif	/* UART_H */

