#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 256
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "bef3d1eb944e4e508e867e961d008e85";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Inception";
char pass[] = "dArKnIgHt@92922";

#define DHTPIN 4         // What digital pin we're connected to

#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  int timeSinceLastRead = 0;
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }
  // SETUP the ALARM Trigger and Send EMAIL 
  // and PUSH Notification
  if(t > 28){
    Blynk.email("deeps.jantz@gmail.com", "Food Computer - High Temperature Alert", "Temperature over 28C!");
    Blynk.notify("Food Computer 1  Alert- Temperature over 28C!");
  }
  if(t < 18){
    Blynk.email("deeps.jantz@gmail.com", "Food Computer - Low Temperature Alert", "Temperature below 18C!");
    Blynk.notify("Food Computer 1  Alert - Temperature below 18 C!");
  }
  if(h > 60 ){
    Blynk.email("deeps.jantz@gmail.com", "Food Computer - High Humdidity Alert", "Humidity over 60%");
    Blynk.notify("Food Computer 1  Alert - Humidity over 60%!");
  }
   if(h < 20 ){
    Blynk.email("deeps.jantz@gmail.com", "Food Computer - Low Humdidity Alert", "Humidity below 20%");
    Blynk.notify("Food Computer 1  Alert - Humidity below 20%!");
  }
}


void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(2500L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}