//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include  <stdlib.h>
#include <math.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

void delay(void) {
  int i = 800000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}


int main(void){

  f3d_uart_init();
  f3d_gyro_init(); 
  f3d_user_btn_init();
  f3d_led_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  float tmp[3];  

  int diff = 0;

  int i = 0;

  printf("select strting axis:\n");

  char c = getchar();

  if(c == 'x'){
	i = 0;
  }
  else if(c == 'y'){
	i = 1;
  }
  else if(c == 'z'){
	i = 2;
  }

  f3d_gyro_getdata(tmp);

  int lastx = round(tmp[0]);
  int lasty = round(abs(tmp[1]));
  int lastz = round(tmp[2]);

  while(1){
	delay();
	f3d_led_all_off();
        f3d_gyro_getdata(tmp);

	if(user_btn_read() == 1){
		delay();
		i++;
	}

	switch (i%3){
	
	  case 0:
		printf("X: %f\n", tmp[0]);

		diff = round(abs(tmp[0])) - lastx;
		
		break;
	  case 1:
		printf("Y: %f\n", tmp[1]);
	
		diff = round(abs(tmp[1])) - lasty;

		break;
	  case 2:
		printf("Z: %f\n", tmp[2]);
		
		diff = round(abs(tmp[2])) - lastz;

		break;
	}

	if(diff >= 5 && diff < 10){
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
        //printf("X: %f, Y: %f, Z: %f\n", test[0], test[1], test[2]);
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
