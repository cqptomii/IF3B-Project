#include "mesure_capteurs.h"
/* @brief
*
*/
void UpdateOLED(float Temp,float Hum,uint16_t co2,uint16_t tvoc,float pression,float lux){
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.print("Temperature : ");
    oled.print(Temp);
    oled.print((char)247);
    oled.println("C");
    oled.print("Humidité : ");
    oled.print(Hum);
    oled.println("%");
    oled.print("Pression : ");
    oled.print(pression);
    oled.println("pa");
    oled.print("Luminosité : ");
    oled.print(lux);
    oled.println("lux");
    oled.print("Taux Co2 : ");
    if(co2>1000){
      oled.println("Dangereux");
    }
    else{
      oled.println("Correct");
    }
    oled.print("Taux Tvoc : ");
    if(tvoc>500){
      oled.println("Dangereux");
    }
    else{
      oled.println("Correct");
    }
    oled.display();
}
uint16_t getCO2Data(){
    if(ccs.available()){
      if(!ccs.readData()){ 
        uint16_t co2=ccs.geteCo2();
        Serial.print("CO2");
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
uint16_t getTVOCData(){
    if(ccs.available()){
      if(!ccs.readData()){ 
        uint16_t tvoc=ccs.geteTvoc();
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
void getBM280Data(float *pressure,float *temp,int *hum){

  pressure=bmp.readPressure();
  temp=bmp.readTemperature();
  hum=bmp.readHumidity();
}
uint16_t getLuxData(){
    int i=0;uint16_t val;
    byte LuxData[2];
    
    Wire.beginTransmission(BH1750address);
    Wire.requestFrom(BH1750address, 2);
    while(Wire.available()) //
    {
        LuxData[i] = Wire.read();  // receive one byte
        i++;
    }
    Wire.endTransmission();

    if(i==2){
        val=((buff[0]<<8)|buff[1])/1.2;
        Serial.print(val,DEC);
        Serial.println("[lx]");
        return val;
    }
    return 0;
}