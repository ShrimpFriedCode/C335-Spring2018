/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */


//Ethan Anderson, (etmander)
//3/1/18

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

int toDeg(float raw){//This function take the raw accel data and maps it's point in a 180 degree arc.

  int deg = (raw -(-1))/(1-(-1)) * (180-0) + 0;
  if(deg < 0) deg = 0;//cleanup output. since we're going from float to int, there is some odd data
  if(deg > 180) deg = 180;
  return deg;
}

int toDegA(float raw){//this func maps the roll/tilt values to their 90 degree equivalents

  int deg = ((raw - (-1.5))*(90 - (-90)))/(1.5 - (-1.5)) + -90;
  return deg;
}

int toDegB(float raw){//this function is for cleaning the roll/tilt values for easier use with their display function. 
//because of this function, I can map the "raw" values to an equivalent graph position without any calculation to avoid out of bounds display

  int deg = ((raw - (-1.5))*(60 - (-60)))/(1.5 - (-1.5)) + -60;
  return deg;
}


void displayTilt(int pitch, int roll){//display function for pitch/roll data

  char pt[] = {'P', 'I', 'T', 'C', 'H', '\0'};//strings for drawString
  char rl[] = {'R', 'O', 'L', 'L', '\0'};

  f3d_lcd_drawString(ST7735_width-35, 0, rl ,BLUE, BLACK);//draw labels
  f3d_lcd_drawString(10, 0, pt, YELLOW, BLACK);

  uint8_t yc;//Y count, for y postion in the following matricies:
  uint16_t barrierc[ST7735_width];//color matrix for the median barrier
  uint16_t pitchc[ST7735_width];//color matrix for the pitch graph
  uint16_t rollc[ST7735_width];//color matrix for the roll graph
  uint16_t blackc[ST7735_width];//color data for the "eraser"

  //fill with appropriate colors
  for (yc = 0; yc < ST7735_width; yc++) barrierc[yc] = RED;
  for (yc = 0; yc < ST7735_width; yc++) pitchc[yc] = YELLOW;
  for (yc = 0; yc < ST7735_width; yc++) rollc[yc] = BLUE;
  for (yc = 0; yc < ST7735_width; yc++) blackc[yc] = BLACK;

  //draw a 2 layer barrier in the middle of the screen to signify +/- values
  f3d_lcd_setAddrWindow (0,ST7735_height/2, ST7735_width-1, ST7735_height-1,MADCTLGRAPHICS);
  f3d_lcd_pushColor(barrierc, ST7735_width);
  f3d_lcd_pushColor(barrierc, ST7735_width);

  ///Eraser for old pitch values. erases whole +/- graph.

  f3d_lcd_setAddrWindow (10,ST7735_height/2 - 60, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  f3d_lcd_setAddrWindow (10,ST7735_height/2 + 2, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  ///Eraser for old roll values
  
  f3d_lcd_setAddrWindow (ST7735_width-35 ,ST7735_height/2 - 60, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  f3d_lcd_setAddrWindow (ST7735_width-35,ST7735_height/2 + 2, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
  for(yc = 0; yc < 60; yc++) f3d_lcd_pushColor(blackc, 31);

  ///

  //DRAW PITCH
  if(pitch >= 0){//if positive

	//draw a graph block up to the pitch value
	f3d_lcd_setAddrWindow (10,ST7735_height/2 - pitch, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
	for(yc = 0; yc < pitch; yc++) f3d_lcd_pushColor(pitchc, 31);

  }
  else if(pitch < 0){//if negative

	//draw graph down to pitch pos.
	f3d_lcd_setAddrWindow (10,ST7735_height/2 + 2, 40, ST7735_height/2 + 1,MADCTLGRAPHICS);
	for(yc = 0; yc < abs(pitch); yc++) f3d_lcd_pushColor(pitchc, 31);
  }

  //DRAW ROLL, works the same as above, just with roll data
  if(roll >= 0){

        f3d_lcd_setAddrWindow (ST7735_width-35 ,ST7735_height/2 - roll, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
        for(yc = 0; yc < roll; yc++) f3d_lcd_pushColor(rollc, 31);

  }
  else if(roll < 0){

        f3d_lcd_setAddrWindow (ST7735_width-35,ST7735_height/2 + 2, ST7735_width-5, ST7735_height/2 + 1,MADCTLGRAPHICS);
        for(yc = 0; yc < abs(roll); yc++) f3d_lcd_pushColor(rollc, 31);
  }

}

void displayComp(float heading){//Compass display function

  f3d_lcd_fillScreen2(BLACK);//reset the screen

  if(heading > .7 && heading < 1.3){//if facing North

	//draw N, S, E, W in their appropriate orientation in a 'circular' shape on screen
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 - 30, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 + 30, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2, 'W', GREEN, BLACK);

  }
  else if(heading > -2.5 && heading < -1.9){//if facing South

	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 + 30, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 - 30, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2, 'W', GREEN, BLACK);

  }
  else if(heading > 2.3 && heading < 2.9){//if facing East

	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 - 30, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 + 30, 'W', GREEN, BLACK);

  }
  else if(heading > -.8 && heading < -.3){//if facing West

	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 + 30, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2, ST7735_height/2 - 30, 'W', GREEN, BLACK);

  }
  else if(heading <= .7 && heading >= -.8){//if facing North East

	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 - 30, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 + 30, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 + 30, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 - 30, 'W', GREEN, BLACK);

  }
  else if(heading <=2.3 && heading >= 1.3){//if facing North West
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 - 30, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 + 30, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 - 30, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 + 30, 'W', GREEN, BLACK);

  }
  else if(heading <= -.8 && heading >= -1.9){//if facing South East

	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 + 30, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 - 30, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 + 30, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 - 30, 'W', GREEN, BLACK);

  }
  else if(heading <= 3.2 && heading >= -3.2){//if facing South West

	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 + 30, 'N', RED, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 - 30, 'S', BLUE, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 - 30, ST7735_height/2 - 30, 'E', YELLOW, BLACK);
	f3d_lcd_drawChar(ST7735_width/2 + 30, ST7735_height/2 + 30, 'W', GREEN, BLACK);
	
  }

}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  // Set up your inits before you go ahead
  f3d_user_btn_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_uart_init();

  //tmp arrays for data collection
  float accel_data[3];
  float mag_data[3];

  //start screen
  f3d_lcd_fillScreen2(BLACK);

  //good ol' crappy enum for button pushes. I dare you to try and overflow this.
  int count = 0;

  while(1){

	//if button pushed
	if(user_btn_read() == 1) count++;//set current selection
 	
	delay(10);//delay for things to catch up
	
	//fill tmp arrays with appropriate data
	f3d_accel_read(accel_data);
	f3d_mag_read(mag_data);

	//fill accel vars for calculation
	float x = accel_data[0];
	float y = accel_data[1];
	float z = accel_data[2];

	//fill mag vars for calculation
	float xM = mag_data[0];
	float yM = mag_data[1];
	float zM = mag_data[2];

	//vars for human readable accel data
	int degx = toDeg(accel_data[0]);
	int degy = toDeg(accel_data[1]);
	int degz = toDeg(accel_data[2]);

	//print accel data
	printf("X: %d\n", degx);
	printf("Y: %d\n", degy);
	printf("Z: %d\n", degz);

	//print mag data
	printf("XM: %lf\n", xM);
	printf("XY: %lf\n", yM);

	//calculate pitch/roll
	float pitch = atan(x/(sqrt(pow(y, 2) + pow(z, 2))));
	float roll = atan(y/(sqrt(pow(x, 2) + pow(z, 2))));

	//calculate heading
	float heading = atan2(yM,xM);
	
	//print heading data
	printf("Heading: %lf\n", heading);
	
	//print pitch/roll data
	printf("Pitch: %d\n", toDegA(pitch));
	printf("Roll: %d\n\n", toDegA(roll));


	switch(count%2){//depending on selection state, select display function

		case 1:
			displayTilt(toDegB(pitch), toDegB(roll));//pass converted vars
			break;
		case 0:
			displayComp(heading);//pass data
			break;	
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


