#include <WiFi.h> 
#include <PubSubClient.h> 
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include "mesure_capteurs.h"
#include <string>
// constantes
#define Largeur 128         
#define Hauteur 32        
#define nbPixel 8
#define brocheReset  -1     
#define adresseI2C 0x3C
int BH1750address = 0x23;
// Pin
#define bLed_pin 27
#define ventil_pin 19


#define reset_pin 4
#define buzzer_pin 26

int tvoc_value=0;
int co2_value=0;
float altitude_value=0;
float humi_value=0.0;
uint8_t buf[4] = {0};
uint16_t data, data1;
float lux_value=0.0;
float pressure_value=0.0;
float temp_value=0.0;

const char* ssid = "iPhone (29)";
const char* password = "ABCDZFGH";
const char* mqtt_server = "mqtt.ci-ciad.utbm.fr";

Adafruit_NeoPixel pixels=Adafruit_NeoPixel(nbPixel, bLed_pin, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 oled(Largeur, Hauteur, &Wire, brocheReset);

WiFiClient espClientAir; 
PubSubClient clientAir(espClientAir); 
long lastMsg = 0;

 // variable de configuration + Risques
bool r_co2=false;
bool r_tvoc=false;
bool r_pression=false;
bool r_temp=false;
bool r_humi=false;
bool config_temp=true;
bool config_lumiere=true;
bool config_tvoc=true;
bool config_co2=true;

void setup() {
  Serial.begin(9600);
  Serial.println("deds");
  //  bmp280 setup
  if(!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while(1);
	}
  Serial.println(F("Reading BME280 : "));
   // ccs811 setup
  if(!ccs.begin(0x76)){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }
  Serial.println(F("Reading CCS811 : "));
  while(!ccs.available());
  //Ecran setup
  if(!oled.begin(SSD1306_SWITCHCAPVCC, adresseI2C)){
    Serial.print("Erreur de communication");
  }
  Serial.print(F("communication ecran réussi "));
  oled.display();

  // luxmetre setup
  Wire.begin()
  // Bouton reset setup
  pinMode(reset_pin,INPUT_PULLUP);
  // Led setup
  pixels.begin();
  safe_led();
  // ventil setup
  pinMode(ventil_pin,OUTPUT);
  // buzzer setup 
  pinMode(buzzer_pin,OUTPUT);
  init_ventile();


  delay(2000); 
  setup_wifi();
  clientAir.setServer(mqtt_server, 1883); 
  clientAir.setCallback(callback); // définit la fonction callback comme la fonction à appeler lorsqu'un message MQTT est reçu.
}


/** @brief Cette fonction se connecte au réseau WiFi en utilisant les paramètres de connexion fournis dans les variables ssid et password.
**/
void setup_wifi() {
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println(); // Imprime une ligne vide/saut de ligne dans la console série.
  Serial.print("Connecting to ");
  Serial.println(ssid); 

  WiFi.begin(ssid, password); // //démarre la connexion Wi-Fi avec les informations de connexion (SSID et mot de passe) fournies.

//Cette boucle effectue une pause de 500 millisecondes jusqu'à ce que l'ESP32 soit
//connecté au réseau Wi-Fi. Le statut de la connexion est obtenu en appelant "WiFi.status()".
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



/** @brief La fonction callback est utilisée pour traiter les messages MQTT reçus par l'ESP32 et de prévoir une action Elle est appelée chaque fois qu'un nouveau message est reçu sur un Topic auquel l'ESP32 est abonné.
**/
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic); // imprime le nom du Topic sur lequel le message a été publié.
  Serial.print(". Message: ");
  
  // Le message reçu est transmis à la fonction en tant que tableau de bytes "message" avec une longueur "length". 
  //Pour pouvoir travailler avec ce message dans le reste du code, nous devons d'abord le convertir en chaîne de caractères.
  String messageTemp;

  // boucle sur chaque élement dans le tableau de bytes "message" et les ajoute à la chaîne "messageTemp".
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
		//Pour chaque itération, l'élément actuel du tableau de bytes "message" est converti en char avec "(char)message[i]" et affiché sur la console série avec "Serial.print((char)message[i])".
	  //Ensuite, ce même élément est ajouté à la fin de la chaîne "messageTemp" avec
	  //"messageTemp += (char)message[i];
  }
  Serial.println();
  if(messageTemp=="C"){ //Risque CO2
    r_co2=true;
  }
  if(messageTemp=="T") {// Risque TVOC
    r_tvoc=true;
  }
  if(messageTemp=="P"){ // Risque Pression
    r_pression=true;
  }
  if(messageTemp=="K"){ // Risque Temperature
    r_temp=true;
  }
  if(messageTemp=="H"){ // Risque Humidité
    r_temp=true;
  }
  if(messageTemp=="c"){
    r_co2=false;
  }
  if(messageTemp=="t"){
    r_tvoc=false;
  }
  if(messageTemp=="p"){
    r_pression=false;
  }
  if(messageTemp=="k"){
    r_temp=false;
  }
  if(messageTemp=="h"){
    r_humi=false;
  }
  if(messageTemp=="ONTVOC"){ // mesure TVOC
    config_tvoc=true;
  }
  if(messageTemp=="ONCO2"){ // mesure CO2
    config_co2=true;
  }
  if(messageTemp=="ONlumiere"){ // mesure pression / altitude
    config_lumiere=true;
  }
  if(messageTemp=="ONtemp"){ // mesure temperature
    config_temp=true;
  }
  if(messageTemp=="OFFTVOC"){ // mesure TVOC
    config_tvoc=false;
  }
  if(messageTemp=="OFFCO2"){ // mesure CO2
    config_co2=false;
  }
  if(messageTemp=="OFFlumiere"){ // mesure pression / altitude
    config_lumiere=false;
  }
  if(messageTemp=="OFFtemp"){ // mesure temperature
    config_temp=false;
  }
}


/** @brief La fonction "reconnect()" est utilisée pour garantir la connexion MQTT entre l'ESP32 et le broker MQTT. Elle est appelée dans la boucle principale et elle se répète jusqu'à ce que la connexion soit établie avec succès.
**/
void reconnect() {
  // Loop until we're reconnected
  while (!clientAir.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
	// Si la connexion est déjà établie, la fonction ne fait rien. 
	//Si la connexion n'est pas établie, le code essaie de se connecter à l'aide de "client.connect("espClient")".
   //Si la connexion est réussie, le code imprime "connected" sur la console série et s'abonne au topic "esp32/led" avec "client.subscribe("esp32/led")".
    if (clientAir.connect("espClientAir")) {
      Serial.println("connected");
      clientAir.subscribe("esp32/bmp280/temperature");
      clientAir.subscribe("esp32/Mode/pression");
      clientAir.subscribe("esp32/Mode/gaz/TVOC");
      clientAir.subscribe("esp32/Mode/gaz/CO2");
      clientAir.subscribe("esp32/bandeauled/danger");
    } 
	// Si la connexion échoue, le code imprime "failed, rc=" suivi de l'état de la connexion avec "client.state()" sur la console série 
   // La fonction se répète jusqu'à ce que la connexion soit établie avec succès.
	else {
      Serial.print("failed, rc=");
      Serial.print(clientAir.state());
      Serial.println(" try again in 5 seconds");
      // On attend 5 secondes avant de retenter la connexion
      delay(5000);
    }
  }
}

void loop() { 
	// La première tâche de la fonction principale est de vérifier si le client MQTT est connecté. 
	//Si ce n'est pas le cas, la fonction reconnect() est appelée pour reconnecter le client.
  if (!clientAir.connected()) {
    reconnect();
  }
  
  clientAir.loop(); // La méthode client.loop() est appelée pour traiter les messages MQTT entrants.
				// Maintient la connexion avec le serveur MQTT en vérifiant si de nouveaux messages sont arrivés et en envoyant les messages en attente.
  
  
  char convertion[10];

// Bouton de réinitialisation
  if (analogRead(reset_pin) == LOW) {
  clientAir.publish("esp32/buttonResetOption", "1");
  }

  // Minuteur pour effectuer les mesures (de base 5 minutes)
  long now = millis();
  if (now - lastMsg > 50000) {
    lastMsg = now; 

    // Mesure BMP280
    if (config_temp) {
      getBM280Data(&pressure_value, &temp_value,&humi_value,&altitude_value);

      snprintf(convertion, sizeof(convertion), "%.2f", temp_value);
      clientAir.publish("esp32/bmp280/temperature", convertion);

      snprintf(convertion, sizeof(convertion), "%.2f", pressure_value);
      clientAir.publish("esp32/bmp280/pression", convertion);

      snprintf(convertion, sizeof(convertion), "%.2f", humi_value);
      clientAir.publish("esp32/bmp280/Humidité", convertion);

      snprintf(convertion, sizeof(convertion), "%.2f", altitude_value);
      clientAir.publish("esp32/bmp280/hauteur", convertion);
    }

    // Mesure CCS811
    if (config_co2) {
      int co2_value = getCO2Data();
      snprintf(convertion, sizeof(convertion), "%d", co2_value);
      clientAir.publish("esp32/css811/qualiteCO2", convertion);
    }

    // Mesure TVOC
    if (config_tvoc) {
      int tvoc_value = getTVOCData();
      snprintf(convertion, sizeof(convertion), "%d", tvoc_value);
      clientAir.publish("esp32/css811/qualiteTVOC", convertion);
    }

    // Mesure luxmètre
    if (config_lumiere) {
      readReg(0x10, buf, 2);              //Register Address 0x10
      data = buf[0] << 8 | buf[1];
      lux_value = (((float)data )/1.2);
      Serial.print("LUX:");
      Serial.print(lux_value);
      Seritemperatureal.print("lx");
      Serial.print("\n");
      snprintf(convertion, sizeof(convertion), "%.2f", lux_value);
      clientAir.publish("esp32/css811/luminositePiece", convertion);
    }
  }

  // Affichage erreur liés au co2
  if(r_co2){
    danger_led();
    digitalWrite(buzzer_pin,HIGH);
  }
  // Affichage erreur lies au tvoc
  if(r_tvoc){
    danger_led();
    digitalWrite(buzzer_pin,HIGH);
  }
  //Affichage erreur lié a la temperature
  if(r_temp){
    danger_led();
    digitalWrite(buzzer_pin,HIGH);
  }
  //Affichage erreur lié a la pression
  if(r_pression){
    danger_led();
    digitalWrite(buzzer_pin,HIGH);
  }
  if(r_humi){
    danger_led();
    digitalWrite(buzzer_pin,HIGH);
  }
  //Affichage oled
  UpdateOLED(temp_value,humi_value,co2_value,tvoc_value,pressure_value,lux_value,altitude_value);
  if(r_pression==false && r_temp==false && r_pression==false && r_co2==false && r_tvoc==false){
    safe_led();	
  }
}
void UpdateOLED(float Temp,float Hum,int co2,int tvoc,float pression,float lux,float altitude){
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0,0);
    oled.setTextColor(SSD1306_WHITE);
    oled.print(Temp);
    oled.print((char)247);
    oled.print("C");
    oled.print("  Hum:");
    oled.print(Hum);
    oled.println("%");
    oled.print("Lumiere:");
    oled.print(lux);
    oled.println("lux");
    oled.print("Alt:");
    oled.print(altitude_value);
    oled.print("m-");
    oled.print(pression);
    oled.println("pa");
    oled.print("Co2:");
    if(co2>1000){
      oled.print("Dangereux");
    }
    else{
      oled.print("Correct");
    }
    oled.print("Tvoc:");
    if(tvoc>500){
      oled.print("Dangereux");
    }
    else{
      oled.print("Correct");
    }
    oled.display();
}
void init_ventile(){
  digitalWrite(ventil_pin,HIGH);
}
void danger_led(){
  digitalWrite(buzzer_pin,HIGH);
  for (int i = 0; i < nbPixel; i++) {
    pixels.setPixelColor(i, pixels.Color(255,0,0));
  }
  pixels.show();
  delay(500);
  for (int i = 0; i < nbPixel; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
  delay(500);
}
void safe_led(){
  for (int i = 0; i < nbPixel; i++) {
    pixels.setPixelColor(i, pixels.Color(0,255,0));
  }
  pixels.show();
}
void oled_error(bool r_co2,bool r_pression, bool r_temp, bool r_tvoc, bool r_humi){
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0,0);
  oled.setTextColor(SSD1306_WHITE);
  if(r_co2){
    oled.println("Danger CO2 , Aerer la pièce imediatement!! ");
  }
  if(r_humi){
    oled.println("Humidité importante, aérer la pièce ! ");
  }
  if(r_pression){
    oled.println("Danger, Pression trop importante !! ");
  }
  if(r_temp){
    oled.println("Attention température trop élevée !");
  }
  if(r_tvoc){
    oled.println("Danger TVOC , Aerer la pièce imediatement !! ");   
  }
  oled.display();
}
uint8_t readReg(uint8_t reg, const void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    Serial.println("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  Wire.beginTransmission(address);
  Wire.write(&reg, 1);
  if ( Wire.endTransmission() != 0) {
    return 0;
  }
  delay(20);
  Wire.requestFrom(address, (uint8_t) size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  return size;
}
