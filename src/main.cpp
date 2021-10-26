#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "U+Net43B0";
const char* password = "DD9D033347";
const int toggle = 2;
int mode = 0;
int status = 0;

WiFiClient client;
ESP8266WebServer server(80);

void handleRoot();
void handleNotFound();
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

  server.on("/", handleRoot);
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "Hello from the inline function\n");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  server.handleClient();
  WiFiClient client;
  HTTPClient http;

  if (mode == 1 && status == 1) {
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
    mode = 0;
  }

  if (mode == 1 && status == 0) {
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
    mode = 0;
  } 
}

void handleRoot() {
  String message = (server.method() == HTTP_GET)?"GET":"POST";
  message += " " + server.uri() + "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + " : " + server.arg(i) + "\n";
  }
  message += "\nHello from ESP8266!\n";
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void IRAM_ATTR Toggle() {
  Serial.println("pushed");
  mode = 1;
  if (status == 1) {
    status = 0;
  } else {
    status = 1;
  }
}