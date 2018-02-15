/* main.c 
   debug this function using gdb*/


/*
  The expected performace of the program is as follows:
  We want to print out the actual bits of numbers! What a great
  way to understand how number representation works

 */

//The libraries to include
#include <f3d_uart.h>
#include <stdio.h> 
#include <stdint.h>
//The arrays that store the bits
//
//Ethan Anderson (etmander)
//2/15/2018
//

int gitBitsChar(unsigned char c, char answer[]){
  int i=7;//program was stepping out of bounds here
  while(i>-1){
    if(c&1) answer[i] = '1';
    else answer[i] = '0';
    c=c>>1;
    i--;
  }
  answer[8]=0;//program was setting valuable information to null
  return 0;
}

int gitBitsShort(unsigned short s, char answer[]){
  int i = 15;//program was stepping out of bounds here, and had a confusing format, fixed format.
  while(i>-1){
    if(s&1) answer[i] = '1';
    else answer[i] = '0';
    s=s>>1;//direction was reversed for the shift
    i--;//format changes
  }
  answer[16]=0;//was setting valuable info to null
  return 0;
}

int gitBitsInt(int yikes, char answer[]){
  int i = 30;//program was stepping out of bounds
  while(i>-1){//completely wrong while loop here, changed format as well
    if(yikes&1) answer[i] = '1';
    else answer[i] = '0';
    yikes=yikes>>1;//shift was reversed
    i--;//format changes
  }
  answer[31]=0;//set nonvaluable info to null to terminate return
  return 0;
}

//runs some test on our program
int main(){
  f3d_uart_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  char ans1[32];
  char ans2[32];
  char ans3[32];
  char ans4[32];
  while (1){
    gitBitsShort((unsigned short) 3200, ans1);
    gitBitsChar((unsigned char) 'a', ans2);
    gitBitsInt(-94, ans3);
    gitBitsInt((unsigned int) 234, ans4);
    printf("unsigned short 3200= %s\n", ans1);
    printf("unsigned a = %s\n", ans2);
    printf("-94 = %s\n", ans3);
    printf("unsigned 234 = %s\n", ans4);
  }
 
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
