//Ethan Anderson (etmander) 2/8/18

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_uart_init();
  //init and stop I/O buffing
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  //counter vars
  int newl = 0;
  int numc = 0;
  int words = 0;

  int c;
  //while not eof
  while ((c = getchar()) != 0x1b){
    //increase number of chars 
    numc++;
    //if whitespace
    if(c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32){
    	//increase word count
	words++;
	if(c == 10){//if new line 
		newl++;//increase count
	}

    }	
  }
  //print counter vars
  printf("lines: %d Words: %d Characters: %d\n", newl, words, numc);

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

