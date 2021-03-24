/**
 * Verkabelung:
 *  GAS  - VCC GND   A0   D0
 *   |      |   |    |    |
 * ESP32 - 3v3 GND  G34  ---
 */

#ifndef _INFOVIS_GAS_H_
#define _INFOVIS_GAS_H_

#include <Arduino.h>
#include <MQUnifiedsensor.h>

class GasClass
{
private:
    const int16_t analogGasPin = 25;
    MQUnifiedsensor *mq135;
    
    const int gasMin = 0;
    const int gasMax = 500;

    const int co2Calibrate = 200;

public:
    float CO = 0, Alcohol = 0, CO2 = 0, Tolueno = 0, NH4 = 0, Acetona = 0;

    GasClass();
    bool begin();
    float read();
    void serialPrint();

    int getGasRelValue();
    int32_t getGasRelColor();
};

extern GasClass GASSENSOR;

#endif