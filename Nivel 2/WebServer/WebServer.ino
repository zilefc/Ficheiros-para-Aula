#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>

const char* ssid = "TME Education";
const char* password = "mz1234mz";

const int ledPin = 1;

String ledState;
AsyncWebServer server(80);

String processor(const String& var){
  if(var == "resultado"){
    if(digitalRead(ledPin)){
      ledState = "Ligada";
    }
    else{
      ledState = "Deslidaga";
    }
    Serial.print(ledState);
    return ledState;
  }
}
 
void setup(){
  pinMode(ledPin, OUTPUT);
  if(!SPIFFS.begin()){
    return;
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/pagina.html", String(), false, processor);
  });
  
  server.on("/estilo.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilo.css", "text/css");
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/pagina.html", String(), false, processor);
  });
  
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/pagina.html", String(), false, processor);
  });
  server.begin();
}
void loop(){
}
