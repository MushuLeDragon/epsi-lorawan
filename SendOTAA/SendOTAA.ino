#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED00175E5";
const char *appKey = "C57D0773BE247FF26966F7FF210939BB";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

/** Broche "DATA" du capteur */
const byte BROCHE_CAPTEUR = 7;

/* Code d'erreur de la fonction readDHT11() et readDHT22() */
const byte DHT_SUCCESS = 0;        // Pas d'erreur
const byte DHT_TIMEOUT_ERROR = 1;  // Temps d'attente dépassé
const byte DHT_CHECKSUM_ERROR = 2; // Données reçues erronées

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  Serial.println(F("Demo DHT11 et DHT22"));

  pinMode(BROCHE_CAPTEUR, INPUT_PULLUP);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  debugSerial.println("-- LOOP");
  float temperature, humidity;
  
  /* Lecture de la température et de l'humidité, avec gestion des erreurs */
  // N.B. Remplacer readDHT11 par readDHT22 en fonction du capteur utilisé !
  switch (readDHT11(BROCHE_CAPTEUR, &temperature, &humidity)) {
  case DHT_SUCCESS: 
     
    /* Affichage de la température et du taux d'humidité */
    Serial.print(F("Humidite (%): "));
    Serial.println(humidity, 2);
    Serial.print(F("Temperature (^C): "));
    Serial.println(temperature, 2);
    break;
 
  case DHT_TIMEOUT_ERROR: 
    Serial.println(F("Pas de reponse !")); 
    break;
 
  case DHT_CHECKSUM_ERROR: 
    Serial.println(F("Pb de communication !")); 
    break;
  }

  // Prepare payload of 1 byte to indicate LED status
  byte payload[1];
  payload[0] = (digitalRead(LED_BUILTIN) == HIGH) ? 1 : 0;

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}
