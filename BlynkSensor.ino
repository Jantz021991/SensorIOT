#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 256
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h> 


// Auth Token  is from the Blynk App.
char auth[] = "bef3d1eb944e4e508e867e961d008e85";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "***********";
char pass[] = "************";
int sensor = 0;
int lastState = 0;

#define DHTPIN 4         // Digital pin as connected to DHT22 sensor
#define ONE_WIRE_BUS 2  // Digital pin as connected for DS18B20 sensor
#define DHTTYPE DHT22  

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  sensors.begin();    
  sensors.requestTemperatures();                  // Polls the sensors.
  sensors.setResolution(10);   
  float c =  sensors.getTempCByIndex(0);   
  
  int timeSinceLastRead = 0;
  if (isnan(h) || isnan(t) || isnan(c) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V1, c);
  
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
  
  //Sensor to check the water Level 
   sensor=analogRead(A0);
  if (sensor < 300 && lastState == 0) {
   Blynk.email("deeps.jantz@gmail.com", "Low Water Level", "Humidity over 60%");
   Blynk.notify("Food Computer 1  Alert - Low Water Level!");
   lastState = 1;
   delay(1000);
  }
  else if (sensor < 300 && lastState == 1) {
   delay(1000);
   else {
    //State
    lastState = 0;
    delay(1000);
  }
  
  delay(100);
  }
}
