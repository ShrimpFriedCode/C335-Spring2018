//
// By Ethan Anderson (etmander)
// 1/25/18
//

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  //var for led pin selected to be on in loop
  int i = 0;
  //initialize the leds and user button
  f3d_led_init();
  f3d_user_btn_init();

  while(1){//while the board is running...
 	//while the user button is not being held...
	while(user_btn_read() != 1){

		if(i > 7){//if we have gone through all leds, enter 'all on' stage
			f3d_led_all_on();//set all leds to on
			i = 0;//reset counter
			delay();//delay
			f3d_led_all_off();//reset
		}
		else{//else, continue the loop
		
			if(i!=0){//if we are at the beginning, we have noting to turn off
				f3d_led_off(i-1);//if we are not on the first pin, turn off the previous one
			}

			f3d_led_on(i);//turn on this pin
			i++;//increase counter
			delay();//delay for next pin
		}
	}
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
