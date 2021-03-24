/**
 * Verkabelung:
 *  GYR  - VCC GND  SCL  SDA
 *   |      |   |    |    |
 * ESP32 - 3v3 GND  G22  G21
 */

#ifndef _INFOVIS_GYRO_H_
#define _INFOVIS_GYRO_H_

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>

class GyroClass
{
private:
    const int MPU_addr = 0x68; // I2C address of the MPU-6050
    Adafruit_MPU6050 *mpu;

public:
    float AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

    GyroClass();
    bool begin();
    void gyroRead();
    void serialPrint();
};

extern GyroClass GYROSENSOR;

#endif