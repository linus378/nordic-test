
#include <SPI.h>
#include "RF24.h"
#include "U8g2lib.h"


//U8G2_ST7920_128X64_1_SW_SPI u8g2(rotation, clock, data, cs [, reset])
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 13, 11, 21, 22);

//U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, 13, 11, 21, 22);

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 22, 11, 21);

RF24 radio(4, 5); // (CE, CSN)

const byte address[6] = "1RF24"; // address / identifier
void setup() {
  Serial.begin(115200);
  u8g2.begin();
  radio.begin();
  radio.openReadingPipe(0,address); // set the address for pipe 0
  radio.startListening(); // set as receiver
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0,20,"start");
  u8g2.sendBuffer();
  delay(1000);
}

void loop() { 
  if(radio.available()){
    char text[33] = {0}; 
    radio.read(&text, sizeof(text)-1);
    Serial.println(text);
  }
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"testwas");
  } while ( u8g2.nextPage() );
  delay(1000);  
}
