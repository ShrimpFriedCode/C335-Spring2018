//Ethan Anderson (etmander)
//1/23/2018

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct CELL *LIST;
struct CELL {
	int val;
	LIST next;
	};

extern LIST stack = NULL;

push(int val){

	LIST c = (LIST) malloc(sizeof(struct CELL));
	if (c){
		c->val = val;
		c->next = stack;
		stack = c;
	} else {
		printf("Error\n");
		exit(0);
	}
}

allfree(){//completely free the entire stack

	LIST c = stack;//temp var

	if(c != NULL){//while we still have entries
		while(c->next != NULL){
			c = stack->next;//hold position of next node
			free(stack);//free last node
			stack = c;//move down
		}

		free(c);//free last node
	}

}

printStack(){//print the whole stack

	LIST c = stack;//temp var

	while(c->next != NULL){//while we still have nodes to visit
		printf("%d\n", c->val);//print current node value
		c = c->next;//move down
	}

	printf("%d\n", c->val);//print last value

}

int pop(){//returns current node value and removes from stack (PRECONDITION: stack is not null)

	int t1;//temp var 

	if(stack != NULL){//do not run if possible seg fault

		int t1 = stack->val;//set value of temp var to node val
		LIST c = (LIST) malloc(sizeof(struct CELL));//create new empty stack
		c = stack->next;//set new stack to the next entry in current stack
		free(stack);//free current node
		stack = c;//set current stack
		return(t1);//return val
	}

}

int main(){	
	char in[100];//string buffer

	while(scanf("%99s", in) != EOF){//while we have not signaled to quit, take input upto buffer

		if(in[0] == 'q'){//quit
			allfree();//free all mem
			exit(0);//exit
		}
		else if(isdigit(in[0])){//digit handling
			int val = atoi(in);//pull digit
			push(val);//push to stack
		}
		else if(in[0] == '+'){//add
			if(stack == NULL || stack->next == NULL){//if there are not enough values for pop
				printf("Stack does not have enough values!\n");//warn user and do not run
			}
			else{
				int i1 = pop();//get vars for operation
				int i2 = pop();
				push((i1+i2));//push new num
			}
		}
		else if(in[0] == '-'){///sub
			if(stack == NULL || stack->next == NULL){
                        	printf("Stack does not have enough values!\n");
                	}
                	else{
              	 		int i1 = pop();
                		int i2 = pop();
               			push((i2-i1));
                	}	
		}
		else if(in[0] == '/'){//div
			if(stack == NULL || stack->next == NULL){
                        	printf("Stack does not have enough values!\n");
                	}
                	else{
                		int i1 = pop();
                		int i2 = pop();
                		push((i2/i1));
                	}
		}
		else if(in[0] == '*'){//multi
			if(stack == NULL || stack->next == NULL){
                        	printf("Stack does not have enough values!\n");
                	}
                	else{
                		int i1 = pop();
                		int i2 = pop();
                		push((i1*i2));
                	}
		}
		else if(in[0] == 'p'){//print top
			if(stack){//if the stack is not null
				printf("%d\n", stack->val);//get current node val
			}
			else{
				printf("Stack is empty!\n");//else warn user
			}
		}
		else if(in[0] == 'f'){//print stack
			if(stack != NULL){//ensure stack is not empty
				printStack();
			}
		}
		else if(in[0] == 'c'){//clear stack
			allfree();//free all values
			stack = NULL;//set stack to null
		}
		else{
			printf("Command not recognized: %c\n", in[0]);
		}
	}

	allfree();//free mem on EOF exit	
}
