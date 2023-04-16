#include <SPI.h>
#include <RF24.h>
RF24 radio(4, 5); // (CE, CSN)
const byte address[6] = "1RF24"; // address / identifier
void setup() {
  radio.begin();
  radio.openWritingPipe(address); // set the address
  radio.stopListening(); // set as transmitter
}
void loop() {
  const char text[] = "Hi Receiver"; // max. 32 bytes
  radio.write(&text, sizeof(text));
  
  delay(2000);
}
