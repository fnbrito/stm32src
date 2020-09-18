#include <stdio.h>

#include "init.h"

#include "common.h"


void gpio_init() {

  GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|oBTN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : iBTN_USR_Pin */
  GPIO_InitStruct.Pin = iBTN_USR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(iBTN_USR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin oBTN_Pin */
  //GPIO_InitStruct.Pin = LD2_Pin|oBTN_Pin;
  GPIO_InitStruct.Pin = oBTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : iBTN_EXT_Pin iBTN_Pin */
  GPIO_InitStruct.Pin = iBTN_EXT_Pin|iBTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}


ParserReturnVal_t CmdInitGPIO(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  gpio_init();
  
  return CmdReturnOk;
}
ADD_CMD("initgpio",CmdInitGPIO,"                Initialize additional GPIO")