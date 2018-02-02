//
//mywc.c
//
//By Ethan Anderson
//1/18/18

#include <stdio.h>

//take stdin from cmd
int main(int argc, char **argv[])
{
	    //int of character and filepointer
	    int c;
	    FILE *fp;
	    //setup counts for each section; lines, chars, and words
	    int newl = 0;
	    int numc = 0;
	    int words = 0;
	    //open file for reading
	    fp = fopen(argv[1], "r");
	    //while not EOF
	    while ((c = getchar()) != 0x1b){
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
} 
