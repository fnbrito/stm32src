/* i2c.c:
 *
 *   Functions for handling the I2C bus.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "stm32g4xx_hal.h"

#include "i2c/I2Cdev.h"

#include "common.h"
#define SAVE_FLASH_SPACE
static I2C_HandleTypeDef hi2c1;

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
  HAL_StatusTypeDef rc;

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x30A0A7FB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  rc = HAL_I2C_Init(&hi2c1);

  if (rc != HAL_OK)
  {
    printf("I2C initialization failed with rc=%u\n",rc);
  }
  /** Configure Analogue filter
  */

  rc = HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
  if (rc != HAL_OK)
  {
    printf("I2C Configure Analog filter failed with rc=%u\n",rc);
  }
  /** Configure Digital filter
  */
  rc = HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0);
  if (rc != HAL_OK)
  {
    printf("I2C Configure Digital filter failed with rc=%u\n",rc);
  }

  i2clib_init(hi2c1);

}

ParserReturnVal_t CmdInitI2C(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  MX_I2C1_Init();
  
  return CmdReturnOk;
}
ADD_CMD("initi2c",CmdInitI2C,"                Initialize I2C bus")


ParserReturnVal_t CmdScanI2C(int mode)
{
  HAL_StatusTypeDef rc;
  uint32_t i;

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  for(i = 0; i<0xff; i+=2) {
    rc = HAL_I2C_IsDeviceReady(&hi2c1, i, 1, 30);
    if(rc == HAL_OK) {
      printf("Found device at address: 0x%02lx\n",i >> 1);
    }
  }

  
  return CmdReturnOk;
}
ADD_CMD("scani2c",CmdScanI2C,"                Scan I2C bus for active peripherals")

static ParserReturnVal_t I2CReadDumpHelper(int mode, uint32_t type)
{
  uint8_t *buf;
  uint32_t rc2;
  HAL_StatusTypeDef rc;
  uint32_t id, addr, size;

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Fetch arguments */
  id = 0;
  rc2 = fetch_uint32_arg(&id);
  if(rc2) {
    printf("Need to specify I2C address to use\n");
    return CmdReturnBadParameter1;
  }

  addr = 0;
  rc2 = fetch_uint32_arg(&addr);
  if(rc2) {
    printf("Need to specify I2C memory address to use\n");
    return CmdReturnBadParameter2;
  }

  /* Default to reading 1 byte */
  size = 1;
  fetch_uint32_arg(&size);

  if(size > 0x100) {
    printf("Too large a size requested\n");
    return CmdReturnBadParameter3;
  }

  buf = alloca(size);

  rc = readBytes(id << 1, addr, size, buf, 30);
  
  if(rc != HAL_OK) {
    printf("Unable to read from I2C address 0x%02lx\n", id);
  }
  DumpBuffer(buf, size, addr);
  
  return CmdReturnOk;



}

ParserReturnVal_t CmdRead16I2C(int mode)
{
  return I2CReadDumpHelper(mode, I2C_MEMADD_SIZE_16BIT);
}
ADD_CMD("read16i2c",CmdRead16I2C,"<id> <addr> <bytes>  Read 16 bit mem device on I2C bus")

ParserReturnVal_t CmdReadI2C(int mode)
{
  return I2CReadDumpHelper(mode, I2C_MEMADD_SIZE_8BIT);
}
ADD_CMD("readi2c",CmdReadI2C,"<id> <addr> <bytes>  Read 8 bit mem device on I2C bus")

static ParserReturnVal_t I2CWriteHelper(int mode, uint32_t type)
{
  uint8_t value;
  uint32_t rc2;
  HAL_StatusTypeDef rc;
  uint32_t id, addr, val;

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  /* Fetch arguments */
  id = 0;
  rc2 = fetch_uint32_arg(&id);
  if(rc2) {
    printf("Need to specify I2C address to use\n");
    return CmdReturnBadParameter1;
  }

  addr = 0;
  rc2 = fetch_uint32_arg(&addr);
  if(rc2) {
    printf("Need to specify I2C memory address to use\n");
    return CmdReturnBadParameter2;
  }

  

  /* Default to writing 1 byte */
  val = 0xff;
  fetch_uint32_arg(&val);

  value = val;

  rc = HAL_I2C_Mem_Write(&hi2c1, id << 1, addr, type,
			&value, 1, 30);
  if(rc != HAL_OK) {
    printf("Unable to write to I2C address 0x%02lx\n", id);
  }
  
  return CmdReturnOk;

}

ParserReturnVal_t CmdWrite16I2C(int mode)
{
  return I2CWriteHelper(mode, I2C_MEMADD_SIZE_16BIT);
}
ADD_CMD("write16i2c",CmdWrite16I2C,"<id> <addr> <byte>  Wrtie 16 bit mem device on I2C bus")

ParserReturnVal_t CmdWriteI2C(int mode)
{
  return I2CWriteHelper(mode, I2C_MEMADD_SIZE_8BIT);
}
ADD_CMD("writei2c",CmdWriteI2C,"<id> <addr> <byte>  Write 8 bit mem device on I2C bus")

