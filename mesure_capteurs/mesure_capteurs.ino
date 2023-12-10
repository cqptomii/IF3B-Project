#include "mesure_capteurs.h"
/* @brief
*
*/
#define bLux_pin 21
Adafruit_BMP280 bmp;
Adafruit_CCS811 ccs;
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
void getBM280Data(float *pressure,float *temp){

  *pressure=bmp.readPressure();
  *temp=bmp.readTemperature();
  Serial.print(F("Pressure (Pa / mB): "));
  Serial.print(*pressure);
  Serial.println(F(" mB"));
  Serial.print(F("Temperature: "));
  Serial.print(*temp);
  Serial.println(F(" °C"));
  Serial.print(F("humidité: "));
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
