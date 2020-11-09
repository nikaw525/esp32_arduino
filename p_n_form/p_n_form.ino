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
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Plus / minus</h1>";
  message +=        String(Mcounter);
  message +=        "<form name='input' action='/get'>";
  message +=        "p/n: <input type='text' name='p_n'>";
  message +=        "<input type='submit' value='Wprowadz'>";
  message +=        "</form><br>";
  message +=       "<a href=\"/\"><button class=\"button\">Back to Pick section</button> </a>";
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);  
}

void MHandle(void){
  if(server.arg("p_n") == "p"){
      Mcounter = Mcounter + 1;
  }
  if(server.arg("p_n") == "n"){
      Mcounter = Mcounter - 1;
  }
  
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Plus / minus</h1>";
  message +=        String(Mcounter);
  message +=        "<form name='input' action='/get'>";
  message +=        "p/n: <input type='text' name='p_n'>";
  message +=        "<input type='submit' value='Wprowadz'>";
  message +=        "</form><br>";
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
  server.on("/get", MHandle);  
  server.begin();
 
}

void loop(){
  server.handleClient();  
}
