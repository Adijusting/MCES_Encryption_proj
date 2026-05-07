#include <lpc214x.h>
#include "timer.h"

//Set up the timer to count raw clock cycles
void Timer0_Init(void) {
	T0TCR = 0x02;
	T0PR = 0x00;
}

//Reset the counter to 0 and start it
void Timer0_Start(void) {
	T0TCR = 0x02;
	T0TCR = 0x01;
}

//Stop the timer and return final product
unsigned int Timer0_Stop(void) {
	T0TCR = 0x00;
	return T0TC;
}

