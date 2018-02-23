/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */


// map [-1 1] to 0, 179.9
// how?

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000

int toDeg(float raw){

  int deg = (raw -(-1))/(1-(-1)) * (180-0) + 0;
  if(deg < 0) deg = 0;
  if(deg > 180) deg = 180;
  return deg;
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  // Set up your inits before you go ahead

  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_uart_init();//init uart

  float accel_data[3];

  while(1){
 
	delay(1500);
	
	f3d_accel_read(accel_data);

	float x = accel_data[0];
	float y = accel_data[1];
	float z = accel_data[2];

	int degx = toDeg(accel_data[0]);
	int degy = toDeg(accel_data[1]);
	int degz = toDeg(accel_data[2]);

	printf("X: %d\n", degx);
	printf("Y: %d\n", degy);
	printf("Z: %d\n", degz);

	int pitch = atan(x/(sqrt(pow(y, 2) + pow(z, 2))));
	int roll = atan(y/(sqrt(pow(x, 2) + pow(z, 2))));

	printf("Pitch: %lf\n", pitch);
	printf("Roll: %lf\n\n", roll);

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


