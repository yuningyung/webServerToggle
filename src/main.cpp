#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "U+Net43B0";
const char* password = "DD9D033347";
const int toggle = 2;
int status = 0;

ESP8266WebServer server(80);

void Toggle();

void setup() {
  // put your setup code here, to run once:
  pinMode(toggle, INPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  attachInterrupt(toggle, Toggle, FALLING);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  if(MDNS.begin("miniwifi2")) {
    Serial.println("MDNS responder started");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  server.handleClient();
  WiFiClient client;
  HTTPClient http;

  if (status == 1) {
    if(http.begin(client, "http://192.168.219.104/RelayOn")) {
      Serial.print("[HTTP] GET...");

      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        
        if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
        } 
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    }
  }

  if (status == 0) {
    if(http.begin(client, "http://192.168.219.104/RelayOff")) {
      Serial.print("[HTTP] GET...");

      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        
        if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
        } 
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    }
  } 
}

void IRAM_ATTR Toggle() {
  Serial.println("pushed");
  if (status == 1) {
    status = 0;
  } else {
    status = 1;
  }
}