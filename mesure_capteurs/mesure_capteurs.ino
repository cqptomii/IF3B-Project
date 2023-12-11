#include "mesure_capteurs.h"
/* @brief
*
*/
#define bLux_pin 21
Adafruit_BME280 bme;
Adafruit_CCS811 ccs;
#define SEALEVELPRESSURE_HPA (1013.25)

byte LuxData[2];
int getTVOCData(){
    if(ccs.available()){
      if(!ccs.readData()){ 
        int tvoc=ccs.getTVOC();
        Serial.print("TVOC");
        Serial.print(tvoc);
        return tvoc;
      }
      else{
        Serial.println("Error");
      }
    }
    else{
      Serial.println("Error ccs unavailable ");
    }
    return 0;
}
int getCO2Data(){
    if(ccs.available()){
      if(!ccs.readData()){ 
        int co2=ccs.geteCO2();
        Serial.print("CO2 ");
        Serial.print(co2);
        return co2;
      }
      else{
        Serial.println("Error");
      }
    }
    else{
      Serial.println("Error ccs unavailable ");
    }
    return 0;
}
void getBM280Data(float *pressure,float *temp,float *humi,float *altitude){

  *pressure=bme.readPressure() /100.0;
  *temp=bme.readTemperature();
  *humi=bme.readHumidity();
  *altitude=bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Temperature = ");
	Serial.print(bme.readTemperature());
	Serial.println("°C");
  Serial.print(F("Pressure : "));
  Serial.print(*pressure);
  Serial.println(F(" hpa"));
  Serial.print("Approx. Altitude = ");
	Serial.print(*altitude);
	Serial.println("m");
  Serial.print("Humidity = ");
	Serial.print(bme.readHumidity());
	Serial.println("%");
}
float getLuxData(){
    int i=0;int val;
    
    unsigned int A0_value= analogRead(bLux_pin);
    if(A0_value){
        float voltage= A0_value *(5/1023);
	      float current= voltage*(-1,6)*pow(10,6);
        float Light= current/0.075;
        return Light;
    }
    else{
      return -1;
    }
}
