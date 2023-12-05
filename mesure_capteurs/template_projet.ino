#include <WiFi.h> 
#include <PubSubClient.h> 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "mesure_capteurs.h"
#define Largeur 128         
#define Hauteur 32        
#define brocheReset  -1     
#define adresseI2C 0x3C
#define EARTHPRESSUR (1013.25)
#define bLux_pin 21
#define bLed_pin 18
#define ventil_pin 18
#define nbPixel 8

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

void setup() {
  Serial.begin(9600);

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

//Si un message MQTT (variable messageTemp) est reçu sur le Topic "esp32/led", vous vérifiez si le message est soit "on" ou "off".
//Ensuite, vous modifiez l'état de sortie de la LED physique en fonction du message "messageTemp" reçu en utilisant la fonction digitalWrite.

/*
  Code à remplir
*/
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
      // Subscribe
      clientAir.subscribe("esp32/bmp280/temperature");
      clientAir.subscribe("esp32/bmp280/pression");
      clientAir.subscribe("esp32/bmp280/hauteur");
      clientAir.subscribe("esp32/css811/luminositePiece");
      clientAir.subscribe("esp32/css811/qualiteCO2");
      clientAir.subscribe("esp32/css811/qualiteTVOC");
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
  
  // timer pour effectuer les mesure ( de base 5 minutes)
  long now = millis();
  if (now - lastMsg > 2000) {
		lastMsg = now;
    // mesure BMP280
    getBM280Data(&pressure,&temp);
		//mesure CCS811
    co2_value=getCO2Data();
    tvoc_value=getTVOCData();
    //Mesure luxmetre
    lux_value= getLuxData();
    //Affichage oled
    UpdateOLED(temp_value,hum_value,co2_value,tvoc_value,pressure_value,lux_value);	
    //publication des valeurs sur node-red
  }
}

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
void end_ventile(){
  analogWrite(ventil_pin,0);
}
void danger_led(){
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