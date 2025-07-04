/*
 * mpu6050.h
 *
 *  Created on: Jul 4, 2025
 *      Author: arkpi
 */


#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f4xx_hal.h"

typedef struct {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
} MPU6050_t;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Read_All(I2C_HandleTypeDef *hi2c, MPU6050_t *data);

#endif
