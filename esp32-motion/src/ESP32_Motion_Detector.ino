#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#define USE_MQTT

int pirPin = 26;
int ledPin = 27;
int calibrationTime = 30;

// wifi creds
//ured
//#define ssid1        "Tehnoloski_park_Zagreb"
//#define password1    "TpZ232!Raza"

//stan
//#define ssid1        "blabla"
//#define password1    "hajduk1950"

// hotspot
//#define ssid1        "zvone"
//#define password1    "hajduk1950"
#define ssid1        "Vip WLAN_8BD492"
#define password1    "DBBDBBEBCE"
//qtt credentials
#ifdef USE_MQTT
  #define MQTT_SERVER      "192.168.0.101"
  #define MQTT_SERVERPORT  1883
  #define MQTT_USERNAME    "zvonimir"
  #define MQTT_KEY         "zadvarje"
  #define MQTT_MOTION      "iot/motion/1"
  #define MQTT_LED         "iot/led/1"

  #include "MQTTStuff.h"
#endif


WiFiMulti wifiMulti;
WiFiServer server(80);


void setup()
{

  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);


  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);


  Serial.begin(115200);
  wifiMulti.addAP(ssid1, password1);
  Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
  }

  int retry = 3;
  Serial.println(WiFi.localIP());

  MQTTConnect();

  server.begin();
  Serial.println("server begin");
}


void loop()
{
  if(digitalRead(pirPin) == HIGH) {
    publishLed.publish("TRUE");

    if(motionDetected.publish("Yes!"))
    {
      Serial.println("Motion detected, I am publishing it");
      digitalWrite(ledPin, HIGH);

    }
  }
  else {
    publishLed.publish("FALSE");
    digitalWrite(ledPin, LOW);

    motionDetected.publish("Nope.");

  }
  delay(4000);
}
