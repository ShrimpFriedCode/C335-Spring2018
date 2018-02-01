#include <stdio.h>
#include <unistd.h>

int main(){

while(1){

	 switch(rand()%8){
	 	case 0:
			 printf("%sHello World!", "\x1b[0m");
			 printf("\n");
			 sleep(3);
			 break;
		case 1:
			 printf("%sHello World!", "\x1b[31m");
			 printf("\n");
			 sleep(3);
			 break;
		case 2:
			 printf("%sHello World!", "\x1b[32m");
			 printf("\n");
			 sleep(3);
			 break;
		case 3:
			 printf("%sHello World!", "\x1b[33m");
			 printf("\n");
			 sleep(3);
			 break;
		case 4:
			 printf("%sHello World!", "\x1b[34m");
			 printf("\n");
			 sleep(3);
			 break;
		case 5:
			 printf("%sHello World!", "\x1b[35m");
			 printf("\n");
			 sleep(3);
			 break;
		case 6:
			 printf("%sHello World!", "\x1b[36m");
			 printf("\n");
			 sleep(3);
			 break;
		case 7:
			 printf("%sHello World!", "\x1b[37m");
			 printf("\n");
			 sleep(3);
			 break;
	 }

}

}
