#include <stm32f30x.h>
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <stdio.h>
#include <math.h>
#include <f3d_i2c.h>
#include <f3d_user_btn.h>
#include <f3d_nunchuk.h>
#include <f3d_accel.h>
#include <f3d_lcd_sd.h>
#include <stdlib.h>
#include <f3d_i2c2.h>

#define TIMER 20000

int STATE = 0;


struct pos
{
	int x, y;
};

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


void moveCheck(int x1, int y1, int x2, int y2){

	uint8_t yc;
        uint16_t wall[ST7735_width];
        uint16_t ball[ST7735_width];
        uint16_t hole[ST7735_width];

        for (yc = 0; yc < ST7735_width; yc++) ball[yc] = RED;

	if(x1 < 0 && y1 > 0 && y1 < ST7735_height-1){
		f3d_lcd_setAddrWindow (0, y1, 10, y2,MADCTLGRAPHICS);
        	f3d_lcd_pushColor(ball, 110);
	}
	else if(x2 > ST7735_width-1 && y1 > 0 && y1 < ST7735_height-1){
		f3d_lcd_setAddrWindow (ST7735_width-11, y1, ST7735_width-1, y2,MADCTLGRAPHICS);
                f3d_lcd_pushColor(ball, 110);
	}
	
	else if(x1 > 0 && x1 < ST7735_width-1 && y1 < 0){
		f3d_lcd_setAddrWindow (x1, 0, x2, 10,MADCTLGRAPHICS);
                f3d_lcd_pushColor(ball, 110);
	}
	
	else if(x1 > 0 && x1 < ST7735_width-1 && y1 > ST7735_height-1){
		f3d_lcd_setAddrWindow (x1, ST7735_height-11, x2, ST7735_height-1,MADCTLGRAPHICS);
                f3d_lcd_pushColor(ball, 110);
	}
	else if(x1 < 0 && y1 < 0){
		f3d_lcd_setAddrWindow (0, 0, 10, 10,MADCTLGRAPHICS);
                f3d_lcd_pushColor(ball, 110);
	}
	else if(x2 > ST7735_width-1 && y2 > ST7735_height-1){
		f3d_lcd_setAddrWindow (ST7735_width-11, ST7735_height-11, ST7735_width-1, ST7735_height-1,MADCTLGRAPHICS);
                f3d_lcd_pushColor(ball, 110);
	}
	else if(x1 < 0 && y1 > ST7735_height-1){

		f3d_lcd_setAddrWindow (0, ST7735_height-11, 10, ST7735_height-1,MADCTLGRAPHICS);
                f3d_lcd_pushColor(ball, 110);

	}
	else if(x2 > ST7735_width-1 && y1 < 0){

	f3d_lcd_setAddrWindow (ST7735_width-11, 0, ST7735_width-1, 10,MADCTLGRAPHICS);
        f3d_lcd_pushColor(ball, 110);

	}
	else{

	f3d_lcd_setAddrWindow (x1, y1, x2, y2,MADCTLGRAPHICS);
	f3d_lcd_pushColor(ball, 110);

	}

}


int goalCheck(int x1, int y1, int x2, int y2, int gx1, int gy1, int gx2, int gy2){
	int ret = 0;

	if(x1 == gx1 && x2 == gx2 && y1 == gy1 && y2 == gy2){
	ret = 1;
        }

	return ret;
}




int main(){
	char yes[] = {'P','R','E','S','S',' ','B','T','N',' ','F','O','R',' ','S','C','O','R','E','S', '\0'};
        char hscore[] = {'H','I','G','H','S','C','O','R','E',':','\0'};
	char list[] = {'B','E','S','T',' ','S','C','O','R','E',':', '\0'};
	char ctime[10];
	int ballx1 = ST7735_width/2 -10;
	int bally1 = ST7735_height/2;
	int ballx2 = ST7735_width/2;
	int bally2 = ST7735_height/2 + 10;

	int goalx1 = rand()%(ST7735_width-11 - 0) + 0;
	int goaly1 = rand()%(ST7735_height-11 -0) + 0;
	int goalx2 = goalx1 + 10;
	int goaly2 = goaly1 + 10;

	int run = 1;
	int show = 0;

	while(1){
		delay(50);
		switch(STATE){
			case 0:
				//initializations
				f3d_user_btn_init();
				delay(10);
				f3d_lcd_init();
				delay(10);
				f3d_i2c1_init();
				delay(10);
				f3d_accel_init();
				delay(10);
				f3d_uart_init();
				delay(10);
				//f3d_i2c2_init();
				delay(10);
				//f3d_nunchuk_init();
				
				f3d_lcd_fillScreen2(BLACK);
				
				setvbuf(stdin,NULL, _IONBF,0);
				setvbuf(stdout, NULL, _IONBF, 0);
				setvbuf(stderr, NULL, _IONBF, 0);
				
				float tilt[3];
			
				
				struct pos curr = {0, 0};
				struct pos last = {0, 0};
				struct pos goal = {0, 0};
				
				//nunchuk_t nun = {0, 0, 0, 0, 0, 0, 0};
				
				int diffX = 0;
				int diffY = 0;
				
				uint8_t yc;
				uint16_t ball[ST7735_width];
				uint16_t hole[ST7735_width];

				for (yc = 0; yc < ST7735_width; yc++) ball[yc] = RED;
				for (yc = 0; yc < ST7735_width; yc++) hole[yc] = BLACK;
				
				int bestTime = 999999;
				int time = 0;
				
				int button = 0;
				
				STATE = 1;
				break;
			case 1:
							
				f3d_lcd_drawString(0, 0, yes, RED, BLACK);
				
				int count = 0;
				/*
				f3d_nunchuk_read();
				
				//PRINT NUNCHUCK OUTPUT SO WE KNOW HOW TO SWITCH!
				
				if(user_btn_read() == 1) count++;
				
				switch(count%2){
					case 1:
						STATE = 2;
						break;
					case 0:
						STATE = 5;
						break;
				}
				*/
				delay(1000);
				if(user_btn_read()==1){
					STATE = 5;
				}
				else{
					STATE = 2;
				}
	
				break;
			case 2:
				
				f3d_lcd_fillScreen2(WHITE);
				STATE = 3;
				int goalx1 = rand()%(ST7735_width-11 - 0) + 0;
                                int goaly1 = rand()%(ST7735_height-11 -0) + 0;
                                int goalx2 = goalx1 + 10;
                                int goaly2 = goaly1 + 10;
				int ballx1 = ST7735_width/2 -10;
                                int bally1 = ST7735_height/2;
                                int ballx2 = ST7735_width/2;
                                int bally2 = ST7735_height/2 + 10;
				
			case 3:
				
				time++;
				f3d_lcd_fillScreen2(WHITE);
				f3d_accel_read(tilt);

				int degx = toDeg(tilt[0]);
      				int degy = toDeg(tilt[1]);
        			int degz = toDeg(tilt[2]);

				float x = tilt[0];
        			float y = tilt[1];
        			float z = tilt[2];	

				float pitch = atan(x/(sqrt(pow(y, 2) + pow(z, 2))));
        			float roll = atan(y/(sqrt(pow(x, 2) + pow(z, 2))));	

		
				printf("Pitch: %d\n", toDegA(pitch));
       				printf("Roll: %d\n\n", toDegA(roll));

				

				int xa = toDegA(pitch);
                                int ya = toDegA(roll);

				ballx1 -= ya;
				ballx2 -= ya;
				bally1 -= xa;
				bally2 -= xa;
			
				curr.x = ballx1;
				curr.y = bally1;
				
				f3d_lcd_setAddrWindow (goalx1, goaly1, goalx2, goaly2,MADCTLGRAPHICS);
                                f3d_lcd_pushColor(hole, 110);

				moveCheck(ballx1, bally1, ballx2, ballx2);				
				
				if(goalCheck(ballx1, bally1, ballx2, bally2, goalx1, goaly1, goalx2, goaly2) == 1){
					STATE = 4;
					run = 1;
					show = 0;
				}
			        
				if(user_btn_read() == 1){
					delay(1000);	
					
					STATE = 1;

					time = 0;
				}
					
				break;
			case 4:
				
				if(time < bestTime){
					bestTime = time;
					show = 1;
				}
				while(run){
					delay(1000);
					f3d_lcd_fillScreen2(BLACK);
				
					char go[] = {'G', 'A', 'M', 'E', ' ', 'O', 'V', 'E', 'R', '\0'};
					char score[]={'N','E','W',' ','H','I','G','H',' ','S','C','O','R','E','!','\0'};
				
					if(show){
						f3d_lcd_drawString(0, 10, score, RED, BLACK);
						sprintf(ctime, "%d", time);
						f3d_lcd_drawString(0, 20, ctime, RED, BLACK);
					}
					f3d_lcd_drawString(0, 0, go, RED, BLACK);
					
					if(user_btn_read() == 1){
						run = 0;
						STATE = 5;
					}				

				}
				break;
			case 5:
			
				f3d_lcd_fillScreen2(BLACK);
				sprintf(ctime, "%d", bestTime);	
				f3d_lcd_drawString(0, 0, hscore, RED, BLACK);
				f3d_lcd_drawString(0, 10, ctime, RED, BLACK);		
				
				delay(1000);

				if(user_btn_read()==1){
					STATE = 2;
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


