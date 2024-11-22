#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

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

// Create a list of certificates with the server certificate
X509List cert(PUSHOVER_ROOT_CA);

const int MQ135pin = D7;
const int alert = D8;
const int stpBtn = D6;
int wait = 3;

struct tm *timeinfo;

void setup()
{
  Serial.begin(9600);
  pinMode(MQ135pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(alert, OUTPUT);
  pinMode(stpBtn, INPUT_PULLUP);

  digitalWrite(LED_BUILTIN, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
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
  while (now < 1510592825)
  {
    delay(500);
    Serial.print('.');
    now = time(nullptr);
  }
  timeinfo = localtime(&now);
  Serial.print("\nCurrent time: ");
  Serial.print(asctime(timeinfo));
  digitalWrite(LED_BUILTIN, 1);
  tone(alert,500);
  delay(200);
  tone(alert,1000);
  delay(500);
  noTone(alert);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();
  if (digitalRead(MQ135pin))
    return;

  digitalWrite(LED_BUILTIN, 0);
  // Create a JSON object with notification details
  // Check the API parameters: https://pushover.net/api
  JsonDocument notification;
  notification["token"] = apiToken;
  notification["user"] = userToken;
  notification["message"] = "High concentration of reactive gases detected.";
  notification["title"] = "Alert!!!";
  notification["url"] = "";
  notification["url_title"] = "";
  notification["html"] = "";
  notification["priority"] = "";
  notification["sound"] = "cosmic";
  notification["timestamp"] = asctime(timeinfo);

  // Serialize the JSON object to a string
  String jsonStringNotification;
  serializeJson(notification, jsonStringNotification);

  // Create a WiFiClientSecure object
  WiFiClientSecure client;
  // Set the certificate
  client.setTrustAnchors(&cert);

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
  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println("Response:");
    Serial.println(response);
  }
  while (digitalRead(stpBtn))
  {
    for (int freq = 500; freq < 1000; freq += 10)
    {
      tone(alert, freq);
      delay(10);
    }
    for (int freq = 1000; freq > 500; freq -= 10)
    {
      tone(alert, freq);
      delay(10);
    }
  }
  noTone(alert);
  digitalWrite(LED_BUILTIN, 1);
}