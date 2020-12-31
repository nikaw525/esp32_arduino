#include <WebServer.h>
#include <Adafruit_INA219.h>
#include <string.h>

#include "stylesheet.h"

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

WebServer server(80);

#define U_D 23

#define UP 0
#define DOWN 1

const int number_of_sect = 5;
int refresh_counter = 0;

String color[2] = {"#006600","#000099"};

struct sekcje
{
  String SectionName;
  float VoltCounter;
  float CurrCounter;
  int CS;
  Adafruit_INA219* Ina219;
};

struct sekcje Sekcje[number_of_sect]= {
  {.SectionName = "Vddm", .VoltCounter=0, .CurrCounter=0, .CS=27, new Adafruit_INA219(0x40)},
  {.SectionName = "Vdda", .VoltCounter=0, .CurrCounter=0, .CS=14, new Adafruit_INA219(0x41)},
  {.SectionName = "Vdiscr", .VoltCounter=0, .CurrCounter=0, .CS=12, new Adafruit_INA219(0x42)},
  {.SectionName = "Vcore", .VoltCounter=0, .CurrCounter=0, .CS=26, new Adafruit_INA219(0x43)},
  {.SectionName = "Vlvds", .VoltCounter=0, .CurrCounter=0, .CS=25, new Adafruit_INA219(0x44)}
 };

void potentiometer_set(int cs, int imp_amount){  
  int u_d = 0;
  
  if(imp_amount > 0){
    u_d = UP;
  }
  if(imp_amount < 0){
    u_d = DOWN;
    imp_amount = imp_amount*(-1);
  } 
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
  
  for(int i = 0; i<number_of_sect; i++){
    pinMode(Sekcje[i].CS,OUTPUT);
    digitalWrite(Sekcje[i].CS,HIGH);
  }  
}

void htmlHandle(void){
  
  refresh_counter ++;
  
  int sec_number = server.arg("section").toInt();  
  
  potentiometer_set(Sekcje[sec_number].CS, server.arg("p_n").toInt());
 
  Sekcje[sec_number].VoltCounter = (Sekcje[sec_number].Ina219->getBusVoltage_V()) + (Sekcje[sec_number].Ina219->getShuntVoltage_mV()/1000);
  Sekcje[sec_number].CurrCounter = Sekcje[sec_number].Ina219->getCurrent_mA()/1000;  
  
  
    String message = "<!DOCTYPE html><HTML>";  
  message +=       "<HEAD>";
  message +=       "<link rel='stylesheet' type='text/css' href='stylesheet.css' media='screen'/>";
  message +=       "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  message +=       "</HEAD>";
  message +=       "<BODY><h2 style='color:";
  message +=       color[refresh_counter%2];
  message +=       "'>";
  message +=       Sekcje[sec_number].SectionName;  
  message +=       "</h2>";
  message +=        "<p4>U[V]</p4><p4>I[A]</p4><p2>";
  message +=       String(Sekcje[sec_number].VoltCounter,3);
  message +=       "</p2><p2>";
  message +=       String(Sekcje[sec_number].CurrCounter,3);
  message +=       "</p2><br><br><br>";
  message +=       "<form action='/Section'>";
  message +=       "<input type=hidden name=section value=";
  message +=       String(sec_number); 
  message +=       "><button class=button_3 name='p_n' type='submit' value='5'>+ +</button>";
  message +=       "<button class=button_3 name='p_n' type='submit' value='1'>+</button>";
  message +=       "<button class=button_3 name='p_n' type='submit' value='-5'>- -</button>";
  message +=       "<button class=button_3 name='p_n' type='submit' value='-1'>-</button>";
  message +=       "</form><br>";
  message +=       "<a href=\'/\'><button class=button_3>Menu</button> </a>";
  message +=       "<a href=\'/Section?section=";
  message +=       String(sec_number); 
  message +=       "\'><button class=button_3>Refresh</button></a>";
  message +=       "</BODY>";
  message +=       "</HTML>";  
  
  server.send(200,"text/html", message);  
}

void rootHandle(void){ 
   
  refresh_counter ++;
  
  for(int i = 0; i<number_of_sect;i++){
    Sekcje[i].VoltCounter = Sekcje[i].Ina219->getBusVoltage_V();
    Sekcje[i].CurrCounter = Sekcje[i].Ina219->getCurrent_mA()/1000;
  }
  
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<HEAD>";
  message +=       "<link rel='stylesheet' type='text/css' href='stylesheet.css' media='screen'/>";
  message +=       "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  message +=       "</HEAD>";
  message +=       "<BODY><h2 style='color:";
  message +=       color[refresh_counter%2];
  message +=       "'>Menu</h2>";
  message +=       "<p3> </p3><p3>U[V]</p3><p3>I[A]</p3>";
  message +=       "<form action='/Section'>";

  for(int i = 0; i<number_of_sect; i++){  
      message +=   "<button class=button_1 name='section' type='submit' value=";
      message +=   String(i);
      message +=   ">";
      message +=   Sekcje[i].SectionName;
      message +=   "</button></a>";
      message +=   " <p1>";
      message +=   String(Sekcje[i].VoltCounter,3);
      message +=   "</p1> <p1>";
      message +=   String(Sekcje[i].CurrCounter,3);
      message +=   "</p1></br>";
  }
  message +=       "</form><form>";
  message +=       "<button  class=button_3 name='save' type='submit' value='1' color=white>Save</button> </a>";
  message +=       "<button class=button_3 name='refresh' type='submit' value='1'>Refresh</button> </a>";
  message +=       "</form></BODY>";
  message +=       "</HTML>";

  server.send(200,"text/html", message);
}
void cssHandle(void){
  
  String message = CSS_page;

  server.send(200,"text/css", message);
}
void setup(void){ 
  potentiometer_init();
  
  for (int i = 0; i<number_of_sect; i++){
  Sekcje[i].Ina219->begin();  
  Sekcje[i].Ina219->setCalibration_16V_2A();
  }
  
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
