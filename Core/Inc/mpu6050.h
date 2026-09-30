/*
 * mpu6050.h
 *
 *  Created on: 29 de set. de 2026
 *      Author: evert
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <math.h>


/* Endereço I2C */

#define MPU6050_I2C_ADDR     (0x68 << 1)

/* Mapeamento de Registradores */
#define MPU6050_REG_WHO_AM_I     0x75
#define MPU6050_REG_PWR_MGMT_1   0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B

extern float dt;



typedef struct {
    I2C_HandleTypeDef *i2c_handle;

    // Dados brutos
    int16_t accel_x_raw, accel_y_raw, accel_z_raw;
    int16_t gyro_x_raw,  gyro_y_raw,  gyro_z_raw;

    // Dados convertidos
    float ax, ay, az;
    float gx, gy, gz; // gz = velocidade de rotação na curva (°/s)

    // Offsets de Calibração (Crucial para o robô não "achar" que está girando parado)
    float gyro_z_offset;

    // Orientação calculada
    float heading_z;   // Ângulo acumulado do robô na pista (0 a 360° ou relativo)
    uint32_t last_time;
} MPU6050_t;



uint8_t MPU6050_Init(MPU6050_t *dev, I2C_HandleTypeDef *hi2c);
void MPU6050_Calibrate(MPU6050_t *dev);
uint8_t MPU6050_Update(MPU6050_t *dev);


#endif /* INC_MPU6050_H_ */
