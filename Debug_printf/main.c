//main.c for lab6
//
//Ethan Anderson (etmander)
//     2/15/2018
//
#include <f3d_uart.h>
#include <stdio.h>
#include  <stdlib.h>
#include <math.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

void delayBTN(void) {//delay function to allow for the user to press the button reliably
  int i = 800000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

void delay(void) {//delay function to allow te led to turn off/on
  int i = 100000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}



int main(void){

  f3d_led_init();
  f3d_user_btn_init();//init drivers
  f3d_uart_init();

  setvbuf(stdin, NULL, _IONBF, 0);//optimize for UART comms
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  int sel = 0;//lazy man's enum

  while(1){
	delayBTN();//allow user to press button in loop

	if(user_btn_read() == 1){//if pressed, increment selection var
		sel++;
	}

	switch(sel%2){//dpeneding on slection var state
	
	  case 0: //flash leds
		f3d_led_on(1);
		delay();
		f3d_led_off(1);
		break;
	  case 1: //flash and printf
		f3d_led_on(1);
		printf("This is a test!\n");
		f3d_led_off(1);
		break;
	}
	
  }  
  
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
