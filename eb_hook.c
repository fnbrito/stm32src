#include "seng2010/init.h"

#include <stdio.h>

#include "common.h"

#include "seng2010/init.h"

void ebInit(void *data)
{

  /* Place Initialization things here.  This function gets called once
   * at startup.
   */

  gpio_init();

}

void ebTask(void *data)
{

  /* Place your task functionality in this function.  This function
   * will be called repeatedly, as if you placed it inside the main
   * while(1){} loop.
   */

}

ADD_TASK(ebTask,  /* This is the name of the function for the task */
	 ebInit,  /* This is the initialization function */
	 NULL,         /* This pointer is passed as 'data' to the functions */
	 0,            /* This is the number of milliseconds between calls */
	 "This is the help text for the task")
  

ParserReturnVal_t ebExample(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Put your command implementation here */
  printf("Example Command\n");
  
  return CmdReturnOk;
}

ADD_CMD("eb",ebExample,"                Example Command")


void EXTI3_IRQHandler(void)
{

  static uint32_t LedState = 0;		// keep track of the led state in a private variable

  LedState = LedState ? 0 : 1;		// toggle the led state value

  HAL_GPIO_WritePin(oBTN_GPIO_Port,oBTN_Pin,LedState);
    printf("EXTI3 Triggered\n");
    HAL_GPIO_EXTI_IRQHandler(iBTN_Pin);
}

void EXTI15_10_IRQHandler(void)
{

  static uint32_t LedState = 0;		// keep track of the led state in a private variable

  LedState = LedState ? 0 : 1;		// toggle the led state value

  HAL_GPIO_WritePin(oBTN_GPIO_Port,oBTN_Pin,LedState);
    printf("EXTI1510 Triggered\n");
    HAL_GPIO_EXTI_IRQHandler(iBTN_EXT_Pin);
}

