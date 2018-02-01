    //
    // dump.c
    //1/18/18
    //By Ethan Anderson and Seth Baugh

    void dump_memory(void *p, int len)
    {
      int i;
	//print reference table	
	printf("address char hexCh short\t integer\t float\t doublefloat\n");

      for (i = 0; i < len; i++) {
	//setup a switch condition (sw) to determine what point in the mem we are
	int sw = i%8;

	//c is integer representation of current address
	int c = *(char *)(p+i);
	//cc (cleaned c) is the char we will use for our print statements
	char cc = c;
	//check to see if c is an acceptable ASCII character
	if(c > 126 || c < 32){
		//if not, print a ?
		cc = '?';
	}


	switch(sw){
		//after 8 bytes, we can print all values
		case 0:
			printf("%8p %c 0x%02x %+hd %+d %+e %+le\n", p + i, cc,*(unsigned char *)(p + i), *(short *)(p + i), *(int *)(p + i), *(float *)(p + i), *(double *)(p + i));
			break;
		//only print char
		case 1:
			printf("%8p %c 0x%02x\n", p + i, cc,*(unsigned char *)(p + i));
			break;
		//we can print shorts now
		case 2:
			printf("%8p %c 0x%02x %+hd\n", p + i, cc,*(unsigned char *)(p + i), *(short *)(p + i));
			break;
		//print only char
		case 3:
			printf("%8p %c 0x%02x\n", p + i, cc,*(unsigned char *)(p + i));
			break;
		//we can print all except for the doubleFloat
		case 4:
			printf("%8p %c 0x%02x %+hd %+d %+e\n", p + i, cc,*(unsigned char *)(p + i), *(short *)(p + i), *(int *)(p + i), *(float *)(p + i));
			break;
		//we can only print char
		case 5:
			printf("%8p %c 0x%02x\n", p + i, cc,*(unsigned char *)(p + i));
			break;
		//print char and short
		case 6:
			printf("%8p %c 0x%02x %+hd\n", p + i, cc,*(unsigned char *)(p + i), *(short *)(p + i));
			break;
		//print only char
		case 7:
			printf("%8p %c 0x%02x\n", p + i, cc,*(unsigned char *)(p + i));
			break;

	}
      }
    }

