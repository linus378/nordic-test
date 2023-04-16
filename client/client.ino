
#include <SPI.h>
#include "RF24.h"
#include <GyverOLED.h>


GyverOLED<SSH1106_128x64> oled;

RF24 radio(4, 5); // (CE, CSN)

const byte address[6] = "1RF24"; // address / identifier
void setup() {
  oled.init();  
  oled.clear();   
  oled.update(); 
  oled.setCursorXY(28, 26);   
  oled.print("init....");
  oled.rect(0,0,127,63,OLED_STROKE);
  oled.update();
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0,address); // set the address for pipe 0
  radio.startListening(); // set as receiver
  delay(1000);
}

void loop() { 
  if(radio.available()){
    oled.clear(); 
    oled.home(); 
    char text[33] = {0}; 
    radio.read(&text, sizeof(text)-1);
    oled.print(text);
    oled.update();
    Serial.println(text);
  }
  delay(1000);  
}
