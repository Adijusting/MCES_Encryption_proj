#ifndef UART_H
#define UART_H

void UART0_Init(void);
void UART0_SendChar(char ch);
void UART0_SendString(char* str);
char UART0_ReceiveChar(void);
void UART0_ReceiveString(char* buffer, int max_length);

#endif

