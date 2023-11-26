#include "mesure_capteurs.h"
void UpdateOLED(){

}
uint16_t getCSS811Data(){
    int BH1750address = 0x23; //setting i2c address
    int i=0;uint16_t val;
    byte LuxData[2];
    Wire.beginTransmission(BH1750address);
    Wire.write(0x10);//1lx reolution 120ms
    Wire.endTransmission();

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
    }
    return val;
}

