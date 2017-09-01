/*
 * Example command - Samsung TV power toggle: 38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798\r\n
 * For more codes, visit: https://irdb.globalcache.com/
 * How to use this program:
 *   1) Update "ssid" and "password" below for your WIFI network.
 *   2) Compile and upload the sketch to your ESP8266 module.
 *   3) (Optional) Use the serial connection to confirm it started and get the
 *      IP address.
 *   4) From a client machine, connect to port 4998 on the ESP8266, using
 *      'telnet', 'nc' (netcat), 'putty' or similar command, etc.
 *      You may need to install one.
 *   Unix/OSX: *     Start a shell. Then type:
 *       telnet <esp8266deviceIPaddress> 4998
 *   Windows: *     Start a new CMD window, then type:
 *       telnet <esp8266deviceIPaddress> 4998
  *   5) Enter a Global Cache-formatted code, starting at the frequency,
 *      and then a return/enter at the end. No spaces. e.g.:
  *   38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798
 *   To exit the 'telnet' command:
 *     press <control> + <]> at the same time, then press 'q', and then <return>.
 *     <control> + <d> might work.
 * This program will display the ESP's IP address on the serial console, or you
 * can check your wifi router for it's address.
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <IRsend.h>

WiFiServer server(4998);  // Uses port 4998.
WiFiClient client;

const char* ssid = "LAN_Time_Forgot";
const char* password = "tasmania";
MDNSResponder mdns;

// VARS
uint16_t RECV_PIN = D2;
//IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)
uint16_t *code_array;

IRrecv irrecv(RECV_PIN);
decode_results results;  // Somewhere to store the results
irparams_t save;         // A place to copy the interrupt state while decoding.

void sendGCString(String str) {
 }

#define HOST_NAME "tnode187"   

void setup() {
  // Status message will be sent to the PC at 115200 baud
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
   Serial.println("IR TCP Server");
    WiFi.hostname(HOST_NAME);
     WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(900);  Serial.print(".");
  }
    server.begin(); IPAddress myAddress = WiFi.localIP(); Serial.println(myAddress);
   if (mdns.begin("lounge", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  //irsend.begin();
  irrecv.enableIRIn();  // Start the receiver
}

// Display encoding type
void encoding(decode_results *results) {
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break;
    case NEC:          Serial.print("NEC");           break;
    case NEC_LIKE:     Serial.print("NEC (non-strict)");  break;
    case SONY:         Serial.print("SONY");          break;
    case RC5:          Serial.print("RC5");           break;
    case RC6:          Serial.print("RC6");           break;
    case DISH:         Serial.print("DISH");          break;
    case SHARP:        Serial.print("SHARP");         break;
    case JVC:          Serial.print("JVC");           break;
    case SANYO:        Serial.print("SANYO");         break;
    case SANYO_LC7461: Serial.print("SANYO_LC7461");  break;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break;
    case SAMSUNG:      Serial.print("SAMSUNG");       break;
    case LG:           Serial.print("LG");            break;
    case WHYNTER:      Serial.print("WHYNTER");       break;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break;
    case PANASONIC:    Serial.print("PANASONIC");     break;
    case DENON:        Serial.print("DENON");         break;
    case COOLIX:       Serial.print("COOLIX");        break;
  }
  if (results->repeat) Serial.print(" (Repeat)");
}

// Dump out the decode_results structure.
void dumpInfo(decode_results *results) {
  if (results->overflow)
    Serial.println("WARNING: IR code too long."
                   "Edit IRrecv.h and increase RAWBUF");
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");
  Serial.print("Code      : ");
  serialPrintUint64(results->value, 16);
  Serial.print(" ");
  Serial.print(results->bits, DEC);
  Serial.println(" bits");
}

void dumpRaw(decode_results *results) {
   Serial.print("Timing[");  Serial.print(results->rawlen - 1, DEC); Serial.println("]: ");
  for (uint16_t i = 1;  i < results->rawlen;  i++) {
    if (i % 100 == 0)
      yield();  // Preemptive yield every 100th entry to feed the WDT.
    uint32_t x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000) Serial.print(" ");
      if (x < 100) Serial.print(" ");
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000) Serial.print(" ");
      if (x < 100) Serial.print(" ");
      Serial.print(x, DEC);
      if (i < results->rawlen - 1)
        Serial.print(", ");  // ',' not needed for last one
    }
    if (!(i % 8)) Serial.println("");
  }
  Serial.println("");  // Newline
}

// Dump out the decode_results structure.
void dumpCode(decode_results *results) {
  // Start declaration
   
  Serial.print("uint16_t  "); Serial.print("rawData["); Serial.print(results->rawlen - 1, DEC); Serial.print("] = {");      
  
  for (uint16_t i = 1; i < results->rawlen; i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if (i < results->rawlen - 1)
      Serial.print(",");  // ',' not needed on last one
    if (!(i & 1)) Serial.print(" ");
  }
  Serial.print("};");  
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  serialPrintUint64(results->value, 16);
  Serial.println("");
  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0) {
      Serial.print("uint32_t  address = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
      Serial.print("uint32_t  command = 0x");
      Serial.print(results->command, HEX);
      Serial.println(";");
    }
    // All protocols have data
    Serial.print("uint64_t  data = 0x");
    serialPrintUint64(results->value, 16);
    Serial.println(";");
  }
}

void loop() {
  
   
   if (irrecv.decode(&results, &save)) {
    dumpInfo(&results);           // Output the results
    dumpRaw(&results);            // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("Rec");           // Blank line between entries
  }
  while (!client.connected()) {
    delay(900);
    client = server.available();
  }
   while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar);
    }
  }
 }
