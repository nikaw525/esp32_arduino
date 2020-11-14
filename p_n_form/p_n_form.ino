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

sekcje Sekcje[3];

String insert_string(String first, String second){

  String output = first + second;

  return output;
} 

void htmlHandle(void){
    
  int sec_number;
  
  if(server.arg("section") == "M"){    
    sec_number = 0;
  }
  else if(server.arg("section") == "D"){
    sec_number = 1;
  }
  else if(server.arg("section") == "A"){
    sec_number = 2;
  }
  if(server.arg("p_n") == "p"){
      Sekcje[sec_number].VoltCounter = Sekcje[sec_number].VoltCounter + 1;      
  }
  if(server.arg("p_n") == "n"){
      Sekcje[sec_number].VoltCounter = Sekcje[sec_number].VoltCounter - 1;      
  }  

  Sekcje[sec_number].CurrCounter = Sekcje[sec_number].VoltCounter*1.5;
  
    String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>";
  message +=       server.arg("section");  
  message +=       " Section</h1>";
  message +=       "<HEAD>";
  message +=       "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  message +=       "<STYLE>";
  message +=       ".button{";
  message +=       "display: block;";
  message +=       "padding: 14px 40px;";
  message +=       "width: 100%;";
  message +=       "cursor: pointer;";
  message +=       "border: none;";
  message +=       "background-color: #555555;";
  message +=       "font-size: 20px;";
  message +=       "font-weight: bold;";
  message +=       "color: #DFDC10;";
  message +=       "}";
  message +=       "</STYLE>";
  message +=       "</HEAD>";
  message +=       "Voltage: ";
  message +=        String(Sekcje[sec_number].VoltCounter);
  message +=       " Current: ";
  message +=        String(Sekcje[sec_number].CurrCounter);
  message +=        "<br>";
  message +=        "<form action='/Section'>";
  message +=        "<input type=hidden name=section value=";
  message +=        Sekcje[sec_number].SectionName; 
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

  for(int i = 0; i<3; i++){        
      message += insert_string("<button class=button name='section' type='submit' value=", Sekcje[i].SectionName);
      message += insert_string(">", Sekcje[i].SectionName);
      message += " section</button> </a>";
  }
  message +=        "</form>";
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);
}

void setup(void){ 

  Sekcje[0].SectionName = "M";
  Sekcje[0].VoltCounter = 10;
  
  Sekcje[1].SectionName = "D";
  Sekcje[1].VoltCounter = 10;
  
  Sekcje[2].SectionName = "A";
  Sekcje[2].VoltCounter = 10;
  
  
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
  server.on("/", rootHandle);    
  server.on("/Section", htmlHandle);        
  server.begin(); 
}

void loop(){
  server.handleClient();  
}
