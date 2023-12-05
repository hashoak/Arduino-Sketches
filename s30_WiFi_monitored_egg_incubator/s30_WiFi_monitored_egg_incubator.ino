#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4

const char* ssid = "ORBI12";
const char* pass = "Mmfsl1234";

int dhtPin = D3;
int heatPin = D5;
int fanPin = D6;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
DHT dht;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiServer server(80);

IPAddress local_IP(1, 2, 3, 4);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello");
  dht.setup(dhtPin);

  sensors.begin();

  while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    Serial.println(F("SSD1306 allocation failed"));
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setTextSize(1);
  display.cp437(true);
  display.println("Initializing...");
  display.display();

  pinMode(heatPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  Serial.println();
  Serial.print("Configuring IP: ");
  Serial.println(WiFi.config(local_IP, gateway, subnet));  // ? "Done" : "Failed!");
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, pass);

  // start the server:
  server.begin();
  Serial.println("Server started");
}

float temp, hmdt, temp1;
int heatState = 1, fanState = 0;
char html1[] = "<!DOCTYPE html> <html lang='en' style='height:100vh;background-image:linear-gradient(135deg,purple,#2000ff);font-family:courier'> <head> <title>Incubator Status</title> </head> <body style='color:white;font-size:6vh'> <p style='margin:0;padding:3vh'>Temperature: ";
char html2[] = "</p> <p style='margin:0;padding:3vh'>Humidity: ";
char html3[] = "</p> <input onClick='history.go(0);' type='button' value='Refresh' style='color:purple;margin:3vh;border:0;border-radius:1.5vh;font:bold 4.5vh courier'> </body> </html>";

void loop() {
  delay(dht.getMinimumSamplingPeriod());
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  temp1 = dht.getTemperature();
  hmdt = dht.getHumidity();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("IP:");
  display.println(WiFi.localIP());
  display.setTextSize(2);
  display.print("Temp:");
  display.println(temp, 2);
  display.print("temp:");
  display.println(temp1, 0);
  display.print("Hmdt:");
  display.println(hmdt, 0);
  display.display();

  if (temp < 37.65 && !heatState) digitalWrite(heatPin, heatState), heatState = 1;
  else if (temp > 37.75 && heatState) digitalWrite(heatPin, heatState), heatState = 0;
  
  if (hmdt < 60 && !fanState) fanState = 1, digitalWrite(fanPin, fanState);
  else if (hmdt > 60 && fanState) fanState = 0, digitalWrite(fanPin, fanState);

  WiFiClient client = server.available();
  if (!client) return;
  while (!client.available()) delay(1);

  String req = client.readStringUntil('\r');
  client.flush();
  Serial.println(req);
  if (req.indexOf("") != -1) {  //checks if you're on the main page
    client.print("HTTP/1.1 200 OK\r\n");
    if (req.indexOf("favicon.ico") == -1) {
      client.print("Content-Type: text/html\r\n\r\n");
      client.printf("%s%.2f%s%.2f%s", html1, temp, html2, hmdt, html3);
    }
  } else {
    client.print("HTTP/1.1 404 Not Found\r\n");
    client.print("Connection: close\r\n\r\n");
    Serial.println("invalid request");
    client.stop();
    return;
  }
}