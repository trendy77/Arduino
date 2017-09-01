#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <IRLib.h>

IRrecv My_Receiver(A0);

IRTYPES codeType;          // The type of code
unsigned long codeValue;   // The data bits if type is not raw
int codeBits;              // The length of the code in bits
unsigned int rawCodes[RAWBUF]; // The durations if raw
int rawCount; bool GotOne, GotNew; IRdecode My_Decoder; 

const char ssid[] = "WiFi2";
const char pass[] = "4328646518";
const int port = 80;
#define HOST_NAME "ir" 
WiFiClient client;
unsigned long currentTimestamp = 0;
unsigned long httpTimestamp = 0;
unsigned long ramTimestamp;

void setup() {
 WiFi.hostname(HOST_NAME);
     WiFi.begin(ssid, pass); 
 while (WiFi.status() != WL_CONNECTED)    {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected");Serial.println(ssid); Serial.print("IP address: ");    Serial.println(WiFi.localIP());
 My_Receiver.enableIRIn(); // Start the receiver
 }
unsigned long loop=0;

void loop() {
 currentTimestamp = millis();
 loops++;

if (loops > 300){
  Serial.println(".");
  loops =0; 
}
      
if (My_Receiver.GetResults(&My_Decoder)) {
      IRDetected();
      delay(100);
      My_Receiver.resume();
    }
 
 }


void sendIR(String command){
  String request = "/api/fRgcNsvxh3ytQKVUZlCso0KbAn7zOlMhtkVmwzQG/lights/";
  request += lightNo;
  request += " HTTP/1.1";
     client.connect(bridge_ip, 80);
   client.println("PUT "+ request);
 const char LIGHTS_ON[] = "{\"on\":true}";
  client.println("Content-type: text/xml; charset=\"utf-8\"");
  client.println("Content-Length: 11");
 client.println("");
  client.println(command); 
  client.println();
    while (client.available()) { 
    if (client.available()) {
  String line = client.readStringUntil('\r');
  Serial.print(line);
      }
    }else {
    Serial.println("connection failed");
  }
}

void IRDetected(){
  My_Decoder.decode(); GotOne=true;
  GotNew=true;  codeType = My_Decoder.decode_type;  if (codeType == UNKNOWN) {    Serial.println("RUC @ IR");
    rawCount = My_Decoder.rawlen-1;    }   
  else{ Serial.print(F("Received "));    Serial.print(Pnames(codeType)); 
  if (My_Decoder.value == REPEAT) { Serial.println(F("repeat; ignoring."));     } 
   else{       codeValue = My_Decoder.value; codeBits = My_Decoder.bits;
}   Serial.print(F(" Value:0x"));Serial.println(My_Decoder.value, HEX);
if(My_Decoder.decode_type==MY_PROTOCOL) {
       Serial.println("IR COMMAND DETECTED - DECODING...");
switch(My_Decoder.value) {
  case SELECT_BUTTON: break;
case BUTTON_0: ; break;case BUTTON_9:  break;
    }
    } 
}
}

void storeCode(void) {
  GotNew=true;
  codeType = My_Decoder.decode_type;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, saving as raw");
    rawCount = My_Decoder.rawlen-1;
    for (int i = 1; i <=rawCount; i++) {
      rawCodes[i - 1] = My_Decoder.rawbuf[i];
    };
    My_Decoder.DumpResults();
    codeType=UNKNOWN;
  }
  else {
    Serial.print(F("Received "));
    Serial.print(Pnames(codeType));
    if (My_Decoder.value == REPEAT) {
      // Don't record a NEC repeat value as that's useless.
      Serial.println(F("repeat; ignoring."));
     }
     else {
       codeValue = My_Decoder.value;
       codeBits = My_Decoder.bits;
     }
     Serial.print(F(" Value:0x"));
     Serial.println(My_Decoder.value, HEX);
  }
}
