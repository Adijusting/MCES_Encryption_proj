#include <lpc214x.h>
#include "uart.h"

void UART0_Init(void) {
    PINSEL0 |= 0x00000005;  
    U0LCR = 0x83;           
    U0DLL = 97;             
    U0DLM = 0;
    U0LCR = 0x03;           
}

void UART0_SendChar(char ch) {
    while (!(U0LSR & 0x20)); 
    U0THR = ch;              
}

void UART0_SendString(char* str) {
    while (*str != '\0') {
        UART0_SendChar(*str++);
    }
}

char UART0_ReceiveChar(void) {
    while (!(U0LSR & 0x01)); 
    return U0RBR;            
}

void UART0_ReceiveString(char* buffer, int max_length) {
    int i = 0;
    char ch;
    while (i < (max_length - 1)) {
        ch = UART0_ReceiveChar(); 
        if (ch == '\r' || ch == '\n') break;
        buffer[i++] = ch; 
        UART0_SendChar(ch); 
    }
    buffer[i] = '\0'; 
}