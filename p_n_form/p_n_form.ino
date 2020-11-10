#include <WebServer.h>

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

WebServer server(80);

int Mcounter = 10;

void rootHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Pick section</h1>";
  message +=       "<a href=\"/Mcounter\"><button class=\"button\">M section</button> </a>";
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);
}

void McounterHandle(void){  

  if(server.arg("p_n") == "p"){
      Mcounter = Mcounter + 1;
  }
  if(server.arg("p_n") == "n"){
      Mcounter = Mcounter - 1;
  }
  
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Plus / minus</h1>";
  message +=       "Value: ";
  message +=        String(Mcounter);
  message +=        "<br>";  
  message +=        "<a href=\"/Mcounter?p_n=p\"><button class=\"button\">p</button> </a>";
  message +=        "<a href=\"/Mcounter?p_n=n\"><button class=\"button\">n</button> </a>";
  message +=        "<br>";
  message +=       "<a href=\"/\"><button class=\"button\">Back to Pick section</button> </a>";
  message +=        "</BODY>";
  message +=        "</HTML>";  
  
  server.send(200,"text/html", message);  
}

void setup(void){    
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
  server.on("/", rootHandle);    
  server.on("/Mcounter", McounterHandle);    
  server.begin();
 
}

void loop(){
  server.handleClient();  
}
