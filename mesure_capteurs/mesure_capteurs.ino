#include "mesure_capteurs.h"
/* @brief
*
*/
Adafruit_BMP280 bmp;
Adafruit_CCS811 ccs;
int BH1750address = 0x23; //setting i2c address
byte buff[2];
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
int getLuxData(){
    int i=0;int val;
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