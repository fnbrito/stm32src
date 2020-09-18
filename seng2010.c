#include <stdio.h>
#include <stdint.h>

#include "common.h"

/* This include will give us the CubeMX generated defines */
#include "main.h"

ParserReturnVal_t CmdBtn(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;


  GPIO_PinState p = HAL_GPIO_ReadPin(iBTN_USR_GPIO_Port, iBTN_USR_Pin);
  
  printf("Button State: %d\n", p);

  return CmdReturnOk;
}

ADD_CMD("btn",CmdBtn,"                Read User Button State")

