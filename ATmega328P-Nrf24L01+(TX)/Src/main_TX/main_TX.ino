#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(10, 9); // CE, CSN
const byte address[6] = "00001";
//char TXString[100];

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
void setup() {
  Serial.begin(115200);
  inputString.reserve(200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}
void loop() {
  /*
  char TXString[] = "m1,15,1,15";
  radio.write(&TXString, sizeof(TXString));
  Serial.write(TXString,'\n');
  delay(5000);
  char TXString2[] = "m1,1,1,1";
  radio.write(&TXString2, sizeof(TXString2));
  Serial.write(TXString2,'\n');
  delay(5000);*/
  if (stringComplete) {
    Serial.println(inputString);
    delay(10);
    int str_len = inputString.length() + 1; 
    char TXString[str_len];
    inputString.toCharArray(TXString, str_len);
    //Serial.println(TXString);
    radio.write(&TXString, sizeof(TXString));
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
