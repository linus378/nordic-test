#include <SPI.h>
#include <RF24.h>

long count_msg=0;

RF24 radio(4, 5); // (CE, CSN)
const byte address[6] = "1RF24"; // address / identifier
void setup() {
  radio.begin();
  radio.openWritingPipe(address); // set the address
  radio.stopListening(); // set as transmitter
}
void loop() {
  count_msg++;
  String msg_str="Hi Receiver ";
  msg_str+=count_msg;
  const char * msg_chr = msg_str.c_str();
  const char text[] = "Hi Receiver"; // max. 32 bytes
  radio.write(&text, sizeof(text));
  //radio.write(&msg_chr, sizeof(msg_chr));
  delay(2000);
}
