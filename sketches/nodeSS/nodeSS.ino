#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define MY_PROTOCOL   SONY
#define UP_ARROW        0x1E108 //  INCREASE TILT TIME +1SEC
#define DOWN_ARROW      0x9E108 //DECREASE TILT TIME -1SEC
#define RIGHT_ARROW     0xDE108 //Move All tilt FWD
#define LEFT_ARROW      0x5E108 //Move All tilt BKD
#define SELECT_BUTTON   0x3E108 //  BUZZER!!
#define BUTTON_0    0x90108       //
#define BUTTON_1    0x108        // KITCHENT F
#define BUTTON_2    0x80108    // KITCHENT B
#define BUTTON_3    0x40108    // WINT F
#define BUTTON_4    0xC0108   // WINT B
#define BUTTON_5    0x20108   // DWT F
#define BUTTON_6    0xa0108     // DWT B
#define BUTTON_7    0x60108    // LT F 
#define BUTTON_8    0xe0108     // LT B
#define BUTTON_9    0x10108    // 
#define PRESET_PREV   0xC108  
#define PRESET_NEXT   0x8C108 
#define RED       0xA010C 
#define GREEN       0x6010C 
#define YELLOW      0xE010C 
#define BLUE      0x2010c 
#define SOUND_PREV    0x7B0B      
#define SOUND_NEXT    0x3B0B  
#define TOP_MENU    0x98108
#define POPUP_MENU    0x58108 
#define PAUSE       0x400B  
#define STOP      0xB
#define PLAY      0x200B    
#define FAST_FWD    0x20C108
#define FAST_RWD    0xCC108
#define SUBTITLE    0x88108

unsigned long interval = 300000;           // every 5 minutes
unsigned long time; 
  
unsigned long stars=0;
unsigned long down=0;

void setup(){
 Serial.begin(115200);
 Serial.println("IRLoungeNodeMCU");
  delay(10);
    // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  mySerial.println("Hello, world?");
 //pinMode(pirPin, INPUT);
}

long loops =0;

void loop(){
  //  pirVal = digitalRead(pirPin); if (pirVal == HIGH) {   if (pirState == LOW) {         // we have just turned on
  //   Serial.println("*NEW MOVE"); stars=0; stars++;down=0;pirState = HIGH;  } else if (pirState == HIGH){
  //  Serial.print("*"); stars++;  } 
  //} else if (pirVal == LOW)  { if (pirState == HIGH)  {Serial.println("_END - TURNING OFF LIGHT");
  // pirState = LOW; stars = 0; down =0;down++;   } else if (pirState == LOW){    down++;    Serial.print("_");           }
loops++;

  if (loops > 1000){
  Serial.println(".");
  loops =0; 
}
     
     if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
