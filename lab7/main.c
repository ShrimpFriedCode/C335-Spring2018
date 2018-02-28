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

int toDegA(float raw){

  int deg = ((raw - (-1.5))*(90 - (-90)))/(1.5 - (-1.5)) + -90;
  return deg;
}

int toDegB(float raw){

  int deg = ((raw - (-1.5))*(60 - (-60)))/(1.5 - (-1.5)) + -60;
  return deg;
}


void displayTilt(int pitch, int roll){

  char pt[] = {'P', 'I', 'T', 'C', 'H', '\0'};
  char rl[] = {'R', 'O', 'L', 'L', '\0'};

  f3d_lcd_drawString(ST7735_width-35, 0, rl ,BLUE, BLACK);
  f3d_lcd_drawString(10, 0, pt, YELLOW, BLACK);

  uint8_t yc;
  uint16_t barrierc[ST7735_width];
  uint16_t pitchc[ST7735_width];
  uint16_t rollc[ST7735_width];
  uint16_t blackc[ST7735_width];

  for (yc = 0; yc < ST7735_width; yc++) barrierc[yc] = RED;
  for (yc = 0; yc < ST7735_width; yc++) pitchc[yc] = YELLOW;
  for (yc = 0; yc < ST7735_width; yc++) rollc[yc] = BLUE;
  for (yc = 0; yc < ST7735_width; yc++) blackc[yc] = BLACK;

  f3d_lcd_setAddrWindow (0,ST7735_height/2, ST7735_width-1, ST7735_height-1,MADCTLGRAPHICS);
  f3d_lcd_pushColor(barrierc, ST7735_width);
  f3d_lcd_pushColor(barrierc, ST7735_width);

  //NEED CLEANUP CODE TO CLEAN GRAPH AFTER EACH REFRESH

  f3d_lcd_setAddrWindow (10,ST7735_height/2 - 60, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  f3d_lcd_setAddrWindow (10,ST7735_height/2 + 2, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  //
  
  f3d_lcd_setAddrWindow (ST7735_width-35 ,ST7735_height/2 - 60, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  f3d_lcd_setAddrWindow (ST7735_width-35,ST7735_height/2 + 2, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  //DRAW PITCH
  if(pitch >= 0){

	f3d_lcd_setAddrWindow (10,ST7735_height/2 - pitch, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
	for(yc = 0; yc < pitch; yc++) f3d_lcd_pushColor(pitchc, 31);

  }
  else if(pitch < 0){

	f3d_lcd_setAddrWindow (10,ST7735_height/2 + 2, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
	for(yc = 0; yc < abs(pitch); yc++) f3d_lcd_pushColor(pitchc, 31);
  }

  //DRAW ROLL
  if(roll >= 0){

        f3d_lcd_setAddrWindow (ST7735_width-35 ,ST7735_height/2 - roll, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
        for(yc = 0; yc < roll; yc++) f3d_lcd_pushColor(rollc, 31);

  }
  else if(roll < 0){

        f3d_lcd_setAddrWindow (ST7735_width-35,ST7735_height/2 + 2, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
        for(yc = 0; yc < abs(roll); yc++) f3d_lcd_pushColor(rollc, 31);
  }

}

void displayComp(float heading){

  f3d_lcd_fillScreen2(BLACK);

  if(heading > 1.2 && heading < 1.8){

	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 - 30, 'N', RED, BLACK);

  }

}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  // Set up your inits before you go ahead

  f3d_lcd_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_uart_init();//init uart

  float accel_data[3];
  float mag_data[3];

  f3d_lcd_fillScreen2(BLACK);

  while(1){
 	//f3d_lcd_fillScreen2(BLACK);
	delay(150);
	
	f3d_accel_read(accel_data);
	f3d_mag_read(mag_data);

	float x = accel_data[0];
	float y = accel_data[1];
	float z = accel_data[2];

	float xM = mag_data[0];
	float yM = mag_data[1];
	float zM = mag_data[2];

	int degx = toDeg(accel_data[0]);
	int degy = toDeg(accel_data[1]);
	int degz = toDeg(accel_data[2]);

	printf("X: %d\n", degx);
	printf("Y: %d\n", degy);
	printf("Z: %d\n", degz);

	printf("XM: %lf\n", xM);
	printf("XY: %lf\n", yM);

	float pitch = atan(x/(sqrt(pow(y, 2) + pow(z, 2))));
	float roll = atan(y/(sqrt(pow(x, 2) + pow(z, 2))));

	float heading = atan2(yM,xM);
	
	printf("Heading: %lf\n", heading);
	
	printf("Pitch: %d\n", toDegA(pitch));
	printf("Roll: %d\n\n", toDegA(roll));

	//displayTilt(toDegB(pitch), toDegB(roll));
	displayComp(heading);
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


