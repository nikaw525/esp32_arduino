#include <WebServer.h>

const char* ssid     = "Esp-Access-Point";
const char* password = "1234567890";

WebServer server(80);

int Mcounter = 10;
int Acounter = 10;
int Dcounter = 10;

String buttons_html(char Letter){

  String output ="<a href=\"/";
  output += Letter;
  output += "counter\"><button class=\"button\">";
  output += Letter;
  output += "section</button> </a>\n";

  return output;
} 

void htmlHandle(int *counter){

  if(server.arg("p_n") == "p"){
      *counter = *counter + 1;
  }
  if(server.arg("p_n") == "n"){
      *counter = *counter - 1;
  }  

    String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Plus / minus</h1>";
  message +=       "Value: ";
  message +=        String(*counter);
  message +=        "<br>";
  message +=        "<form>";
  message +=        "<button name='p_n' type='submit' value='p'>p</button>";
  message +=        "<button name='p_n' type='submit' value='n'>n</button>";
  message +=        "</form><br>";
  message +=       "<a href=\"/\"><button class=\"button\">Back to Pick section</button> </a>";
  message +=        "</BODY>";
  message +=        "</HTML>";  
  
  server.send(200,"text/html", message);  
}

void rootHandle(void){

  char section_names[] = {'M', 'D', 'A'};

  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Pick section</h1>";

  for(int i = 0; i<3; i++){
        message += buttons_html(section_names[i]);
    }
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);
}

void McounterHandle(void){      
  htmlHandle(&Mcounter);
}

void AcounterHandle(void){  
 htmlHandle(&Acounter);   
}

void DcounterHandle(void){ 
  htmlHandle(&Dcounter); 
}

void setup(void){    
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char 
  server.on("/", rootHandle);    
  server.on("/Mcounter", McounterHandle);    
  server.on("/Dcounter", DcounterHandle);   
  server.on("/Acounter", AcounterHandle);   
  server.begin();
 
}

void loop(){
  server.handleClient();  
}
