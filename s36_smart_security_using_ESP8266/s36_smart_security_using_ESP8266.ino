#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#define SCL_PIN 12
#define SDO_PIN 13
char key;
char keymap[] = { 0,
                  '1', '2', '3', 'A',
                  '4', '5', '6', 'B',
                  '7', '8', '9', 'C',
                  '*', '0', '#', 'D' };

const char *ssid = "your SSID";
const char *password = "your PASS";

const char *apiToken = "your API token";
const char *userToken = "your user token";

// Pushover API endpoint
const char *pushoverApiEndpoint = "https://api.pushover.net/1/messages.json";

// Pushover root certificate (valid from 11/10/2006 to 15/01/2038)
const char *PUSHOVER_ROOT_CA =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n"
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
  "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
  "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n"
  "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n"
  "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n"
  "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n"
  "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n"
  "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n"
  "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n"
  "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n"
  "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n"
  "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n"
  "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n"
  "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
  "MrY=\n"
  "-----END CERTIFICATE-----\n";

const int LED_BUILTIN = 2;
const int lamp = 35;
const int LDRpin = 32;
const int LSRpin = 33;
const int PIRpin = 25;
const int servopin = 26;
const int stpBtn = 27;
const int alert = 14;
const int alertChnl = 3;
const int openAngle = 180;
const int closeAngle = 68;

int thresh;
int smpl=100;
String input = "";
String pass = "1234";
bool lock = 1;

Servo servo;

struct tm *timeinfo;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(LDRpin, INPUT);
  pinMode(LSRpin, OUTPUT);
  pinMode(PIRpin, INPUT);
  pinMode(stpBtn, INPUT_PULLUP);
  pinMode(alert, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);

  digitalWrite(LSRpin, 1);
  digitalWrite(lamp, 1);
  digitalWrite(LED_BUILTIN, 1);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nConnected.");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  // Set time via NTP, as required for x.509 validation
  configTime(5 * 3600, 1800, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync...");
  time_t now = time(nullptr);
  while (now < 1510592825) {
    delay(500);
    Serial.print('.');
    now = time(nullptr);
  }
  timeinfo = localtime(&now);
  Serial.print("\nCurrent time: ");
  Serial.print(asctime(timeinfo));

  servo.attach(servopin);
  ledcAttachPin(alert, alertChnl);

  digitalWrite(LED_BUILTIN, 0);
  digitalWrite(LSRpin, 0);
  ledcWriteTone(alertChnl, 500);
  delay(200);
  ledcWriteTone(alertChnl, 1000);
  delay(500);
  ledcWrite(alertChnl,0);
  for(int pos=openAngle;pos>=closeAngle;pos-=3) {
    servo.write(pos);
    delay(15);
  }
  for(int i=0;i<smpl;i++) {
    thresh+=analogRead(LDRpin);
    delay(1);
  }
  thresh/=smpl;
  thresh-=100;
  Serial.print("Threshold: ");
  Serial.println(thresh);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();
  key = read_keypad();
  if (key) {
    if (lock) {
      input += key;
      Serial.print("pass: ");
      Serial.println(input);
      if (input.length() == 4) {
        if (input == pass) {
          ledcWriteTone(alertChnl, 500);
          lock = 0;
          digitalWrite(LSRpin, 1);
          for(int pos=closeAngle;pos<=openAngle;pos+=3) {
            servo.write(pos);
            delay(15);
          }
          Serial.println("Unocked");
          delay(500);
          ledcWrite(alertChnl,0);
        } else {
          for(int x=0;x<3;x++) {
            ledcWriteTone(alertChnl, 500);
            delay(200);
            ledcWrite(alertChnl,0);
            delay(50);
          }
          Serial.println("Wrong Password");
        }
        input = "";
      }
      else{
        ledcWriteTone(alertChnl, 500);
        delay(100);
        ledcWrite(alertChnl,0);
      }
    } else if (key == '*') {
      ledcWriteTone(alertChnl, 500);
      delay(100);
      ledcWriteTone(alertChnl, 1000);
      delay(200);
      ledcWrite(alertChnl,0);
      lock = 1;
      for(int pos=openAngle;pos>=closeAngle;pos-=3) {
        servo.write(pos);
        delay(15);
      }
      digitalWrite(LSRpin, 0);
      Serial.println("Locked");
      delay(500);
    }
    delay(300);
  }

  int x=analogRead(LDRpin);
  if (!lock || (x > thresh && !digitalRead(PIRpin))) {
    delay(50);
    return;
  }
  Serial.println(x);
  Serial.println(!digitalRead(PIRpin));

  digitalWrite(LED_BUILTIN, 1);
  digitalWrite(LSRpin, 1);
  // Create a JSON object with notification details
  // Check the API parameters: https://pushover.net/api
  JsonDocument notification;
  notification["token"] = apiToken;
  notification["user"] = userToken;
  notification["message"] = "Intrusion Detetcted!";
  notification["title"] = "Alert!!!";
  notification["url"] = "";
  notification["url_title"] = "";
  notification["html"] = "";
  notification["priority"] = "";
  notification["sound"] = "cosmic";
  notification["timestamp"] = asctime(timeinfo);

  // Serialize the JSON object to a String
  String jsonStringNotification;
  serializeJson(notification, jsonStringNotification);

  // Create a WiFiClientSecure object
  WiFiClientSecure client;
  // Set the certificate
  client.setCACert(PUSHOVER_ROOT_CA);

  // Create an HTTPClient object
  HTTPClient http;

  // Specify the target URL
  http.begin(client, pushoverApiEndpoint);

  // Add headers
  http.addHeader("Content-Type", "application/json");

  // Send the POST request with the JSON data
  int httpResponseCode = http.POST(jsonStringNotification);

  // Check the response
  Serial.printf("HTTP response code: %d\n", httpResponseCode);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response:");
    Serial.println(response);
  }
  while (digitalRead(stpBtn)) {
    for (int freq = 500; freq < 1000; freq += 10) {
      ledcWriteTone(alertChnl, freq);
      delay(10);
    }
    for (int freq = 1000; freq > 500; freq -= 10) {
      ledcWriteTone(alertChnl, freq);
      delay(10);
    }
  }
  ledcWrite(alertChnl,0);
  digitalWrite(LED_BUILTIN, 0);
  digitalWrite(LSRpin, 0);
  delay(300);
}

char read_keypad(void) {
  byte key_state = 0;
  for (byte count = 1; count <= 16; count++) {
    digitalWrite(SCL_PIN, LOW);
    if (!digitalRead(SDO_PIN)) key_state = count;
    digitalWrite(SCL_PIN, HIGH);
  }
  return keymap[key_state];
}
