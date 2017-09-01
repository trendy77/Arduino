#define pirPin D1
unsigned long stars=0;
unsigned long down=0;
    // Place to store read PIR Value
int pirState = 0; // we start, assuming no motion detected
int pirVal = LOW;        // variable for reading the pin status
int lightNo = 1;

void setup() {
Serial.begin(115200);
  Serial.println("COMMENCING TNODE LIGHTSENSOR");
  delay(10);
pinMode(pirPin, INPUT);
}

void loop() {
 pirVal = digitalRead(pirPin); // read input value
  if (pirVal == HIGH) {        
    if (pirState == LOW) {         // we have just turned on
      Serial.println("*NEW MOVE"); stars=0; stars++;down=0;
       pirState = HIGH; 
       } else if (pirState == HIGH){
      Serial.print("*"); stars++;
       } 
  } else if (pirVal == LOW) 
      { if (pirState == HIGH) 
          {
      Serial.println("_END - TURNING OFF LIGHT");
      pirState = LOW; stars = 0; down =0;down++;  
    } else if (pirState == LOW)
    {
    down++;
    Serial.print("_"); 
    }
      }
        delay(10);
 }
