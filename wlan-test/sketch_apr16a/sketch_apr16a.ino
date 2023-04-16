#include <WebServer.h>
#define PORT 80
#include <GyverOLED.h>
#include "RF24.h"

GyverOLED<SSH1106_128x64> oled;

RF24 radio(4, 5); // (CE, CSN)

const byte address[6] = "1RF24"; // address / identifier

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";

char text2[33] = {0}; //


IPAddress ip(192,168,178,110);
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);
 
String text= "<a href=\"/text_\">TEXT</a>";

void setup(){
  oled.init();  
  oled.clear();   
  oled.update(); 
  oled.setCursorXY(28, 26);   
  oled.print("in1t....");
  oled.rect(0,0,127,63,OLED_STROKE);
  oled.update();
  Serial.begin(115200);
  radio.begin();
  if(radio.isChipConnected()){
     oled.clear();
    oled.home();
    oled.print("Antenna connected");
    oled.update();
  }
  radio.openReadingPipe(0,address); // set the address for pipe 0
  radio.startListening(); // set as receiver
  Serial.println("Minimal Program to switch one LED");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);
 
  delay(1000);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    Serial.print(".");
  }

  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP-Address of ESP32 module: ");
  Serial.println(WiFi.localIP());

  server.on("/text_", recieved);
  server.on("/",handleRoot);
  server.begin();
}
  
void loop(){
  recieved();
  //server.handleClient(); 
  
}

void handleRoot() {
  String message="<h1>Control your ESP8266 by your Browser</h1>";
  message += "Minimal version, just one LED</BR></BR>";
  message += text2;
  server.send(200, "text/html", message);
}

void recieved(){
   if(radio.available()){
    oled.clear(); 
    oled.home();
    radio.read(&text2, sizeof(text2)-1);
    oled.print(text2);
    oled.update();
    Serial.println(text2);
  }
  else{
    oled.clear();
    oled.home();
    oled.print("No signal");
    oled.update();
  }
  //delay(1000); 

 
}
