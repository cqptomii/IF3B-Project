#include <WiFi.h> 
#include <PubSubClient.h> 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "mesure_capteurs.h"
#include <string>
// constantes
#define Largeur 128         
#define Hauteur 32        
#define nbPixel 8
#define brocheReset  -1     
#define adresseI2C 0x3C
#define EARTHPRESSUR (1013.25)
// Pin
#define bLux_pin 21
#define bLed_pin 18
#define ventil_pin 18
#define reset_pin 13
#define buzzer_pin 19

float pressure=0,temp=0;
int tvoc_value=0;
int co2_value=0;
float hum_value=0.0;
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
bool config_temp=true;
bool config_lumiere=true;
bool config_tvoc=true;
bool config_co2=true;

void UpdateOLED(float Temp,float Hum,int co2,int tvoc,float pression,float lux){
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0,0);
    oled.setTextColor(SSD1306_WHITE);
    oled.print(10);
    oled.print((char)247);
    oled.print("C");
    oled.print("  Lumiere: ");
    oled.print(10);
    oled.println("lux");
    oled.print("Alt : ");
    oled.print(0);
    oled.print("m - ");
    oled.print(1023);
    oled.println("pa");
    oled.print("Co2 : ");
    if(co2>1000){
      oled.println("Dangereux");
    }
    else{
      oled.println("Correct");
    }
    oled.print("Tvoc : ");
    if(tvoc>500){
      oled.println("Dangereux");
    }
    else{
      oled.println("Correct");
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

void setup() {
  Serial.begin(9600);
  // Bouton reset setup
  pinMode(reset_pin,INPUT_PULLUP);
  //  bmp280 setup
  while (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    delay(1000);
  }
  Serial.println(F("Reading BMP280 : "));
  //Ecran setup
  if(!oled.begin(SSD1306_SWITCHCAPVCC, adresseI2C)){
    Serial.print("Erreur de communication");
    for(;;);
  }
  Serial.print(F("communication ecran réussi "));
  oled.display();

  // luxmetre setup
  pinMode(bLux_pin,INPUT);

  // Led setup
  pixels.begin();
  safe_led();
  // ventil setup
  pinMode(ventil_pin,OUTPUT);
  // buzzer setup 
  pinMode(buzzer_pin,OUTPUT);

  delay(2000); 
  setup_wifi();
  init_ventile();
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
      clientAir.subscribe("esp32/Mode/temperature");
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
  // bouton reinitialisation
  if(analogRead(reset_pin)==LOW){
    clientAir.publish("esp32/buttonResetOption","1");
  }
  // timer pour effectuer les mesure ( de base 5 minutes)
  long now = millis();
  if (now - lastMsg > 2000) {
		lastMsg = now;
    // mesure BMP280
    if(config_temp){
      getBM280Data(&pressure,&temp);
      memcpy(convertion,temp,10);
      clientAir.publish("esp32/bmp280/temperature",convertion);
      memcpy(convertion,pressure,10);
      clientAir.publish("esp32/bmp280/pression",convertion);
    }
		//mesure CCS811
    if(config_co2){
      co2_value=getCO2Data();
      memcpy(convertion,co2_value,10);
      clientAir.publish("esp32/css811/qualiteCO2",convertion);
    }
    if(config_tvoc){
      tvoc_value=getTVOCData();
      memcpy(convertion,tvoc_value,10);
      clientAir.publish("esp32/css811/qualiteTVOC",convertion);
    }
    //Mesure luxmetre
    if(config_lumiere){
      lux_value= getLuxData();
      memcpy(convertion,lux_value,10);
      clientAir.publish("esp32/css811/luminositePiece",convertion);
    }
    // Affichage erreur liés au co2
    if(r_co2){
      danger_led();
      digitalWrite(buzzer_pin,LOW);
    }
    // Affichage erreur lies au tvoc
    if(r_tvoc){
      danger_led();
      digitalWrite(buzzer_pin,LOW);
    }
    //Affichage erreur lié a la temperature
    if(r_temp){
      danger_led();
      digitalWrite(buzzer_pin,LOW);
    }
    //Affichage erreur lié a la pression
    if(r_pression){
      danger_led();
      digitalWrite(buzzer_pin,LOW);
    }
    //Affichage oled
    UpdateOLED(temp_value,hum_value,co2_value,tvoc_value,pressure_value,lux_value);
    if(r_pression==false && r_temp==false && r_pression==false && r_co2==false && r_tvoc==false){
      safe_led();	
    }
    //publication des valeurs sur node-red
  }
}