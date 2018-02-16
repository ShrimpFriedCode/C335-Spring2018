/* main.c ---
 * *
 * * Filename: main.c
 * * Description:
 * * Author:
 * * Maintainer:
 * * Created: Thu Jan 10 11:23:43 2013
 * * Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000



int main(void) {
  // If you have your inits set up, this should turn your LCD screen red
  f3d_lcd_init();
  f3d_lcd_fillScreen(BLACK);
  f3d_uart_init();
  f3d_gyro_init();

  setvbuf(stdin, NULL, _IONBF, 0);//optimize for UART comms
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  char str[] = {'\52', '\54', '\0'};
  //f3d_lcd_drawString(0, 0, str, BLACK, WHITE);

  float tmp[3]; 

  int x = 0;
  int y = 0;
  int z = 0;

  char c1[10];
  char c2[10];
  char c3[10];

  while(1){
	delay(50); 
	f3d_gyro_getdata(tmp);
	
	x = round(tmp[0]);
	y = round(tmp[1]);
	z = round(tmp[2]);

	sprintf(c1, "%d", x);
	sprintf(c2, "%d", y);
	sprintf(c3, "%d", z);

	char xp[] = {'X', ':', '\0'};
	char yp[] = {'Y', ':', '\0'};
	char zp[] = {'Z', ':', '\0'};

	f3d_lcd_drawString(0, 0, xp, RED, BLACK);
	f3d_lcd_drawString(0, 10, yp, YELLOW, BLACK);
	f3d_lcd_drawString(0, 20, zp, BLUE, BLACK);

	f3d_lcd_drawString(15, 0, c1, WHITE, BLACK);
	f3d_lcd_drawString(15, 10, c2, WHITE, BLACK);
	f3d_lcd_drawString(15, 20, c3, WHITE, BLACK);	
	

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
