#include <SPI.h>
#include <RF24.h>
#include <WebServer.h>
#define PORT 80
//#include <arduino-timer.h>

const char* ssid = "LI-LAN";
const char* pass = "o6$#@99#Qs*hNy@9R2HEXARQ5";


WebServer server(80);

//auto timer = timer_create_default();

//RF24 radio(4, 5); // (CE, CSN)
//15:37:48.129 -> MOSI: 23
//15:37:48.129 -> MISO: 19
//15:37:48.129 -> SCK: 18
//15:37:48.129 -> SS: 5

//RF24 radio(18,5); // (CE, CSN)
RF24 radio; // (CE, CSN)
const byte address[6] = "1RF24"; // address / identifier

String ant= "<a href=\"/ant_\"><button style=\"background: green; color: white; font-size: x-large; \">Sending Data ... </button></a>";

String listening= "<a href=\"/listening_\"><button style=\"background: yellow; color: white; font-size: x-large; \">Listening ... </button></a>";

String noant= "<a href=\"/no_ant\"><button style=\"background: red; color: white; font-size: x-large; \">No data being send </button></a>";

String headAndTitle = "<head><meta http-equiv=\"refresh\" content=\"0.5\"></head>"
                      "<h1>Leading Transciever</h1>"
                      "Transmitter Status </BR></BR>";

const int MAX_TEXT_LEN = 32;

char text[MAX_TEXT_LEN] = "0";

String  handshake = "shook";
String  payload = "1";

void _ant(){
 server.send(200, "text/html", ant);
} 

void _listening(){
 server.send(200, "text/html", listening);
} 

void _no_ant(){
  server.send(200, "text/html", noant); 
}

void setup(){
  Serial.begin(115200);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
  
  server.on("/ant_", _ant );
  server.on("/no_ant", _no_ant  );
  server.on("/listening_", _listening );
  server.begin();
  Serial.println("webserver online");
   while ( !radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    delay(1000);
  }

  //radio.begin();
  radio.setPALevel(RF24_PA_LOW); // sufficient for tests side by side 
  radio.openWritingPipe(address); // set the address
  radio.openReadingPipe(1,address);
  while ( radio.isChipConnected () != true )  {
    delay(1000);
    Serial.println("rf24 not connected yet..");
  }
  Serial.println("leave setup");
}
  
void loop(){
   //we must handle clients
   server.handleClient();
   radio.startListening(); // set as receiver
  if(radio.available()){
radio.read(&payload, sizeof(payload));
  payload = String(payload.toInt() + 1);
  radio.stopListening();
  radio.write(&payload, sizeof(payload));
   radio.startListening();

    String message3 = "";
    message3 += ant;
    message3 + listening;
    message3 + ("Payload send" + String(payload));

    server.send(200, "text/html", message3);
    /*
    oled.clear(); 
    oled.home();
    oled.print("No message");
    oled.update();
    */

 } 
 if(radio.isChipConnected ()) {
   radio.stopListening();
   radio.write(&handshake, sizeof(handshake));
   radio.startListening();

    String message2 = "";
    message2 += ant;
    message2 += headAndTitle;
    message2 += "Handshake send" + String(handshake);
    
    
    server.send(200, "text/html", message2);
    /*
    oled.clear(); 
    oled.home();
    oled.print("Sending Handshake");
    oled.update();
    */
      
}else{

    String message1 = "";
    message1 += headAndTitle;
    message1 += noant;
    
    server.send(200, "text/html", message1);
    
    /*oled.clear(); 
    oled.home();
    oled.print("No message");
    oled.update();
    */
    delay(1000);

}

}
