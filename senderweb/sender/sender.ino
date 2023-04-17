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

String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"2\"></head>"
                      "<h1>Is Antenna Connected?</h1>"
                      "Just look below</BR></BR>";


void setup(){
  WiFi.begin(ssid, pass);
  WiFi.status();

  server.on("/", handleRoot );
  server.on("/ant_", _ant );
  server.on("/no_ant", _no_ant  );
  server.begin();
  
  
  radio.begin();
  radio.openWritingPipe(address); // set the address
  radio.stopListening(); // set as transmitter
}




  
void loop(){
  server.handleClient();
}



void sendandcheck(){
if(radio.isChipConnected()){

    String message = "";
    message += headAndTitle;
    message += ant;
    server.send(200, "text/html", message);
    const char text[] = "Hi Receiver"; // max. 32 bytes
    radio.write(&text, sizeof(text));
}
else{
  String message1 = "";
    message1 += headAndTitle;
    message1 += noant;
    server.send(200, "text/html", message1);  
}
}


void _ant(){
 server.send(200, "text/html", ant);
} 

void _no_ant(){
  server.send(200, "text/html", noant); 
}

void handleRoot() {
   sendandcheck();
}
