#include <WiFi.h> // Cette bibliothèque permet à l'ESP32 de se connecter au réseau WiFi.
#include <PubSubClient.h> // Cette bibliothèque vous permet d'envoyer et de recevoir des messages MQTT.
#include "mesure_capteurs.ino"
#define EARTHPRESSUR (1013.25)

OLED myOLED(SDA, SCL, 8);
Adafruit_BME280 bme;
Adafruit_CCS811 ccs;
int BH1750address = 0x23;
const char* ssid = "iPhone (29)";
const char* password = "ABCDZFGH";
const char* mqtt_server = "mqtt.ci-ciad.utbm.fr";


WiFiClient espClientAir; // Initialiser la bibliothèque client wifi (connexion WiFi)
PubSubClient clientAir(espClientAir); // Créer un objet client MQTT (connexion MQTT)
long lastMsg = 0;

/**@brief Cette fonction initialise les paramètres pour la connexion WiFi et MQTT, configure le mode d'entrée et de sortie pour les broches de bouton et de LED, et démarre la communication série.
 **/
void setup() {
  Serial.begin(9600);

  myOLED.begin();
  Wire.begin();
  Serial.begin(57600);//init Serail band rate

  if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
    if(!ccs.begin()){
        Serial.println("Could not find a valid CCS811 sensor, check wiring!");
        while(1);
    }
    while(!ccs.available());

  delay(2000); // Donnez-moi le temps de mettre en place le moniteur série
  Serial.println("ESP32 Button Test");

  setup_wifi(); // Cette ligne appelle la fonction setup_wifi() pour configurer la connexion Wi-Fi sur l'ESP32.
  clientAir.setServer(mqtt_server, 1883); //définit le serveur (broker) MQTT à utiliser pour la communication et le port de communication à 1883, qui est le port standard pour MQTT.
  
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
      clientAir.subscribe("esp32/ledAir");
    } 
	// Si la connexion échoue, le code imprime "failed, rc=" suivi de l'état de la connexion avec "client.state()" sur la console série 
   // La fonction se répète jusqu'à ce que la connexion soit établie avec succès.
	else {
      Serial.print("failed, rc=");
      Serial.print(clientFraisse.state());
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
  
  //La dernière partie vérifie le temps écoulé depuis le dernier message publié et n'envoie le prochain message que toutes les 2 secondes (2000 millisecondes).
  long now = millis(); // Crée une variable "now" pour stocker le nombre de millisecondes écoulées depuis le démarrage du programme.
  if (now - lastMsg > 2000) { // Vérifie si le temps écoulé depuis le dernier message publié est supérieur à 2000 millisecondes.
		lastMsg = now; // Si oui, met à jour la variable "lastMsg" avec la valeur actuelle de "now".
		
		/*
        Code à remplir
    */
		
  }
}