#include "gyroSensor.h"

/**
 * Constructor
 */
GyroClass::GyroClass()
{
}

/**
 * initialise gyro-sensor
 * @return {bool} success
 */
bool GyroClass::begin()
{
    mpu = new Adafruit_MPU6050();

    if (!mpu->begin(MPU_addr))
    {
        Serial.println("Failed to find MPU6050 chip");
    }
    mpu->setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu->setGyroRange(MPU6050_RANGE_250_DEG);
    mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);

    return true;
}

/**
 * reads gyro sensor and saves values
 * in variables
 * @return void
 */
void GyroClass::gyroRead()
{
    sensors_event_t a, g, temp;
    mpu->getEvent(&a, &g, &temp);

    AcX = a.acceleration.x;
    AcY = a.acceleration.y;
    AcZ = a.acceleration.z;
    Tmp = temp.temperature;
    GyX = g.gyro.x;
    GyY = g.gyro.y;
    GyZ = g.gyro.z;
}

/**
 * creates serial print of all values
 * @return void
 */
void GyroClass::serialPrint()
{
    gyroRead();
    /* Print out the values */
    Serial.print("Acceleration X: ");
    Serial.print(AcX);
    Serial.print(", Y: ");
    Serial.print(AcY);
    Serial.print(", Z: ");
    Serial.print(AcZ);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(GyX);
    Serial.print(", Y: ");
    Serial.print(GyY);
    Serial.print(", Z: ");
    Serial.print(GyZ);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(Tmp);
    Serial.println(" degC");
}

GyroClass GYROSENSOR = GyroClass();