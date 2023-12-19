//
// Created by tomfr on 25/11/2023.
//

#ifndef PROJETIF3B_MESUR_CAPTEURS_H
#define PROJETIF3B_MESUR_CAPTEURS_H
#include <Wire.h>
#include <math.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_SSD1306.h>
int getCO2Data();
void getBM280Data(float *pressure,float *temp,float *humi,float *altitude);
int getTVOCData();
#endif //PROJETIF3B_MESUR_CAPTEURS_H
