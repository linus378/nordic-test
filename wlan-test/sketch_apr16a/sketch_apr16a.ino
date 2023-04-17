#include <WebServer.h>
#define PORT 80
#include "RF24.h"
#include <GyverOLED.h>

GyverOLED<SSH1106_128x64> oled;

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";


WebServer server(80);


String ant= "<a href=\"/ant_\"><button style=\"background: green; green: white; font-size: x-large; \">Message send</button></a>";

String listening= "<a href=\"/listening_\"><button style=\"background: yellow; color: white; font-size: x-large; \">Listening ... </button></a>";

String noant= "<a href=\"/no_ant\"><button style=\"background: red; color: white; font-size: x-large; \">No data recieved</button></a>";

String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"0.5\"></head>"
                      "<h1> Trailing Transciever</h1>"
                      "This is recieved </BR></BR>";
                      
                      
RF24 radio(4, 5); // (CE, CSN)


const byte address[6] = "1RF24"; // address / identifier

float payload = 0.0;

void _ant(){
 server.send(200, "text/html", ant);
} 

void _listening(){
 server.send(200, "text/html", listening);
} 

void _no_ant(){
  server.send(200, "text/html", noant); 
}

void handleRoot() {
  sendandcheck();
}


void sendandcheck(){
  if((millis()%1000) == 0){
    radio.startListening();  // gentle reminder to listen
  }
  if(radio.available()){ 
    radio.read(&payload, sizeof(payload));
    payload += 0.1;
    String message2 = "";
    message2 += headAndTitle;
    message2 += listening;
    message2 += payload;
    message2 += "</BR></BR>Message recieved ";
    server.send(200, "text/html", message2);
    
    oled.clear(); 
    oled.home();
    oled.print("Recieved" + String(payload));
    oled.update();
      
    radio.stopListening();
   
    delay(300);
    if(radio.write(&payload, sizeof(payload))){
     String message = "";
    message += headAndTitle;
    message += ant;
    message += payload;
    message += "</BR></BR>Message Send ";
    
    oled.clear(); 
    oled.home();
    oled.print("Message send" + String(payload));
    oled.update();  
    
      radio.startListening();
      
      delay(300);
    }
  

}else{
  String message4 = "";
    message4 += headAndTitle;
    message4 += noant;
    server.send(200, "text/html", message4);
    oled.clear(); 
    oled.home();
    oled.print("No message");
    oled.update();  
}

}



void setup(){
  WiFi.begin(ssid, pass);
  WiFi.status();

  radio.begin();
  radio.setPALevel(RF24_PA_LOW); // sufficient for tests side by side 
   
  radio.openReadingPipe(0,address); // set the address
  radio.openWritingPipe(address);
  radio.startListening(); // set as receiver
 
  server.on("/",handleRoot);
  server.on("/ant_", _ant );
  server.on("/listening_", _listening );
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
  // delay for 1 second
  delay(300);
}
