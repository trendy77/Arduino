// KITCHEN NODE HUE-MOTION SENSOR
// 4/7 - ADDED 5 SEC 'DELAY' BEFORE TURNING OFF LIGHT

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "RemoteDebug.h"
#include <IRrecv.h>

uint16_t RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

WiFiClient serverClient;
WiFiClient client;
#define HOST_NAME "tnode187"        // Host mDNS
RemoteDebug Debug;
int status = WL_IDLE_STATUS;      // the Wifi radio's status
const char ssid[] = "WiFi2";
const char pass[] = "4328646518";

void setup() {
 Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
     WiFi.hostname(HOST_NAME);
     WiFi.begin(ssid, pass);      
    while (WiFi.status() != WL_CONNECTED)    {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");    Serial.print("Connected to ");    Serial.println(ssid);
  Serial.print("IP address: ");    Serial.println(WiFi.localIP());
          // Register host name in WiFi and mDNS
  String hostNameWifi = HOST_NAME;
  hostNameWifi.concat(".local");
      if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 4998);   // Initialize the telnet server of RemoteDebug
  Debug.begin(HOST_NAME); // Initiaze the telnet server
  Debug.setResetCmdEnabled(true); 
  Debug.showTime(true); 
    delay(50);
 irrecv.enableIRIn();  // Start the receiver
  delay(10);
}

    
void loop() {
   digitalWrite(BUILTIN_LED, LOW);
  if (irrecv.decode(&results)) {
    digitalWrite(BUILTIN_LED, HIGH);
    // print() & println() can't handle printing long longs. (uint64_t)
    // So we have to print the top and bottom halves separately.
    if (results.value >> 32)
      Serial.print((uint32_t) (results.value >> 32), HEX);
      if (Debug.isActive(Debug.INFO)) {
       Debug.println((uint32_t) (results.value >> 32), HEX);    
        }
    Serial.println((uint32_t) (results.value & 0xFFFFFFFF), HEX);
     if (Debug.isActive(Debug.INFO)) {
       Debug.println((uint32_t) (results.value & 0xFFFFFFFF), HEX);    
        }
    irrecv.resume();  // Receive the next value
  }
  delay(100);
 telnetMe();
 
 Debug.handle();

    yield();
}





void telnetMe(){
  String lastCmd = Debug.getLastCommand();
    if (lastCmd == "1") {
        if (Debug.isActive(Debug.INFO)) {
              
        }
    } else if (lastCmd == "2") {
        if (Debug.isActive(Debug.INFO)) {
    
        }
    }
if (lastCmd == "3") {
        if (Debug.isActive(Debug.INFO)) {
      
        }
    } else if (lastCmd == "4") {
        if (Debug.isActive(Debug.INFO)) {
     
        }
    }
if (lastCmd == "5") {
        if (Debug.isActive(Debug.INFO)) {
       
        }
    } else if (lastCmd == "6") {
        if (Debug.isActive(Debug.INFO)) {
        }
    }    
        if (Debug.isActive(Debug.INFO)) {
            Debug.printf("* done.");
        }
    }
