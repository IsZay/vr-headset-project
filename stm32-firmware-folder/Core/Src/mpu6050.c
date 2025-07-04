/*
 * mpu6050.c
 *
 *  Created on: Jul 4, 2025
 *      Author: arkpi
 */


#include "mpu6050.h"

// I'm putting the defines here just because no other file should be wanting these values, its only for the MPU
#define MPU6050_ADDR 0x68 << 1
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t check;
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, HAL_MAX_DELAY);
    if (check != 0x68) return HAL_ERROR;

    // Wake up MPU6050
    uint8_t data = 0;
    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_Read_All(I2C_HandleTypeDef *hi2c, MPU6050_t *data) {
    uint8_t raw[14];
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, ACCEL_XOUT_H, 1, raw, 14, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;

    data->ax = (raw[0] << 8) | raw[1];
    data->ay = (raw[2] << 8) | raw[3];
    data->az = (raw[4] << 8) | raw[5];
    data->gx = (raw[8] << 8) | raw[9];
    data->gy = (raw[10] << 8) | raw[11];
    data->gz = (raw[12] << 8) | raw[13];

    return HAL_OK;
}
