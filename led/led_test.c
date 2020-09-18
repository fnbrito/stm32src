#include <stdio.h>
#include <stdlib.h>
#include "stm32g4xx_hal.h"

#include "led_matrix.h"
#include "i2c/I2Cdev.h"

#include <string.h> // Needed for memset
#include "common.h"

void initLED(void)
{
    printf("Setting up led matrix\n");
    // LED Matrix Controller
    uint8_t DevAddress = HT16K33_DEFAULT_I2C_ADDR; // 0x70 << 1;

    // Shifted by one for 7 bit addressing
    //DevAddress = HT16K33_DEFAULT_I2C_ADDR << 1;

    // Init

    HAL_Delay(100);

    ht16k33_init(DevAddress << 1);
    //writeBytes(DevAddress, 0x21, 0, (uint8_t*)NULL);
    /*
    buf[0] = 0x21;
    ret = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, buf, 1, HAL_MAX_DELAY);
    if ( ret != HAL_OK ) {
      strcpy((char*)buf, "Init Error Tx\r\n");
      printf("Error: %d\n", (int)ret);
    }
    */

    HAL_Delay(100);

    ht16k33_setBrightness(0);
    //writeBytes(DevAddress, 0xE0, 0, &buf[1]);
    /*
    buf[0] = 0xE0;
    ret = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, buf, 1, HAL_MAX_DELAY);
    if ( ret != HAL_OK ) {
      strcpy((char*)buf, "BR Error Tx\r\n");
      printf("Error: %d\n", (int)ret);
    }
    */

    HAL_Delay(100);

    ht16k33_setBlinkRate(BLINK_OFF);

    uint8_t* buf = alloca(16);
    memset(buf, 0xFF, 16);
    writeBytes(DevAddress << 1, 0x00, 16, buf);    

    HAL_Delay(100);

    memset(buf, 0, 16);
    writeBytes(DevAddress << 1, 0x00, 16, buf);    




}

void writeString(char* message) {

    matrix_create();
    //matrix_setDisplayOrientation(DISPLAY_ROTATE_180);
    matrix_clear();
    matrix_writeString(message, 1250, ACTION_SCROLLING); // ACTION_SCROLLING or ACTION_SHIFT
    //printf("Writing string\n");
    matrix_display();


}

void writeIcon(uint32_t icon) {

  matrix_create();
  matrix_clear();
  matrix_writeIcon(icon);
  matrix_display();

}


ParserReturnVal_t CmdInitLED(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  initLED();
  
  return CmdReturnOk;
}
ADD_CMD("initled",CmdInitLED,"                Initialize LED Matrix")

ParserReturnVal_t CmdWriteMSG(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  char* message;

  uint32_t rc = fetch_string_arg(&message);
  if(rc) {
    printf("Please supply a string to display\n");
    return CmdReturnBadParameter1;
  }


  writeString(message);
  
  return CmdReturnOk;
}
ADD_CMD("writemsg",CmdWriteMSG,"                Write LED Matrix")

ParserReturnVal_t CmdWriteIcon(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  uint32_t rc, val;

  rc = fetch_uint32_arg(&val);
  if(rc) {
    printf("Please supply integer icon\n");
    return CmdReturnBadParameter1;
  }


  writeIcon(val);
  
  return CmdReturnOk;
}
ADD_CMD("writeicon",CmdWriteIcon,"                Write LED Matrix icon")
