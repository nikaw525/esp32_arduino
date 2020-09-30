#include <WebServer.h> 
#include <Adafruit_INA219.h>

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

Adafruit_INA219 ina219;
WebServer server(80);

float busvoltage = 0;
float current_mA = 0;
  
void rootHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1> </h1>";
  message +=       "<a href=\"/current\"><button class=\"button\">Current</button> </a>";
  message +=       "<a href=\"/voltage\"><button class=\"button\">Voltage</button> </a>";
  message +=       "<a href=\"/config\"><button class=\"button\">Config</button> </a>";
  message +=       "</BODY>";
  message +=       "</HTML>";

  server.send(200,"text/html", message);
}

void configHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Config</h1>";
  message +=       "<a href=\"/current\"><button class=\"button\">Current</button> </a>";
  message +=       "<a href=\"/voltage\"><button class=\"button\">Voltage</button> </a>";
  message +=       "</BODY>";
  message +=       "</HTML>"; 

  server.send(200,"text/html", message);
}

void currentHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Ina219 Current [mA]</h1>";
  message +=       String(current_mA);
  message +=       " mA";
  message +=       "<br><a href=\"/config\"><button class=\"button\">Config</button> </a>";
  message +=       "<a href=\"/voltage\"><button class=\"button\">Voltage</button> </a>";
  message +=       "</BODY>";
  message +=       "</HTML>"; 

  server.send(200,"text/html", message);
}

void voltageHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Ina219 Bus Voltage</h1>";
  message +=       String(busvoltage);
  message +=       " V";
  message +=       "<br><a href=\"/current\"><button class=\"button\">Current</button> </a>";
  message +=       "<a href=\"/config\"><button class=\"button\">Config</button> </a>";
  message +=       "</BODY>";
  message +=       "</HTML>"; 

  server.send(200,"text/html", message);
}

void setup(void){   
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
   
  ina219.begin();
  
  server.on("/voltage", voltageHandle);
  server.on("/current", currentHandle);
  server.on("/config", configHandle);
  server.on("/", rootHandle);
  
  server.begin();
}

void loop(){
  server.handleClient();  
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
} 
