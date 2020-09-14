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

void counterHandle(void){
  client.println("<!DOCTYPE html><HTML>");
  client.println("<BODY><h1>Counter</h1>");
  client.println(counter);
  client.println("</BODY>");
  client.println("</HTML>"); 

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
                     
            if (header.indexOf("GET /counter") >= 0) {       
              counterHandle();            
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
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
