#ifndef _I2CDEV_H_
#define _I2CDEV_H_


// -----------------------------------------------------------------------------
// Arduino-style "Serial.print" debug constant (uncomment to enable)
// -----------------------------------------------------------------------------
//#define I2CDEV_SERIAL_DEBUG

#include <stdint.h>
#include <stdbool.h>
#include "stm32g4xx_hal.h"


#define millis() 0

// support Arduino M0
#ifndef BUFFER_LENGTH
    #define BUFFER_LENGTH 64
#endif

// 1000ms default read timeout (modify with "readTimeout = [ms];")
#define I2CDEV_DEFAULT_READ_TIMEOUT     1000



void i2clib_init(I2C_HandleTypeDef h);

int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint16_t timeout);
int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data,
                        uint16_t timeout);

bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data);






#endif /* _I2CDEV_H_ */
