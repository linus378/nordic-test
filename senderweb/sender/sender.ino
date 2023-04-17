#include <SPI.h>
#include <RF24.h>
#include <WebServer.h>
#define PORT 80

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";

WebServer server(80);

RF24 radio(4, 5); // (CE, CSN)

const byte address[6] = "1RF24"; // address / identifier

String ant= "<a href=\"/ant_\"><button style=\"background: green; color: white; font-size: x-large; \">Sending Data ... </button></a>";

String listening= "<a href=\"/listening_\"><button style=\"background: yellow; color: white; font-size: x-large; \">Listening ... </button></a>";

String noant= "<a href=\"/no_ant\"><button style=\"background: red; color: white; font-size: x-large; \">No data being send </button></a>";

String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"0.5\"></head>"
                      "<h1>Leading Transciever</h1>"
                      "Transmitter Status </BR></BR>";

const int MAX_TEXT_LEN = 32;

char text[MAX_TEXT_LEN] = "0";

int count = 0;

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
    unsigned long int sendingPeriod = 2000;
    static unsigned long int lastSend = 0;

  
  if((millis()-lastSend)>sendingPeriod){
    radio.stopListening(); // set as transmitter
    lastSend = millis();
  
    if(radio.write(&text, sizeof(text))){
      
    String message = "";
    message += headAndTitle;
    message += ant;
    message += text;
    message += "</BR></BR>Message Send ";
    server.send(200, "text/html", message);
    radio.startListening();
    delay(300);
    
    }
  }
  if(radio.available()){
    char receivedText[33] = {0}; 
    radio.read(&receivedText, sizeof(receivedText));
    String message2 = "";
    message2 += headAndTitle;
    message2 += listening;
    message2 += receivedText;
    message2 += "</BR></BR>Message Recieved ";
    server.send(200, "text/html", message2);
    radio.stopListening();
    delay(300);
  }

else{
  String message1 = "";
    message1 += headAndTitle;
    message1 += noant;
    server.send(200, "text/html", message1);
}
}

void setup(){
  WiFi.begin(ssid, pass);
  WiFi.status();

  server.on("/", handleRoot );
  server.on("/ant_", _ant );
  server.on("/no_ant", _no_ant  );
  server.on("/listening_", _listening );
  server.begin();
  
  radio.begin();

  radio.setPALevel(RF24_PA_LOW); // sufficient for tests side by side 
  radio.openWritingPipe(address); // set the address
  radio.openReadingPipe(1,address);
  
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
