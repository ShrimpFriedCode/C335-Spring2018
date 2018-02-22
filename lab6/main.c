/* main.c ---
 * *
 * * Filename: main.c
 * * Description: display for gyro
 * * Author: Ethan Anderson (etmander)
 * * Created: Thu Jan 10 11:23:43 2013
 * * Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>//includes for math operations 
#include <math.h>

#define TIMER 20000



int main(void) {
  // If you have your inits set up, this should turn your LCD screen red
  f3d_lcd_init();//initialize LCD
  f3d_lcd_fillScreen2(BLACK);//clear screen
  f3d_uart_init();//init uart
  f3d_gyro_init();//init gyro

  setvbuf(stdin, NULL, _IONBF, 0);//optimize for UART comms
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  float tmp[3]; //tmp arr for storing gyro data

  int x = 0;
  int y = 0; //x/y/z vars for raw data
  int z = 0;

  char c1[10];
  char c2[10]; //arrays for 'string' gyro data
  char c3[10];

  int storedValX = 0;
  int storedValY = 0; //stored value of gyro from last tick
  int storedValZ = 0;

  int valDiff = 0;//calculated difference between current and stored gyro data

  while(1){

	delay(50); 
	
	f3d_gyro_getdata(tmp);//get gyro data
	
	x = round(tmp[0]);
	y = (round(tmp[1]));//assign raw data to vars
	z = round(tmp[2]);

	sprintf(c1, "%d", x);
	sprintf(c2, "%d", y);//convert raw data to strings
	sprintf(c3, "%d", z);

	char xp[] = {'X', ':', '\0'};
	char yp[] = {'Y', ':', '\0'};//print statement for identified displayed data
	char zp[] = {'Z', ':', '\0'};

	f3d_lcd_drawString(0, 0, xp, RED, BLACK);
	f3d_lcd_drawString(0, 10, yp, YELLOW, BLACK);//print what axis we're looking at
	f3d_lcd_drawString(0, 20, zp, BLUE, BLACK);

	f3d_lcd_drawString(15, 0, c1, WHITE, BLACK);
	f3d_lcd_drawString(15, 10, c2, WHITE, BLACK);//print data
	f3d_lcd_drawString(15, 20, c3, WHITE, BLACK);	

	int xg = x - 437;
	int yg = y + 569; //clean values for use on graph display
	int zg = z - 380;

	int j;//iter vars for for loops
	int iter;

	//draw graph up to given x pos
	for(j = 0; j < 10; j++){
		for(iter = 0; iter < xg; iter++){

			f3d_lcd_drawPixel(j, 158 - iter, RED);
	
		}	
	}
	

	valDiff = storedValX - xg;//calculate current difference between cuur and stored

	if(valDiff > 0){//if the graph dropped
		for(j = 0; j < 10; j++){
			for(iter = xg; storedValX - iter > 0 ; iter++){
				f3d_lcd_drawPixel(j, 158 - iter, BLACK);//erase old data
			}
		}
	}
	
	storedValX = xg;//store current data as old for next cycle
	
	//do the same as above, with y data
        for(j = 0; j < 10; j++){
        	for(iter = 0; iter < yg; iter++){
                	f3d_lcd_drawPixel(j+20, 158 - iter, YELLOW);
        	}	
        }

        valDiff = storedValY - yg;

        if(valDiff > 0){
       		for(j = 0; j < 10; j++){
                	for(iter = yg; iter < storedValY; iter++){
                        	f3d_lcd_drawPixel(j+20, 158 - iter, BLACK);
                	}
        	}
        }
        
        storedValY = yg;

	//do the same as above for z data	
        for(j = 0; j < 10; j++){
        	for(iter = 0; iter < zg; iter++){
                	f3d_lcd_drawPixel(j+40, 158 - iter, BLUE);
        	}
        }

        valDiff = storedValZ - zg;

        if(valDiff > 0){
        	for(j = 0; j < 10; j++){
                	for(iter = zg; iter < storedValZ; iter++){
                        	f3d_lcd_drawPixel(j+40, 158 - iter, BLACK);
                	}
        	}
        }
        
        storedValZ = zg;

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
