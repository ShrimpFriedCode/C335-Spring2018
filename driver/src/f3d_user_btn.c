/**********************************
 *f3d_user_btn.c 
 *contains the init and read functions for the User Button
 *********************************/
//
// By Ethan Anderson (etmander)
// 1/25/2018
//

#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>


/*Initialization of the UserButton*/
void f3d_user_btn_init(void){

  GPIO_InitTypeDef GPIO_InitStructure;//define our struct
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//select our pin to initialize
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//set it to read input
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//enable clock
  GPIO_Init(GPIOA, &GPIO_InitStructure);//initialize our pin on port A

}

/*reads the User Button*/
int user_btn_read(void){

  if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){//if the button is reading an off state...
	return 0;
  }
  else{
	return 1;
  }
  
}
