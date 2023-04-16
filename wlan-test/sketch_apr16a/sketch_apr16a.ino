#include <WebServer.h>
#define PORT 80
#include <GyverOLED.h>

GyverOLED<SSH1106_128x64> oled;

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";


IPAddress ip(192,168,178,110);
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);
 
String led1= "<a href=\"/led_on\">LED On</a>";
String led0= "<a href=\"/led_off\">LED Off</a>";

void setup(){
  oled.init();  
  oled.clear();   
  oled.update(); 
  oled.setCursorXY(28, 26);   
  oled.print("init....");
  oled.rect(0,0,127,63,OLED_STROKE);
  oled.update();
  Serial.begin(115200); 
  Serial.println("Minimal Program to switch one LED");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP-Address of ESP8266 module: ");
  Serial.println(WiFi.localIP());
  
  server.on("/",handleRoot);
  server.on("/led_on", don);
  server.on("/led_off", dof);
  server.begin();
}
  
void loop(){
  server.handleClient(); 
}

void handleRoot() {
  String message="<h1>Control your ESP8266 by your Browser</h1>";
  message += "Minimal version, just one LED</BR></BR>";
  message += led1;
  server.send(200, "text/html", message);
}

void don(){
    oled.clear(); 
    oled.home(); 
    char text[33] = {0}; 
    oled.print("don");
    oled.update();
    Serial.println(text);
}

void dof(){
    oled.clear(); 
    oled.home(); 
    char text[33] = {0}; 
    oled.print("DOF");
    oled.update();
    Serial.println(text);
}
