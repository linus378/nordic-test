#include <WebServer.h>
#define PORT 80
#include "RF24.h"
#include <GyverOLED.h>

GyverOLED<SSH1106_128x64> oled;

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";


WebServer server(80);


String ant= "<a href=\"/ant_\"><button style=\"background: green; green: white; font-size: x-large; \">The Ant is connected</button></a>";

String listening= "<a href=\"/listening_\"><button style=\"background: yellow; color: white; font-size: x-large; \">Listening ... </button></a>";

String noant= "<a href=\"/no_ant\"><button style=\"background: red; color: white; font-size: x-large; \">No data recieved</button></a>";

String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"0.5\"></head>"
                      "<h1> Trailing Transciever</h1>"
                      "This is recieved </BR></BR>";
                      
                      
RF24 radio(4, 5); // (CE, CSN)


const byte address[6] = "1RF24"; // address / identifier

const int MAX_TEXT_LEN = 32;

char text[MAX_TEXT_LEN] = "0";

int count = 0;


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
   // increment the count variable by 1
  count++;

  // convert the count variable to a string and store it in the text array
  String countStr = String(count);
  countStr.toCharArray(text, sizeof(text));

  // delay for 1 second
  delay(300);
}


void sendandcheck(){
  if((millis()%1000) == 0){
    radio.startListening();  // gentle reminder to listen
  }
  if(radio.available()){
    char receivedText[33] = {0}; 
    radio.read(&receivedText, sizeof(receivedText));
    String message = "";
    message += headAndTitle;
    message += ant;
    message += receivedText;
    message += "</BR></BR>Message recieved ";
    server.send(200, "text/html", message);
    oled.clear(); 
    oled.home();
    oled.print(receivedText);
    oled.update();  
    radio.stopListening();
    delay(300);
    if(radio.write(&text, sizeof(text))){
     String message = "";
    message += headAndTitle;
    message += ant;
    message += text;
    message += "</BR></BR>Message Send ";
    oled.clear(); 
    oled.home();
    oled.print("Message send");
    oled.update();  
      radio.startListening();
      delay(300);
    }
  }

  
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
    delay(300);
    
}
else{
  String message1 = "";
    message1 += headAndTitle;
    message1 += noant;
    server.send(200, "text/html", message1);
    oled.clear(); 
    oled.home();
    oled.print("No message");
    oled.update();  
}
}



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
