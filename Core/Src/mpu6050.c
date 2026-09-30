/*
 * mpu6050.c
 *
 *  Created on: 29 de set. de 2026
 *      Author: evert
 */


#include "mpu6050.h"
#include <math.h>

uint8_t MPU6050_Init(MPU6050_t *dev, I2C_HandleTypeDef *hi2c) {
    dev->i2c_handle = hi2c;
    dev->heading_z = 0.0f;
    dev->last_time = HAL_GetTick();

    uint8_t check, data;

    HAL_I2C_Mem_Read(dev->i2c_handle, MPU6050_I2C_ADDR, MPU6050_REG_WHO_AM_I, 1, &check, 1, 100);

    if (check == 0x68) {
        // 1. Acorda o sensor
        data = 0x00;
        HAL_I2C_Mem_Write(dev->i2c_handle, MPU6050_I2C_ADDR, MPU6050_REG_PWR_MGMT_1, 1, &data, 1, 100);

        // 2. Configura Filtro Digital Passa-Baixas (DLPF) para ~42Hz
        // Remove ruído de vibração mecânica dos motores de corrente contínua (DC)
        data = 0x03;
        HAL_I2C_Mem_Write(dev->i2c_handle, MPU6050_I2C_ADDR, MPU6050_REG_WHO_AM_I, 1, &data, 1, 100);

        return 1;
    }
    return 0;
}

// Rotina de calibração automática (Chamar com o robô totalmente PARADO na linha de largada)
void MPU6050_Calibrate(MPU6050_t *dev) {
    int32_t gyro_z_sum = 0;
    uint8_t raw_data[2];

    // Tira a média de 200 leituras para descobrir o desvio estático do sensor
    for (int i = 0; i < 200; i++) {
        // Lê especificamente o registrador do Giroscópio Z (0x47 e 0x48)
        HAL_I2C_Mem_Read(dev->i2c_handle, MPU6050_I2C_ADDR, 0x47, 1, raw_data, 2, 50);
        gyro_z_sum += (int16_t)(raw_data[0] << 8 | raw_data[1]);
        HAL_Delay(5);
    }

    // Converte a média bruta calculada para graus por segundo (°/s)
    dev->gyro_z_offset = (gyro_z_sum / 200.0f) / 131.0f;
}

uint8_t MPU6050_Update(MPU6050_t *dev) {
    uint8_t raw[14];

    if (HAL_I2C_Mem_Read(dev->i2c_handle, MPU6050_I2C_ADDR, MPU6050_REG_ACCEL_XOUT_H, 1, raw, 14, 50) != HAL_OK) {
        return 0;
    }

    // Processa dados do giroscópio Z
    dev->gyro_z_raw = (int16_t)(raw[12] << 8 | raw[13]);

    // Aplica o fator de escala e remove o offset de calibração
    dev->gz = ((float)dev->gyro_z_raw / 131.0f) - dev->gyro_z_offset;

    // Integração no tempo para descobrir o ângulo Z de guinada (Heading / Yaw)
    float dt = (HAL_GetTick() - dev->last_time) / 1000.0f;
    dev->last_time = HAL_GetTick();

    // Filtro de banda morta (ignora微ruídos residuais abaixo de 0.2 graus por segundo)
    if (fabs(dev->gz) > 0.2f) {
        dev->heading_z += dev->gz * dt; // Ângulo acumulado da direção do robô
    }

    return 1;
}
