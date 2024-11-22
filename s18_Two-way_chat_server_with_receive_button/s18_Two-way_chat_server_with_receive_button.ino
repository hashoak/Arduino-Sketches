#include <WiFiServer.h>
#include <ESP8266WebServer.h>

const char* ssid = "your SSID";  //  your network SSID (name)
const char* pass = "your PASS";  // your network password (use for WPA, or use as key for WEP)

int led = BUILTIN_LED;

WiFiServer server(80);

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  // Initialize serial and wait for port to open:
  Serial.begin(115200);

  // wait for serial port to connect. Needed for native USB port only
  while (!Serial) { ; }

  Serial.println();
  Serial.println("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, pass) ? "Ready" : "Failed!");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // start the server:
  server.begin();
  Serial.println("Server started");
  digitalWrite(led, 1);
}

int s,e;
String msg, ser;
char p;

String htmlF = "<!DOCTYPE html> <html lang='en'> <head> <title>OnlineChatServer</title> </head> <style> body { overflow: hidden; height: 100vw; display: flex; flex-direction: column; background-color: rgb(54, 54, 54); } .top { height: 50px; display: flex; background: rgb(255, 108, 108); color: aliceblue; justify-content: center; } h1 { margin: 0; align-self: center; } .conv { display: flex; padding: 0 20px 0 20px; margin: 20px 0 10px 0; overflow-y: scroll; height: 450px; color: aliceblue; align-self: left; flex-direction: column; font-family: Consolas, 'Courier New', monospace; } ::-webkit-scrollbar { width: 6px; } ::-webkit-scrollbar-track { background: #666; border-radius: 30px; } ::-webkit-scrollbar-thumb { background: white; border-radius: 30px; } ::-webkit-scrollbar-thumb:hover { background: linear-gradient(skyblue, yellowgreen); } .text { display: block; width: auto; max-width: 80%; height: auto; padding: 10px; margin: 3px; } .left { align-self: flex-start; text-align: right; background-color: rgb(69, 69, 69); } .right { align-self: flex-end; text-align: right; background-color: rgb(41, 105, 255); } .message { display: flex; text-align: center; height: 50px; padding: 2.5px; display: flex; justify-content: center; } .in { width: 500px; height: 45px; } .send { font-size: small; font-weight: bold; color: whitesmoke; background-color: lightseagreen; border-color: red; width: 100px; height: 50px; border-radius: 18px; } #rcv { font-size: xx-small; color: red; } </style> <body> <div class='top'> <h1>HASH's Chat Room</h1> </div> <div class='conv'> <div class='text left'>Hi client!</div>";
String htmlL = " </div> <div class='message'> <form> <input class='in' type='textarea' name='msg' placeholder='Write a message...' /> <button class='send' type='submit'>Send/Receive</button> </form> </div> </body> </html>";
String html=htmlF+htmlL;

char convert(char a,char b)
{
  char c=0;
  if ('A'<=a && a<='F') c+=(a-'A'+10)*16;
  else c+=(a-'0')*16;
  if ('A'<=b && b<='F') c+=(b-'A'+10);
  else c+=(b-'0');
  return c;
}

void newHtmlRecieve(String msg) {
  Serial.print("Client: ");
  htmlF += "<div class='text right'>";
  for (int i = 0; msg[i] != '\0'; i++) {
    p=msg[i];
    if (p == '+') p=' ';
    if (p == '%')
    {
      p=convert(msg[i+1],msg[i+2]);
      i+=2;
    }
    Serial.print(p);
    htmlF+=p;
  }
  Serial.println();
  htmlF+="</div>";
  html=htmlF + htmlL;
}

void newHtmlSend() {
  if (!Serial.available()) {
    delay(1);
  }
  ser = Serial.readStringUntil('\n');
  Serial.print("You: ");
  Serial.println(ser);
  htmlF += "<div class='text left'>" + ser + "</div>";
  html=htmlF + htmlL;
}

void loop() {
  WiFiClient client = server.available();
  // Check if a client has connected
  if (!client) {
      if (!Serial.available()) return;
      newHtmlSend();
      client.print("HTTP/1.1 200 OK\r\n");
      client.print("Content-Type: text/html\r\n\r\n");
      client.print(html);
    return;
  }

  // Serial.println(client);
  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  // Serial.println(req);
  // Match the request

 if (req.indexOf("") != -1) {  //checks if you're on the main page
    s = req.indexOf("/?msg=");
    if (s != -1 && req.indexOf("favicon.ico") == -1) {
      e = req.indexOf(" HTTP/1.1");
      s += strlen("/?msg=");
      while(req[s]=='+') s++;
      msg = "";
      for (int i = s; i < e; i++) {
        msg += req[i];
      }
      if(msg!="") newHtmlRecieve(msg);
    }
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: text/html\r\n\r\n");
  client.print(html);
}
