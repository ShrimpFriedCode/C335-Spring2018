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

void delay(void) {//delay function to allow for the user to press the button reliably
  int i = 800000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}


int main(void){

  f3d_uart_init();//initialize all devices and pins used
  f3d_gyro_init();
  f3d_user_btn_init();
  f3d_led_init();

  setvbuf(stdin, NULL, _IONBF, 0);//optimize for UART comms
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  float tmp[3]; //create a temporary array to store gyro x,y,z values

  int diff = 0; //difference value, used to determine change from location

  int i = 0; //a lazy man's enum for current axis selected

  printf("select strting axis:\n");//start by selecting axis on UART

  char c = getchar();//get selection

  if(c == 'x'){//select based on user input. if invalid, start with X
	i = 0;
  }
  else if(c == 'y'){
	i = 1;
  }
  else if(c == 'z'){
	i = 2;
  }

  f3d_gyro_getdata(tmp);//get current gyro data once

  int lastx = round(tmp[0]);//set base position for the gyro, so we can figure out how far we've moved
  int lasty = round(abs(tmp[1]));
  int lastz = round(tmp[2]);

  while(1){//running loop
	delay();//delay each tick, to allow for the button to only register one press per tick
	f3d_led_all_off();//reset positional LEDs
        f3d_gyro_getdata(tmp);//constantly get current gyro data

	if(user_btn_read() == 1){//if user button has been pressed, change axis
		delay();
		i++;
	}

	switch (i%3){//select axis based on current selection var
	
	  case 0:
		printf("X: %f\n", tmp[0]);//print axis pos

		diff = round(abs(tmp[0])) - lastx; //find absolute non-float pos of this axis
		
		break;
	  case 1:
		printf("Y: %f\n", tmp[1]);//print pos
	
		diff = round(abs(tmp[1])) - lasty;//determine difference from base pos

		break;
	  case 2:
		printf("Z: %f\n", tmp[2]);//print pos
		
		diff = round(abs(tmp[2])) - lastz;//determine difference from base pos

		break;
	}

	if(diff >= 5 && diff < 10){//large conditional that determines number of leds to switch on based on +/- pos difference in increments of 5.
                        f3d_led_on(1);
                }
                else if(diff >= 10 && diff < 15){
                        f3d_led_on(1);
                        f3d_led_on(2);
                }
                else if(diff >= 15 && diff < 20){
                        f3d_led_on(1);
                        f3d_led_on(2);
                        f3d_led_on(3);
                }
                else if(diff >= 20 && diff < 25){
                        f3d_led_on(1);
                        f3d_led_on(2);
                        f3d_led_on(3);
                        f3d_led_on(4);
                }
                else if(diff >= 25){
                        f3d_led_on(1);
                        f3d_led_on(2);
                        f3d_led_on(3);
                        f3d_led_on(4);
                        f3d_led_on(5);
                }
                else if(diff <= -5 && diff > -10){
                        f3d_led_on(1);
                }
                else if(diff <= -10 && diff > -15){
                        f3d_led_on(1);
                        f3d_led_on(0);
                }
                else if(diff <= -15 && diff > -20){
                        f3d_led_on(1);
                        f3d_led_on(0);
                        f3d_led_on(7);
                }
                else if(diff <= -20 && diff > -25){
                        f3d_led_on(1);
                        f3d_led_on(0);
                        f3d_led_on(7);
                        f3d_led_on(6);
                }
                else if(diff <= -25){
                        f3d_led_on(1);
                        f3d_led_on(0);
                        f3d_led_on(7);
                        f3d_led_on(6);
                        f3d_led_on(5);
                }
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
