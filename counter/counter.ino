#include <WebServer.h>

const char* ssid     = "abcde";
const char* password = "12345";

WebServer server(80);

int counter = 0;

void counterHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Counter</h1>";
  message +=        String(counter);
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);
  
  counter++;
}

void setup(void){    
  WiFi.begin(ssid, password);   
  server.on("/counter", counterHandle);
  server.begin();
}

void loop(){
  server.handleClient();  
}