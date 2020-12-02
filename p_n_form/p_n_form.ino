#include <WebServer.h>
#include <Adafruit_INA219.h>

#include "stylesheet.h"

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

WebServer server(80);

Adafruit_INA219 ina219(0x40);

#define U_D 23

#define UP 0
#define DOWN 1

const int number_of_sect = 5;

struct sekcje
{
  String SectionName;
  float VoltCounter;
  float CurrCounter;
  int CS;
};

struct sekcje Sekcje[number_of_sect]= {
  {.SectionName = "Vddm", .VoltCounter=10, .CurrCounter=0, .CS=27},
  {.SectionName = "Vdda", .VoltCounter=10, .CurrCounter=0, .CS=14},
  {.SectionName = "Vdiscr", .VoltCounter=10, .CurrCounter=0, .CS=12},
  {.SectionName = "Vcore", .VoltCounter=10, .CurrCounter=0, .CS=26},
  {.SectionName = "Vlvds", .VoltCounter=10, .CurrCounter=0, .CS=25}
 };

void potentiometer_set(int cs, int u_d, int imp_amount){  
  digitalWrite(U_D,u_d);
  delay(1);
  digitalWrite(cs, LOW);
  delay(1);
  for(int i=0; i<imp_amount; i++){
    digitalWrite(U_D,!u_d);
    delay(1);
    digitalWrite(U_D,u_d);
    delay(1);
  }
  digitalWrite(cs, HIGH);
}

void potentiometer_init(){  
  pinMode(U_D, OUTPUT);
  digitalWrite(U_D,HIGH);
  
  for(int i = 0; i<9; i++){
    pinMode(Sekcje[i].CS,OUTPUT);
    digitalWrite(Sekcje[i].CS,HIGH);
  }  
}

void htmlHandle(void){
  
  int sec_number = server.arg("section").toInt();  

  switch(server.arg("p_n").toInt()){
    case(1):
      potentiometer_set(Sekcje[sec_number].CS, UP, 1);
      break;
    case(-1):
      potentiometer_set(Sekcje[sec_number].CS, DOWN, 1);  
      break;
     default:
      break;  
  }
  Sekcje[sec_number].VoltCounter = ina219.getBusVoltage_V();
  Sekcje[sec_number].CurrCounter = ina219.getCurrent_mA();  
  
  
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
  message +=        "><button class=button_1 name='p_n' type='submit' value='1'>p</button><br>";
  message +=        "<button class=button_1 name='p_n' type='submit' value='-1'>n</button><br>";
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
  potentiometer_init();
  ina219.begin();  
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
