#include "I2Cdev.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static I2C_HandleTypeDef hi2c;

void i2clib_init(I2C_HandleTypeDef h)
{
  hi2c = h;
}



/** Read single byte from an 8-bit device register.
    @param devAddr I2C slave device address
    @param regAddr Register regAddr to read from
    @param data Container for byte value read from device
    @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
    @return Status of read operation (true = success)
*/
int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint16_t timeout) {
    return readBytes(devAddr, regAddr, 1, data, timeout);
}



/** Read multiple bytes from an 8-bit device register.
    @param devAddr I2C slave device address
    @param regAddr First register regAddr to read from
    @param length Number of bytes to read
    @param data Buffer to store read data in
    @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
    @return Number of bytes read (-1 indicates failure)
*/
int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data, uint16_t timeout) {
    #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print("I2C (0x");
    Serial.print(devAddr, HEX);
    Serial.print(") reading ");
    Serial.print(length, DEC);
    Serial.print(" bytes from 0x");
    Serial.print(regAddr, HEX);
    Serial.print("...");
    #endif

    HAL_StatusTypeDef ret;
    uint8_t *buf = alloca(length + 1); //uint8_t buf[33];


    // HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
    //ret = HAL_I2C_Mem_Read(&hi2c, devAddr, (uint16_t)regAddr, I2C_MEMADD_SIZE_8BIT, buf, length, HAL_MAX_DELAY);


    // Generic read of bytes on i2c
    buf[0] = regAddr;

    // Transmit the request
    ret = HAL_I2C_Master_Transmit(&hi2c, devAddr, buf, 1, HAL_MAX_DELAY);
    if ( ret != HAL_OK ) {
      printf("Read Req Error 0x%x\n", buf[0]);
    } else {

      // Receive the answer
      ret = HAL_I2C_Master_Receive(&hi2c, devAddr, buf, length, HAL_MAX_DELAY);
      if ( ret != HAL_OK ) {
        printf("Read Error 0x%x\n", buf[0]);
      } else {
        //printf("\nREC 0x%x\n", buf[0]);
      }
    }

    if (length > 0)
    {
        memcpy(data, buf, length);
    }


    #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print(". Done (");
    Serial.print(count, DEC);
    Serial.println(" read).");
    #endif

    return ret;
}



/** Write single byte to an 8-bit device register.
    @param devAddr I2C slave device address
    @param regAddr Register address to write to
    @param data New byte value to write
    @return Status of operation (true = success)
*/
bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return writeBytes(devAddr, regAddr, 1, &data);
}



/** Write multiple bytes to an 8-bit device register.
    @param devAddr I2C slave device address
    @param regAddr First register address to write to
    @param length Number of bytes to write
    @param data Buffer to copy new data from
    @return Status of operation (true = success)
*/
bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data) {
    #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print("I2C (0x");
    Serial.print(devAddr, HEX);
    Serial.print(") writing ");
    Serial.print(length, DEC);
    Serial.print(" bytes to 0x");
    Serial.print(regAddr, HEX);
    Serial.print("...");
    #endif

    HAL_StatusTypeDef ret;
    uint8_t *buf = alloca(length + 1); //uint8_t buf[33];
    //uint8_t act_length = length > 32 ? 32 :length;

    buf[0] = regAddr;
    //buf[1] = 0; // Data goes here

    // dest src size
    if (length > 0)
    {
        memcpy(buf + 1, data, length);
    }

    //printf("DEST: %x DATA: %x\n", regAddr, data);

    // We transmit one extra byte at the beginning for the target register address
    ret = HAL_I2C_Master_Transmit(&hi2c, devAddr, buf, length + 1, HAL_MAX_DELAY);
    if ( ret != HAL_OK ) {
      printf("WriteBytes Error: %d\n", (int)ret);
    }

    return ret;

}
