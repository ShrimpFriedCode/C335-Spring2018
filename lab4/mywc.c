//
//mywc.c
//
//By Ethan Anderson
//1/18/18

#include <stdio.h>
#include <f3d_uart.h>
#include <stm32f30x.h>
#include <f3d_led.h> 

// Simple looping delay function
void delay(void) {
   int i = 2000000;
   while (i-- > 0) {
     asm("nop"); /* This stops it optimising code out */
   }
}           

//take stdin from cmd
int main(){

	f3d_uart_init();

  	setvbuf(stdin, NULL, _IONBF, 0);
  	setvbuf(stdout, NULL, _IONBF, 0);
  	setvbuf(stderr, NULL, _IONBF, 0);
while(1){
	    //int of character and filepointer
	    //FILE *fp;
	    //setup counts for each section; lines, chars, and words
	    int newl = 0;
	    int numc = 0;
	    int words = 0;
	    //open file for reading
	    //fp = fopen(argv[1], "r");
	    //while not EOF
	    putchar(getchar());
/*
	    while ((c = getchar()) != 0x1b){
		putchar(c);
		//increase num of chars
		numc++;
		    //if current char is whitespace:
		    if(c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32){
			//increase word count
			words++;
			//if whitespace is newline:
			if (c == 10){
				//increase line count
				newl++;
			}
			}

	    }
	    //print off all counts
	    printf("lines: %d Words: %d Characters: %d\n", newl, words, numc);
*/
}
} 
