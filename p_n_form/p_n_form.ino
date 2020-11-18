#include <WebServer.h>

#include "stylesheet.h"
const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

WebServer server(80);

const int number_of_sect = 5;

struct sekcje
{
  String SectionName;
  float VoltCounter;
  float CurrCounter;
};

struct sekcje Sekcje[number_of_sect]= {
  {.SectionName = "Vddm", .VoltCounter=10},
  {.SectionName = "Vdda", .VoltCounter=10},
  {.SectionName = "Vdiscr", .VoltCounter=10},
  {.SectionName = "Vcore", .VoltCounter=10},
  {.SectionName = "Vlvds", .VoltCounter=10}
 };

void htmlHandle(void){
    
  int sec_number = server.arg("section").toInt();  
 
  if(server.arg("p_n") == "p"){
      Sekcje[sec_number].VoltCounter = Sekcje[sec_number].VoltCounter + 1;      
  }
  if(server.arg("p_n") == "n"){
      Sekcje[sec_number].VoltCounter = Sekcje[sec_number].VoltCounter - 1;      
  }  

  Sekcje[sec_number].CurrCounter = Sekcje[sec_number].VoltCounter*1.5;
  
    String message = "<!DOCTYPE html><HTML>";  
  message +=       "<HEAD>";
  message +=       "<link rel='stylesheet' type='text/css' href='stylesheet.css' media='screen'/>";
  message +=       "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  message +=       "</HEAD>";
  message +=       "<BODY><h2><span>";
  message +=       Sekcje[sec_number].SectionName;  
  message +=       " Section<span></h2>";
  message +=       "<p1>Voltage: ";
  message +=        String(Sekcje[sec_number].VoltCounter);
  message +=       "</p1><p1>Current: ";
  message +=        String(Sekcje[sec_number].CurrCounter);
  message +=        "</p1><br><br><br>";
  message +=        "<form action='/Section'>";
  message +=        "<input type=hidden name=section value=";
  message +=        String(sec_number); 
  message +=        "><button class=button_1 name='p_n' type='submit' value='p'>p</button><br>";
  message +=        "<button class=button_1 name='p_n' type='submit' value='n'>n</button><br>";
  message +=        "</form><br>";
  message +=       "<a href=\'/\'><button class=button_2>Back to<br>Pick section</button> </a>";
  message +=        "</BODY>";
  message +=        "</HTML>";  
  
  server.send(200,"text/html", message);  
}

void rootHandle(void){  

  String message = "<!DOCTYPE html><HTML>";
  message +=       "<HEAD>";
  message +=       "<link rel='stylesheet' type='text/css' href='stylesheet.css' media='screen'/>";
  message +=       "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  message +=       "</HEAD>";
  message +=       "<BODY><h2><span>Pick section</span></h2>";
  message +=       "<form action='/Section'>";

  for(int i = 0; i<number_of_sect; i++){        
      message += "<button class=button_1 name='section' type='submit' value=";
      message +=  String(i);
      message +=  ">";
      message +=  Sekcje[i].SectionName;      
      message += " section</button> </a>";
  }
  message +=        "</form>";
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);
}
void cssHandle(void){
  
  String message = CSS_page;

  server.send(200,"text/css", message);
}
void setup(void){ 
    
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
  server.on("/", rootHandle);    
  server.on("/Section", htmlHandle);  
  server.on("/stylesheet.css", cssHandle);  
  server.begin(); 
}

void loop(){
  server.handleClient();  
}
