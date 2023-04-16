#include <SPI.h>
#include <RF24.h>
#include <WebServer.h>
#define PORT 80

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";

WebServer server(80);

RF24 radio(4, 5); // (CE, CSN)

const byte address[6] = "1RF24"; // address / identifier

String ant= "<a href=\"/ant_\"><button style=\"background: red; color: white; font-size: x-large; \">The Ant is connected</button></a>";


String noant= "<a href=\"/no_ant\"><button style=\"background: red; color: white; font-size: x-large; \">Ant no no</button></a>";


void setup(){
  WiFi.begin(ssid, pass);
  WiFi.status();

  server.on("/",handleRoot);
  server.on("/ant_", _ant);
  server.begin();
  
  radio.begin();
  radio.openWritingPipe(address); // set the address
  radio.stopListening(); // set as transmitter
}

  
void loop(){
  server.handleClient(); 
}

void handleRoot() {
  _ant();
}

void _ant(){
  if(radio.isChipConnected()){
  const char text[] = "Hi Receiver"; // max. 32 bytes
  radio.write(&text, sizeof(text));
  server.send(200, "text/html", ant); 
  
  
  delay(2000);
}
else{
  server.send(200, "text/html", noant); 
}
}
