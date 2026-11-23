#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP8266-LED";
const char* password = "ledcontrol";  // At least 8 characters

const int ledPin = LED_BUILTIN;
bool ledOn = false;

ESP8266WebServer server(80);

void showPage() {
  String page = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>LED Control</title>
    </head>
    <body style="font-family: sans-serif; text-align: center; margin-top: 50px;">
      <h1>LED Control</h1>
      <p>LED is %STATE%</p>
      <p><a href="/on"><button style="font-size: 24px;">ON</button></a></p>
      <p><a href="/off"><button style="font-size: 24px;">OFF</button></a></p>
    </body>
    </html>
  )rawliteral";

  page.replace("%STATE%", ledOn ? "ON" : "OFF");
  server.send(200, "text/html", page);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // Built-in LED is typically active-low: HIGH = off

  WiFi.softAP(ssid, password);

  server.on("/", showPage);

  server.on("/on", []() {
    ledOn = true;
    digitalWrite(ledPin, LOW);  // on
    showPage();
  });

  server.on("/off", []() {
    ledOn = false;
    digitalWrite(ledPin, HIGH);  // off
    showPage();
  });

  server.begin();
}

void loop() {
  server.handleClient();
}