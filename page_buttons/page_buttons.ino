#include <WiFi.h> 

const char* ssid     = "abcde";
const char* password = "12345";

WiFiClient client;
WiFiServer server(80);

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

int counter = 0;
String header;

void rootHandle(void){
  client.println("<!DOCTYPE html><HTML>");
  client.println("<BODY><h1> </h1>");
  client.println("<a href=\"/current\"><button class=\"button\">Current</button> </a>");
  client.println("<a href=\"/voltage\"><button class=\"button\">Voltage</button> </a>");
  client.println("<a href=\"/config\"><button class=\"button\">Config</button> </a>");
  client.println("</BODY>");
  client.println("</HTML>"); 
}

void configHandle(void){
  client.println("<!DOCTYPE html><HTML>");
  client.println("<BODY><h1>Config</h1>");
  client.println("<a href=\"/current\"><button class=\"button\">Current</button> </a>");
  client.println("<a href=\"/voltage\"><button class=\"button\">Voltage</button> </a>");
  client.println("</BODY>");
  client.println("</HTML>"); 
}

void currentHandle(void){
  client.println("<!DOCTYPE html><HTML>");
  client.println("<BODY><h1>Current</h1>");
  client.println("<a href=\"/voltage\"><button class=\"button\">Voltage</button> </a>");
  client.println("<a href=\"/config\"><button class=\"button\">Config</button> </a>");
  client.println("</BODY>");
  client.println("</HTML>"); 
}

void voltageHandle(void){
  client.println("<!DOCTYPE html><HTML>");
  client.println("<BODY><h1>Voltage</h1>");
  client.println("<a href=\"/current\"><button class=\"button\">Current</button> </a>");
  client.println("<a href=\"/config\"><button class=\"button\">Config</button> </a>");
  client.println("</BODY>");
  client.println("</HTML>"); 
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
  server.begin();
}
void loop(){
  client = server.available();  

  if (client) {                           
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                           
        header += c;
        if (c == '\n') {    
          if (currentLine.length() == 0) {           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println(); 
                       
            if (header.indexOf("GET /voltage") >= 0) {       
              voltageHandle();            
            }  
            else if (header.indexOf("GET /current") >= 0) {       
              currentHandle();            
            }  
            else if (header.indexOf("GET /config") >= 0) {       
              configHandle();            
            }   
            else if (header.indexOf("GET /") >= 0) {       
              rootHandle();            
            }                     
          } 
          else { 
            currentLine = "";
          }
        } 
        else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }    
   
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
