#include "gasSensor.h"

/**
 * Constructor
 */
GasClass::GasClass()
{
}

/**
 * initialise Gas-sensor
 * @return {bool} success
 */
bool GasClass::begin()
{
  mq135 = new MQUnifiedsensor("ESP-32", 5.0, 12, analogGasPin, "MQ-135");
  mq135->setRegressionMethod(1);
  mq135->init();

  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++)
  {
    mq135->update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += mq135->calibrate(3.6);
    Serial.print(".");
  }
  mq135->setR0(calcR0 / 10);
  Serial.println(" done!.");

  if (isinf(calcR0))
  {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
    return false;
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
    return false;
  }

  return true;
}

/**
 * reads sensor values and save in variables
 * and returns CO2 value
 * @return {float} CO2
 */
float GasClass::read()
{
  mq135->update();

  mq135->setA(605.18);
  mq135->setB(-3.937);      // Configurate the ecuation values to get CO concentration
  CO = mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  mq135->setA(77.255);
  mq135->setB(-3.18);            // Configurate the ecuation values to get Alcohol concentration
  Alcohol = mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  mq135->setA(110.47);
  mq135->setB(-2.862);       // Configurate the ecuation values to get CO2 concentration
  CO2 = co2Calibrate + mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  mq135->setA(44.947);
  mq135->setB(-3.445);           // Configurate the ecuation values to get Tolueno concentration
  Tolueno = mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  mq135->setA(102.2);
  mq135->setB(-2.473);       // Configurate the ecuation values to get NH4 concentration
  NH4 = mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  mq135->setA(34.668);
  mq135->setB(-3.369);           // Configurate the ecuation values to get Acetona concentration
  Acetona = mq135->readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  return CO2;
}

/**
 * creates a Serial print with all data
 * @return void
 */
void GasClass::serialPrint()
{
  read();
  Serial.print("|   ");
  Serial.print(CO);
  Serial.print("   |   ");
  Serial.print(Alcohol);
  Serial.print("   |   ");
  Serial.print(CO2); // + 400 cause it's always there ?!
  Serial.print("   |   ");
  Serial.print(Tolueno);
  Serial.print("   |   ");
  Serial.print(NH4);
  Serial.print("   |   ");
  Serial.print(Acetona);
  Serial.println("   |");
}

/**
 * calculates scale position 
 * of gas between 0 and 10
 * @return {int} scalePosition
 */
int GasClass::getGasRelValue()
{
  float relative = 10.0 / float(gasMax - gasMin);
  relative *= read() - gasMin;
  return round(relative);
}

/**
 * color of gas on the scale 
 * @return {int32_t} hexColor (24 Bit rgb)
 */
int32_t GasClass::getGasRelColor()
{
  switch (getGasRelValue())
  {
  case 0:
    return 0x0AFA0A;
  case 1:
  case 2:
    return 0xE2FA0A;
  case 3:
  case 4:
    return 0xFFB30A;
  case 5:
  case 6:
    return 0xFF5F0A;
  case 7:
  case 8:
    return 0xCC0A0A;
  case 9:
  case 10:
    return 0x4D0A0A;
  default:
    return 0xFFFFFF; // white
  }
}

GasClass GASSENSOR = GasClass();