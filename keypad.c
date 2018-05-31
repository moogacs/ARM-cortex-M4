/*---------------------------------------------------------------------------------------
 * Testprogram:  Port M LED on/off
 * 	             Print "MP-Labor" on console
 *                                                                        mooga 3/2018
 *---------------------------------------------------------------------------------------
*/

#include "tm4c1294ncpdt.h"
#include "stdio.h"

void wait(void){
	int tmp;
	for(tmp=0;tmp<100;tmp++);
}
int main(void) {

	  int i,j;
	  unsigned char key;
	  static char x;
	  // array of the keypad
	  char  array[4][4]= {
			  {'1', '2', '3', 'F'},
			  {'4', '5', '6', 'E'},
			  {'7', '8', '9', 'D'},
			  {'A', '0', 'B', 'C'},
	  };

		SYSCTL_RCGCGPIO_R |= 0x800;
		// Port M clock ini
		while ((SYSCTL_PRGPIO_R & 0x00000800) ==0);		// Port M ready ?

	 // Set direction
		GPIO_PORTM_DIR_R = 0xF0;

	 // Digital enable
		GPIO_PORTM_DEN_R = 0xFF;

	 // Ports on/off
		while(1){
			// columns
			for(i=0;i<4;i++){
				GPIO_PORTM_DATA_R = ~(0x80>>i);
				wait();
				//rows
				key  = GPIO_PORTM_DATA_R & 0x0F;
				for(j=0;j<4;j++){
					if((key & (1u<<j)) ==0){
						if(array[j][i] != x){
							x = array[j][i];
							printf(" %c \n" ,x);
						}
					}
				}
			}
		}
}
