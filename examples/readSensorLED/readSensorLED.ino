/*
  Supported DEVIO NB-DEVKIT I Board 
    |  Do not use PIN   |
    |      16 TX        |
    |      17 RX        |
    |      4 EINT       |
    |   26 power key    |
    |     27 reset      |
  Pre required install library
         - ClosedCube_HDC1080
         
  This is an example for DEVIO NB-DEVKIT I. That has read sensor on board and report data to Magellan IoT Platform.
  
  Please login and enjoy with https://magellan.ais.co.th
  
  If you have any questions, please see more details at https://www.facebook.com/AISDEVIO
*/

#include "ClosedCube_HDC1080.h"
#include "Magellan_SIM7020E.h"

#include "AIS_SIM7020E_API.h"
//AIS_SIM7020E_API nb;

Magellan_SIM7020E magel;          
ClosedCube_HDC1080 hdc1080;

// Temp Humidity Light
const int lightSensorPin=34; 
String payload;

// LED on board
const int ledPin = 2;      //LED on board Pin 2
int ledStatus = 1;

// MQTT
String address    = "";               //Your IPaddress or mqtt server url
String serverPort = "";               //Your server port
String clientID   = "";               //Your client id < 120 characters
String topic      = "";               //Your topic     < 128 characters
String username   = "";               //username for mqtt server, username <= 100 characters
String password   = "";               //password for mqtt server, password <= 100 characters 
unsigned int subQoS       = 0;
unsigned int pubQoS       = 0;
unsigned int pubRetained  = 0;
unsigned int pubDuplicate = 0;


const long interval = 5000;           //time in millisecond 
unsigned long previousMillis = 0;
int cnt = 0;



void setup() 
{
  Serial.begin(115200);

  Serial.println("ClosedCube HDC1080");

  // Default settings:
  // - Heater off
  // - 14 bit Temperature and Humidity Measurement Resolutions
  hdc1080.begin(0x40);
   
  Serial.print("Manufacturer ID=0x");
  Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
  Serial.print("Device ID=0x");
  Serial.println(hdc1080.readDeviceId(), HEX);       // 0x1050 ID of the device
      
  magel.begin();                                     //Init Magellan LIB
  pinMode(ledPin,OUTPUT);

//  setupMQTT();
//  nb.setCallback(callback);   
//  previousMillis = millis();        
}

void blinkLED(){

    digitalWrite(ledPin,1); //Turn on LED
    delay(500);
    digitalWrite(ledPin,0); //Turn off LED
 
}

void loop() 
{
   /*
    Example read sensor on board and report data to Magellan IoT platform
  */
  String temperature=String(hdc1080.readTemperature());
  String humidity=String(hdc1080.readHumidity());
  String Light=String(analogRead(lightSensorPin));
  
  payload="{\"temperature\":"+temperature+",\"humidity\":"+humidity+",\"light\":"+Light+"}";  //Please provide payload with json format
  //Serial.println(temperature);
  blinkLED();
  magel.report(payload);   //Report sensor data
  delay(5000);  //Delay 5 second

  //sendDataMQTT(payload);
}


//=========== MQTT Function ================
//
//void sendDataMQTT(String payload){
//    nb.MQTTresponse();
//  unsigned long currentMillis = millis();
//  if (currentMillis - previousMillis >= interval) {
//        cnt++;
//        connectStatus();
//        nb.publish(topic,payload+String(cnt));      
////      nb.publish(topic, payload, pubQoS, pubRetained, pubDuplicate);      //QoS = 0, 1, or 2, retained = 0 or 1, dup = 0 or 1
//        previousMillis = currentMillis;  
//  }
//}
//
//void setupMQTT(){
//  if(!nb.connectMQTT(address,serverPort,clientID,username,password)){ 
//     Serial.println("\nconnectMQTT");
//  }
//    nb.subscribe(topic,subQoS);
////  nb.unsubscribe(topic); 
//}
//void connectStatus(){
//    if(!nb.MQTTstatus()){
//        if(!nb.NBstatus()){
//           Serial.println("reconnectNB ");
//           nb.begin();
//        }
//       Serial.println("reconnectMQ ");
//       setupMQTT();
//    }   
//}
//void callback(String &topic,String &payload, String &QoS,String &retained){
//  Serial.println("-------------------------------");
//  Serial.println("# Message from Topic \""+topic+"\" : "+nb.toString(payload));
//  Serial.println("# QoS = "+QoS);
//  if(retained.indexOf(F("1"))!=-1){
//    Serial.println("# Retained = "+retained);
//  }
//}
