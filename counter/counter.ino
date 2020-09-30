#include <WebServer.h>

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

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
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char     
  server.on("/counter", counterHandle);
  server.begin();
}

void loop(){
  server.handleClient();  
}
