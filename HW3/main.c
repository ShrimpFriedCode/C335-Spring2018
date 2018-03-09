//Ethan Anderson
//
//3/9/2018
//
//Yes I know this is wayy too long, but I just went about it a different, worse way that what was suggested, because this made sense to me
#include <stdio.h>
#include <stdlib.h>

//convert a hex string into a binary string
void toBin(char *hex, char *bin){
	
	int i = 0;//iteration
	int pos = 0;//position in binary string
	while(hex[i] != '\0'){
		switch(hex[i]){//find appropriate value
			case '0':
				bin[pos]='0';
				bin[pos+1]='0';
				bin[pos+2]='0';
				bin[pos+3]='0';
				break;
			case '1':
				bin[pos]='0';
                                bin[pos+1]='0';
                                bin[pos+2]='0';
                                bin[pos+3]='1';
                                break;
			case '2':
				bin[pos]='0';
                                bin[pos+1]='0';
                                bin[pos+2]='1';
                                bin[pos+3]='0';
                                break;
			case '3':
				bin[pos]='0';
                                bin[pos+1]='0';
                                bin[pos+2]='1';
                                bin[pos+3]='1';
                                break;
			case '4':
				bin[pos]='0';
                                bin[pos+1]='1';
                                bin[pos+2]='0';
                                bin[pos+3]='0';
                                break;
			case '5':
				bin[pos]='0';
                                bin[pos+1]='1';
                                bin[pos+2]='0';
                                bin[pos+3]='1';
                                break;
			case '6':
				bin[pos]='0';
                                bin[pos+1]='1';
                                bin[pos+2]='1';
                                bin[pos+3]='0';
                                break;
			case '7':
				bin[pos]='0';
                                bin[pos+1]='1';
                                bin[pos+2]='1';
                                bin[pos+3]='1';
                                break;
			case '8':
				bin[pos]='1';
                                bin[pos+1]='0';
                                bin[pos+2]='0';
                                bin[pos+3]='0';
                                break;
			case '9':
				bin[pos]='1';
                                bin[pos+1]='0';
                                bin[pos+2]='0';
                                bin[pos+3]='1';
                                break;
			case 'A':
				bin[pos]='1';
                                bin[pos+1]='0';
                                bin[pos+2]='1';
                                bin[pos+3]='0';
                                break;
			case 'B':
				bin[pos]='1';
                                bin[pos+1]='0';
                                bin[pos+2]='1';
                                bin[pos+3]='1';
                                break;
			case 'C':
				bin[pos]='1';
                                bin[pos+1]='1';
                                bin[pos+2]='0';
                                bin[pos+3]='0';
                                break;
			case 'D':
				bin[pos]='1';
                                bin[pos+1]='1';
                                bin[pos+2]='0';
                                bin[pos+3]='1';
                                break;
			case 'E':
				bin[pos]='1';
                                bin[pos+1]='1';
                                bin[pos+2]='1';
                                bin[pos+3]='0';
                                break;
			case 'F':
				bin[pos]='1';
                                bin[pos+1]='1';
                                bin[pos+2]='1';
                                bin[pos+3]='1';
                                break;
		}
				
		pos+=4;//itterate
		i++;
	}
}

//convedry binary to decimal, for use with imm8
int binToDec(char *imm8){
	int i;	//iterator
	int mult = 1;//binary value at ith pos
	int sum = 0;//total value

	for(i = 7; i > -1; i--){//iterate through, if 1, then add to sum
		if(imm8[i] == '1'){
			sum += mult;
		}
	mult *= 2;//increse binary pos value
	}
	return sum;//return
}

//works the same as binToDec, just designed for 4 insted of 8 binary chars
int binToDecOP(char *op){
        int i;
        int mult = 1;
        int sum = 0;

        for(i = 3; i > -1; i--){
                if(op[i] == '1'){
                        sum += mult;
                }
        mult *= 2;
        }
        return sum;
}

//returns appropriate value based off of registry values. USed for regname selection
int regComp(char *reg){
	
	if(reg[0] == '0' && reg[1] == '0' && reg[2] == '0'){
		return 0;
	}
	else if(reg[0] == '0' && reg[1] == '0' && reg[2] == '1'){
		return 1;
	}
	else if(reg[0] == '0' && reg[1] == '1' && reg[2] == '0'){
                return 2;
        }
	else if(reg[0] == '0' && reg[1] == '1' && reg[2] == '1'){
                return 3;
        }
	else if(reg[0] == '1' && reg[1] == '0' && reg[2] == '0'){
                return 4;
        }
	else if(reg[0] == '1' && reg[1] == '0' && reg[2] == '1'){
                return 5;
        }
	else if(reg[0] == '1' && reg[1] == '1' && reg[2] == '0'){
                return 6;
        }
	else if(reg[0] == '1' && reg[1] == '1' && reg[2] == '1'){
                return 7;
        }

}

//decomplier
void decomp(int format, char *bin){
	//open file for appending
	FILE * fp;
	fp = fopen("test.out", "a");
	//word banks
	char *opnames[] = {"ands", "eors", "lsls", "lsrs", "asrs", "adcs", "sbcs", "rors", "tst", "rsbs", "cmp", "cmn", "orrs", "muls", "bics", "mvns"};
	char *regnames[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
	//reg and op arrays
	char reg[3];
	char reg1[3];
	char reg2[3];
	char imm8[8];
	char op[4];
	//based on type of code determined in format, run operations
	if (format == 0){

		if(bin[5] == '0'){//register 3bit
			if(bin[6] == '0'){//add
				reg[0] = bin[7];
				reg[1] = bin[8];
				reg[2] = bin[9];
				reg1[0] = bin[10];
				reg1[1] = bin[11];
				reg1[2] = bin[12];
				reg2[0] = bin[13];
				reg2[1] = bin[14];
				reg2[2] = bin[15];
				
				fprintf(fp, "    adds %s,%s,%s\n", regnames[regComp(reg2)], regnames[regComp(reg1)], regnames[regComp(reg)]);
										
			}
			else{//sub
				
				reg[0] = bin[7];
                                reg[1] = bin[8];
                                reg[2] = bin[9];
                                reg1[0] = bin[10];
                                reg1[1] = bin[11];
                                reg1[2] = bin[12];
                                reg2[0] = bin[13];
                                reg2[1] = bin[14];
                                reg2[2] = bin[15];

                                fprintf(fp, "    subs %s,%s,%s\n", regnames[regComp(reg2)], regnames[regComp(reg1)], regnames[regComp(reg)]);
	
			}
		}
		else{//immediate 3bit
			if(bin[6] == '0'){
				reg[0] = bin[7];
                                reg[1] = bin[8];
                                reg[2] = bin[9];
                                reg1[0] = bin[10];
                                reg1[1] = bin[11];
                                reg1[2] = bin[12];
                                reg2[0] = bin[13];
                                reg2[1] = bin[14];
                                reg2[2] = bin[15];

                                fprintf(fp, "    adds %s,%s,%s\n", regnames[regComp(reg2)], regnames[regComp(reg1)], regComp(reg));
			}

                        else{//sub
				reg[0] = bin[7];
                                reg[1] = bin[8];
                                reg[2] = bin[9];
                                reg1[0] = bin[10];
                                reg1[1] = bin[11];
                                reg1[2] = bin[12];
                                reg2[0] = bin[13];
                                reg2[1] = bin[14];
                                reg2[2] = bin[15];

                                fprintf(fp, "    subs %s,%s,%s\n", regnames[regComp(reg2)], regnames[regComp(reg1)], regComp(reg));
                        }
		}
	}
	else if(format == 1){
		if(bin[3] == '0' && bin[4] == '0'){//mov
			reg[0] = bin[5];
                        reg[1] = bin[6];
                        reg[2] = bin[7];

			imm8[0] = bin[8];
			imm8[1] = bin[9];
			imm8[2] = bin[10];
			imm8[3] = bin[11];
			imm8[4] = bin[12];
			imm8[5] = bin[13];
			imm8[6] = bin[14];
			imm8[7] = bin[15];

			fprintf(fp, "    mov %s,%d\n", regnames[regComp(reg)], binToDec(imm8));
		}
		else if(bin[3] == '0' && bin[4] == '1'){//cmp
			reg[0] = bin[5];
                        reg[1] = bin[6];
                        reg[2] = bin[7];

                        imm8[0] = bin[8];
                        imm8[1] = bin[9];
                        imm8[2] = bin[10];
                        imm8[3] = bin[11];
                        imm8[4] = bin[12];
                        imm8[5] = bin[13];
                        imm8[6] = bin[14];
                        imm8[7] = bin[15];

                        fprintf(fp, "    cmp %s,%d\n", regnames[regComp(reg)], binToDec(imm8));
		}
		else if(bin[3] == '1' && bin[4] == '0'){//add
			reg[0] = bin[5];
                        reg[1] = bin[6];
                        reg[2] = bin[7];

                        imm8[0] = bin[8];
                        imm8[1] = bin[9];
                        imm8[2] = bin[10];
                        imm8[3] = bin[11];
                        imm8[4] = bin[12];
                        imm8[5] = bin[13];
                        imm8[6] = bin[14];
                        imm8[7] = bin[15];

                        fprintf(fp, "    adds %s,%d\n", regnames[regComp(reg)], binToDec(imm8));
		}
		else{//sub
			reg[0] = bin[5];
                        reg[1] = bin[6];
                        reg[2] = bin[7];

                        imm8[0] = bin[8];
                        imm8[1] = bin[9];
                        imm8[2] = bin[10];
                        imm8[3] = bin[11];
                        imm8[4] = bin[12];
                        imm8[5] = bin[13];
                        imm8[6] = bin[14];
                        imm8[7] = bin[15];

                        fprintf(fp, "    subs %s,%d\n", regnames[regComp(reg)], binToDec(imm8));
		}
	}
	else if(format == 2){

		reg[0] = bin[10];
                reg[1] = bin[11];
                reg[2] = bin[12];

                reg1[0] = bin[13];
                reg1[1] = bin[14];
                reg1[2] = bin[15];	

		op[0] = bin[6];
		op[1] = bin[7];
		op[2] = bin[8];
		op[3] = bin[9];

		fprintf(fp, "    %s %s,%s\n", opnames[binToDecOP(op)], regnames[regComp(reg1)], regnames[regComp(reg)]);

	}

	fclose(fp);//close after done

}

//determine format of binary based of of first 3 bits
int formatC(char x, char y, char z){

	if(x == '0' && y == '0' && z == '0'){
		//add/sub imm
		return 0;
	}
	else if(x == '0' && y == '0' && z == '1'){
		//add/sub/com/mov imm
		return 1;
	}
	else if(x == '0' && y == '1' && z == '0'){
		//data-processing
		return 2;
	}
	else{//unrecognized
		return 3;
	}

}
//main
int main(int argc, char **argv[]){
	//add . info and reset file
	FILE * fp;
        fp = fopen("test.out", "w");		
	fprintf(fp, ".text\n.syntax unified\n.thumb\n");
	fclose(fp);
	//binary array
	char bin[16];
	
	int i;	
	//for all passed args
	for(i = 1; i < argc; i++){
		//determine their binary values
		toBin(argv[i], bin);
		//run them through the above functions
		decomp(formatC(bin[0], bin[1], bin[2]), bin);

	}

	
}

