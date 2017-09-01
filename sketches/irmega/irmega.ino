#include <IRLib.h>

int RECV_PIN = A0;
IRrecv My_Receiver(RECV_PIN);
IRdecode My_Decoder;
IRdecodeHash My_Hash_Decoder;

void setup()
{
  My_Receiver.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  delay(2000);while(!Serial);//delay for Leonardo
}

long time,last =0;

void loop() {
  time = millis();
  
  if (My_Receiver.GetResults(&My_Decoder)) {//Puts results in My_Decoder
    My_Hash_Decoder.copyBuf(&My_Decoder);//copy the results to the hash decoder
    My_Decoder.decode();
    Serial.print("real decode type:");
    Serial.print(Pnames(My_Decoder.decode_type));
    Serial.print(" value: 0x");
    Serial.print(My_Decoder.value, HEX);
    My_Hash_Decoder.decode();
    Serial.print(", hash decode: 0x");
    Serial.println(My_Hash_Decoder.hash, HEX); // Do something interesting with this value
    delay(1000);
    My_Receiver.resume(); 
  }
  if ((time-last)>10000){
  Serial.println(".");
  last = time;
}
}
