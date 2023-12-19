#include "mesure_capteurs.h"

Adafruit_BME280 bme;
Adafruit_CCS811 ccs;
#define SEALEVELPRESSURE_HPA (1013.25)

int getTVOCData(){
     int tvoc=ccs.getTVOC();
    Serial.print("TVOC");
    Serial.println(tvoc);
    return tvoc;
}
int getCO2Data(){
    if(ccs.available()){
      if(!ccs.readData()){ 
        int co2=ccs.geteCO2();
        Serial.print("CO2 ");
        Serial.println(co2);
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
