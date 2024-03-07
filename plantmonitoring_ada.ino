#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


//WiFi Access Point

#define ssid  "Galaxy"
#define passwd  "12345678"

//Adafruit.io Setup

#define AIO_SERVER  "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Shivang1995"
#define AIO_KEY "aio_hRck89APlrOLg1kUTNqOoC0AW0KZ"


//Moisture Sensor Setup

#define moisturepin A0

//Adafruit.io Feeds
#define MOISTURE_FEED "Moisture Level" 


//sketch code
//WiFi Client
WiFiClient client;

//Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//Initialise the MQTT moisture feeds
Adafruit_MQTT_Publish moisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/" MOISTURE_FEED);

void setup() {
  Serial.begin(115200);
  delay(10);

  //Connect to WiFi 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to:");
  Serial.println(ssid);

  WiFi.begin(ssid, passwd);

  while(WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected");

}

  void loop(){

  // Ensure the connection to the MQTT server is alive (heartbeat)
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.processPackets(10000);

 //Read moisture level from sensor
 int moistureLevel = analogRead(moisturepin);

 // Publish moisture level to Adafruit IO
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel);
  moisture.publish(moistureLevel);
 //Delay between readings
 delay(3000); 
}
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Connecting to MQTT... ");
    // Attempt to connect
    if (mqtt.connect() != 0) {
      Serial.print("Failed to connect. Retry in 5 seconds...");
      delay(5000);
    }
  }
}
