#include <WebServer.h>

const char* ssid     = "abcde";
const char* password = "12345";

WebServer server(80);

int counter = 0;

void counterHandle(void){
  String message = "<!DOCTYPE html><HTML>";
  message +=       "<BODY><h1>Counter</h1>";
  message +=        String(counter);
  message +=        "</BODY>";
  message +=        "</HTML>";

  server.send(200,"text/html", message);
  
  counter++;
}

void setup(void){   
  Serial.begin(115200);  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);   
  WiFi.begin(ssid, password);     
  while(WiFi.status() != WL_CONNECTED){    
    delay(500);
    Serial.print(".");
  }
  Serial.print("");
  Serial.print("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 

  server.on("/counter", counterHandle);

  server.begin();
}

void loop(){
  server.handleClient();  
}
