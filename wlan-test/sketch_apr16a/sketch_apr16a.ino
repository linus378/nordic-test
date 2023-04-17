#include <WebServer.h>
#define PORT 80
#include "RF24.h"
#include <GyverOLED.h>

GyverOLED<SSH1106_128x64> oled;

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";


WebServer server(80);


String ant= "<a href=\"/ant_\"><button style=\"background: red; color: white; font-size: x-large; \">The Ant is connected</button></a>";

String noant= "<a href=\"/no_ant\"><button style=\"background: red; color: white; font-size: x-large; \">Ant no no</button></a>";

String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"2\"></head>"
                      "<h1> Antenna connected</h1>"
                      "This is recieved/BR></BR>";
                      
                      
RF24 radio(4, 5); // (CE, CSN)


const byte address[6] = "1RF24"; // address / identifier

void setup(){
  WiFi.begin(ssid, pass);
  WiFi.status();

 
  radio.begin();
  radio.openReadingPipe(0,address); // set the address for pipe 0
  radio.startListening(); // set as receiver
 
  server.on("/",handleRoot);
  server.on("/ant_", _ant );
  server.on("/no_ant", _no_ant  );
  server.begin();

  oled.init();  
  oled.clear();   
  oled.update();
  oled.setCursorXY(28, 26);
  oled.rect(0,0,127,63,OLED_STROKE);
  oled.update();
}
  
void loop(){
  server.handleClient();
}


void sendandcheck(){
if(radio.available()){
    char text[33] = {0};
    radio.read(&text, sizeof(text)-1);

    
    String message = "";
    message += headAndTitle;
    message += ant;
    message += text;
    server.send(200, "text/html", message);

    oled.clear(); 
    oled.home();
    oled.print(text);
    oled.update();
    
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
