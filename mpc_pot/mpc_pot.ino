#include <WebServer.h>

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

#define VD_CS 12
#define VA_CS 14
#define VM_CS 27
#define VC_CS 26
#define VP_CS 25
#define HV_CS 4
#define CALLIB_N_CS 19
#define CALLIB_P_CS 2
#define U_D 23

#define UP 1
#define DOWN 0

WebServer server(80);

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
  int output_pins[]={VD_CS, VA_CS, VM_CS, VC_CS, VP_CS, HV_CS, CALLIB_N_CS, CALLIB_P_CS, U_D};

  for(int i = 0; i<9; i++){
    pinMode(output_pins[i],OUTPUT);
    digitalWrite(output_pins[i],HIGH);
  }  
}

void setup() {  
  potentiometer_init();
  
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
  server.on("/", rootHandle);        
  server.begin();
}

void rootHandle(void){ 
  String cs_str = server.arg("cs");
  String u_d_str = server.arg("u_d");
  String imp_amount_str = server.arg("imp_amount");
  
  int cs = cs_str.toInt();
  int u_d = u_d_str.toInt();
  int imp_amount = imp_amount_str.toInt();  

   String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Potentiometer </h1>";
  message +=       "CS:";
  message +=        cs_str;
  message +=        "<br>";
  message +=       "U_D:";
  message +=        u_d_str;
  message +=        "<br>";
  message +=       "Imp_amount: ";
  message +=        imp_amount_str;
  message +=        "<br>";
  message +=        "</BODY>";
  message +=        "</HTML>";  
  
  server.send(200,"text/html", message); 

  potentiometer_set(cs, u_d,imp_amount);  
}

void loop() {
  server.handleClient();    
}
