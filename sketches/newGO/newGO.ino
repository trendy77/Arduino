// KITCHEN NODE HUE-MOTION SENSOR
// 4/7 - ADDED 5 SEC 'DELAY' BEFORE TURNING OFF LIGHT

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

unsigned long lastTime = 0;
WiFiClient serverClient;

unsigned long times=0;
WiFiClient client;
#define HOST_NAME "tnode187"        // Host mDNS
int status = WL_IDLE_STATUS;      // the Wifi radio's status
char bridge_ip[] = "192.168.0.101";  // IP address of the HUE bridge
const char *pathGet = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/5"; 
const char *pathPut = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/5/state"; 
const char ssid[] = "WiFi2";
const char pass[] = "4328646518";
const int port = 80;

#define pirPin D1
unsigned long stars=0;
unsigned long down=0;
    // Place to store read PIR Value
int pirState = 0; // we start, assuming no motion detected
int pirVal = LOW;        // variable for reading the pin status
int lightNo = 5;

void setup() {
 Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  pinMode(pirPin, INPUT);
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
   
    MDNS.addService("telnet", "tcp", 23);   // Initialize the telnet server of RemoteDebug
  Debug.begin(HOST_NAME); // Initiaze the telnet server
  Debug.setResetCmdEnabled(true); 
  Debug.showTime(true); 
    delay(10);
pinMode(pirPin, INPUT);
}

void brightenHue(int waiting, boolean loop){
  for (int i = 1; i<255;i+5){
sendRequest("bri", "i");
delay(waiting);
  }
 if (loop){
  doLoop();
  }
 }
 
void darkenHue(int waiting, boolean loop){
  for (int i = 255; i>0;i-5){
sendRequest("bri", "i");
delay(waiting);
  }
  if (loop){
  doLoop();
  }
}
void sendRequest(String cmd, String value) {
  String request = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
  request += lightNo;
  request += "/state HTTP/1.1";
  String contentType = "application/json";     /////  contentType="application/x-www-form-urlencoded";
  String hueCmd = "{\"on\":true}";
  //hueCmd += "true";
  //hueCmd += "}";
  if (Debug.isActive(Debug.INFO))   {
  Debug.print("Request to server: ");
  Debug.println(request);
  Debug.print("sending...");
   Debug.println(hueCmd);
   }
   Serial.print("Request to server: ");
  //Serial.println(request);
  client.connect(bridge_ip, 80);
   client.println("PUT "+ request);
 const char LIGHTS_ON[] = "{\"on\":true}";
  client.println("Content-type: text/xml; charset=\"utf-8\"");
  client.println("Content-Length: 11");
 client.println("");
  client.println(LIGHTS_ON); 
  client.println();
    while (client.available()) { 
    if (client.available()) {
  String line = client.readStringUntil('\r');
  Serial.print(line);
   if (Debug.isActive(Debug.INFO))   {
    Debug.print(line);
   } 
    }
  }
}

void sendRequestOff(String cmd, String value) {
  String request = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
  request += lightNo;
  request += "/state HTTP/1.1";
  String contentType = "application/json";     /////  contentType="application/x-www-form-urlencoded";
  if (Debug.isActive(Debug.INFO))   {
  Debug.print("Request to server: ");
  Debug.println(request);
  Debug.print("sending...");
 //  Debug.println(hueCmd);
   }
   Serial.print("Request to server: ");
Serial.println(request);
  client.connect(bridge_ip, 80);
const char LIGHTS_OFF[] = "{\"on\":false}";
const char EFFECT_NONE[] = "{\"effect\":\"none\"}";
client.println("PUT "+ request);
      client.println("Content-type: text/xml; charset=\"utf-8\"");
 client.println("Content-Length: 12");
 client.println("");
  client.print(LIGHTS_OFF);  
  client.println();
  // Serial.println(hueCmd); Serial.println("");
   while (client.available()) { 
    if (client.available()) {
  String line = client.readStringUntil('\r');
  Serial.print(line);
   if (Debug.isActive(Debug.INFO))   {
    Debug.print(line);
   } 
    }
  }
}

void doLoop(){
  const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";
}
void statCheck(int lightNo) {
  String request = "GET /api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
  request += lightNo;
  String contentType = "application/text";
  if (Debug.isActive(Debug.INFO))   {
  Debug.print("Request to server: ");
  Debug.println(request);
  Debug.print("sending...");
   }
   Serial.println("Request to server: ");
  Serial.println(request);
  Serial.println("");
  client.connect(bridge_ip, 80);
   client.println(request);
 client.println(contentType);
 client.println("");
  client.println("");
  while (client.available()) {
    if (client.available()) {
  String line = client.readStringUntil('\r');
  Serial.println(line);
   if (Debug.isActive(Debug.INFO))   {
    Debug.print(line);
   } 
    }
  }
}
long lastMove, timeSinceMove;

void motionCheck() {
 pirVal = digitalRead(pirPin); // read input value
  if (pirVal == HIGH) {        
     lastMove = millis();
    if (pirState == LOW) {         // we have just turned on
 sendRequest("on", "true");
 Serial.println("*NEW MOVE"); stars=0; stars++;down=0;
       pirState = HIGH; 
       } else if (pirState == HIGH){
      Serial.print("*"); stars++;
       } 
  } else if (pirVal == LOW){
  timeSinceMove = millis()- lastMove;
    if ((pirState == HIGH) && (timeSinceMove >=10000))  {
 sendRequestOff("on", "false");
 Serial.println("END - TURNING OFF LIGHT");
      pirState = LOW; stars = 0; down =0;down++;  
    } else if (pirState == LOW){
    down++;
    Serial.print("_"); 
    }
  timeSinceMove++;
  }
 delay(10);
}

void telnetMe(){
  String lastCmd = Debug.getLastCommand();
    if (lastCmd == "1") {
        if (Debug.isActive(Debug.INFO)) {
           brightenHue(10, false);
           Debug.println("brightenHue 10");
        }
    } else if (lastCmd == "2") {
        if (Debug.isActive(Debug.INFO)) {
         darkenHue(10, false);   Debug.println("*darken Hue 10");
        }
    }
if (lastCmd == "3") {
        if (Debug.isActive(Debug.INFO)) {
          brightenHue(50, true);   Debug.println("brightenHue 50");
        }
    } else if (lastCmd == "4") {
        if (Debug.isActive(Debug.INFO)) {
         darkenHue(50, true);   Debug.println("*darken Hue 50");
        }
    }
if (lastCmd == "5") {
        if (Debug.isActive(Debug.INFO)) {
       Debug.println("brightenHue 100");  brightenHue(100, true);    
        }
    } else if (lastCmd == "6") {
        if (Debug.isActive(Debug.INFO)) {
     Debug.println("*darken Hue 100");   darkenHue(100, true);    
        }
    }    
        if (Debug.isActive(Debug.INFO)) {
            Debug.printf("* done.");
        }
    }
    
void loop() {
motionCheck();
delay(100);
times = millis();
lightNo = 1;
if (times>5000){
statCheck(lightNo);
 
}
  telnetMe();
  
    // Give a time for ESP8266

    yield();
}
