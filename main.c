#include <lpc214x.h> 
#include <stdio.h>   
#include "uart.h"    
#include "timer.h"    

extern void rc4_encrypt(char* state_array, char* key, char* plaintext, char* ciphertext);
void rc4_encrypt_c(char* s, char* key, char* plaintext, char* ciphertext) {
	int i, j = 0, t, k=0;
	int key_len = 0;
	
	//1. Get key length
	while (key[key_len] != '\0') key_len++;
	
	//2. KSA Initialization
	for (i=0; i<256; i++) s[i] = i;
	
	//3. KSA Scrambling
	for (i=0; i<256; i++) {
		j = (j+s[i] + key[i %key_len]) %256;
		
		//Swap
		t = s[i]; s[i] = s[j]; s[j] = t;
	}
	
	//4. PRGA Encryption
	i=0; j=0;
	while (plaintext[k] != '\0') {
		i = (i+1) % 256;
		j = (j + s[i]) % 256;
		
		//Swap
		t = s[i]; s[i] = s[j]; s[j] = t;
		
		//XOR
		ciphertext[k] = plaintext[k] ^ s[(s[i] + s[j]) % 256];
		k++;
	}
	ciphertext[k] = '\0'; //Null terminate
}

int main(void) {
    // 1. Variable Declarations
    char s_array[256];
    char key[] = "MyKey1234";
    char input_buffer[64];  
    char output_buffer[64]; 
    char time_string[64];      // Buffer to hold our execution time text

    // 2. Initialize Peripherals
    UART0_Init(); 
    Timer0_Init(); // Initialize our new timer
		IO0DIR |= 0x00000001;
    
    UART0_SendString("\r\n=== ARM7TDMI RC4 Hardware Crypto ===\r\n");

    // 3. The Infinite Interactive Loop
    while(1) {
			unsigned int time_c, time_asm;
			
			UART0_SendString("\r\nEnter text to encrypt: ");
			UART0_ReceiveString(input_buffer, 64);
			
			//1. Benchmark pure C
			IO0SET = 0x00000001;
			Timer0_Start();
			rc4_encrypt_c(s_array, key, input_buffer, output_buffer);
			time_c = Timer0_Stop();
			IO0CLR = 0x00000001;
			
			//2. Benchmark assembly
			Timer0_Start();
			rc4_encrypt(s_array, key, input_buffer, output_buffer);
			time_asm = Timer0_Stop();
			
			//3. Print results
			UART0_SendString("\r\nCiphertext: ");
			UART0_SendString(output_buffer);
			
			sprintf(time_string, "\r\nC Complier Time: %u cycles", time_c);
			UART0_SendString(time_string);
			
			sprintf(time_string, "\r\nAssembly Time: %u cycles", time_asm);
			UART0_SendString(time_string);
			
			UART0_SendString("\r\n---------------------");
		}
}

