/*---------------------------------------------------------------------------------------
 * Testprogram:             
 * 				 Print on LED usign Timers
 *                                                                        mooga 4/2018
 *---------------------------------------------------------------------------------------
*/
#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void wait(void){
	int tmp;
	for(tmp=0;tmp<100;tmp++);
}
void timerConfig(void){
    SYSCTL_RCGCTIMER_R |= 0x1;
    while(!(SYSCTL_PRTIMER_R & 0x1));
    TIMER0_CTL_R   &= ~0x1;
    TIMER0_CFG_R   |= 0x4;
    TIMER0_TAMR_R  |= 0x02;
    TIMER0_TAPR_R  |= 2;
}

void timerWait(unsigned short usec) {
	timerConfig();
	TIMER0_TAILR_R = ceil(16 * pow(10, 6) * usec / 2) - 1;
	TIMER0_CTL_R = 0x01;
	while((TIMER0_RIS_R & 0x01) == 0);
	TIMER0_ICR_R |= 0x01;
	TIMER0_CTL_R &= ~0x01;
}


 int main(void) {
	 unsigned char input, edge;
	 static unsigned char temp = 0x00;
	 int i =0;

	 //
	 unsigned char arr[45]= {0X00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF, 0X18, 0x18, 0xFF, 0xFF, 0x00,0xC0,0x70,0x1C,0x16,0x11,0x16,0x1C,0x17,0xC3,0x0E,0x38,0xE0,0x20,0x10,0x08,0x10,0x20,0xA0,0x38,0x0E,0x03,0x00};
	SYSCTL_RCGCGPIO_R |= 1<<3; // Port D clock ini
	SYSCTL_RCGCGPIO_R |= 0x800; // Port M clock ini

	while ((SYSCTL_PRGPIO_R & 0x00000800) == 0);		// Port M ready ?
	while ((SYSCTL_PRGPIO_R & 1<<3) == 0);		// Port D ready ?

	// Set direction
	GPIO_PORTM_DIR_R = 0xFF;
	GPIO_PORTD_AHB_DIR_R = 0x00;

	// Digital enable
	GPIO_PORTM_DEN_R = 0xFF;
	GPIO_PORTD_AHB_DEN_R = 0x01;
	while(1){
		input = GPIO_PORTD_AHB_DATA_R & 0x01;
		edge = GPIO_PORTD_AHB_DATA_R;
		if(temp != input){
//	GPIO_PORTM_DATA_R = 0xFF;
			temp = input;
			timerWait(0.010);
			if(edge == 0x01){
				for(i=0;i<45;i++){
					GPIO_PORTM_DATA_R =	arr[i];
					timerWait(0.010);
				}
			}else {
				for(i=45;i>45;i--){
					GPIO_PORTM_DATA_R =	arr[i];
					timerWait(20);
				}
			}

		}else {
			GPIO_PORTM_DATA_R = 0x00;
		}
	}
}
