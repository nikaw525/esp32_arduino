#include <WebServer.h>

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

WebServer server(80);

struct sekcje
{
  String SectionName;
  float VoltCounter;
  float CurrCounter;
};

sekcje Sekcje[5];

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
  message +=       "<BODY><h1>";
  message +=       Sekcje[sec_number].SectionName;  
  message +=       " Section</h1>";
  message +=       "Voltage: ";
  message +=        String(Sekcje[sec_number].VoltCounter);
  message +=       " Current: ";
  message +=        String(Sekcje[sec_number].CurrCounter);
  message +=        "<br>";
  message +=        "<form action='/Section'>";
  message +=        "<input type=hidden name=section value=";
  message +=        String(sec_number); 
  message +=        "><button class=button name='p_n' type='submit' value='p'>p</button>";
  message +=        "<button class=button name='p_n' type='submit' value='n'>n</button>";
  message +=        "</form><br>";
  message +=       "<a href=\"/\"><button class=\"button\">Back to Pick section</button> </a>";
  message +=        "</BODY>";
  message +=        "</HTML>";  
  
  server.send(200,"text/html", message);  
}

void rootHandle(void){  

  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Pick section</h1>";
  message +=       "<form action='/Section'>";

  for(int i = 0; i<5; i++){        
      message += "<button class=button name='section' type='submit' value=";
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

void setup(void){ 

  Sekcje[0].SectionName = "Vddm";
  Sekcje[0].VoltCounter = 10;
  
  Sekcje[1].SectionName = "Vdda";
  Sekcje[1].VoltCounter = 10;
  
  Sekcje[2].SectionName = "Vdiscr";
  Sekcje[2].VoltCounter = 10;

  Sekcje[3].SectionName = "Vcore";
  Sekcje[3].VoltCounter = 10;

  Sekcje[4].SectionName = "Vlvds ";
  Sekcje[4].VoltCounter = 10;
  
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
  server.on("/", rootHandle);    
  server.on("/Section", htmlHandle);        
  server.begin(); 
}

void loop(){
  server.handleClient();  
}
