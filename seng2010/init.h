#include "stm32g4xx_hal.h"

#define iBTN_USR_Pin GPIO_PIN_13
#define iBTN_USR_GPIO_Port GPIOC
#define iaLight_Pin GPIO_PIN_0
#define iaLight_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define iBTN_EXT_Pin GPIO_PIN_10
#define iBTN_EXT_GPIO_Port GPIOB
#define iBTN_EXT_EXTI_IRQn EXTI15_10_IRQn
#define oBTN_Pin GPIO_PIN_10
#define oBTN_GPIO_Port GPIOA
#define iBTN_Pin GPIO_PIN_3
#define iBTN_GPIO_Port GPIOB
#define iBTN_EXTI_IRQn EXTI3_IRQn

void gpio_init();