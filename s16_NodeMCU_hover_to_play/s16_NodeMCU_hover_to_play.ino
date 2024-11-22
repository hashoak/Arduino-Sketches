// Successfull
#include <ESP8266WiFi.h>
#include <pitches.h>
#include <themes2.h>

const char* ssid = "your SSID";
const char* password = "your PASS";
const char* host = "192.168.78.160";  //it will tell you the IP once it starts up
                                      //just write it here afterwards and upload
int ledr = D3;
int ledg = D2;
int ledy = D1;
int ledb = D4;
bool stater = 1;
bool stateg = 1;
bool statey = 1;
bool stateb = 0;
int speaker = D7;

void desp() {
  for (int thisNote = 0; thisNote < (sizeof(desp_note) / sizeof(int)); thisNote++) {

    int noteDuration = 1000 / desp_duration[thisNote];
    if (thisNote % 3 == 0) {


      tone(speaker, desp_note[thisNote], noteDuration);
    }
    if (thisNote % 4 == 0) {


      tone(speaker, desp_note[thisNote], noteDuration);
    }
    if (thisNote % 5 == 0) {


      tone(speaker, desp_note[thisNote], noteDuration);
    } else {


      tone(speaker, desp_note[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 1.50;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

WiFiServer server(1234);  //just pick any port number you like

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println(WiFi.localIP());
  // prepare GPIO2
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledy, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(speaker, OUTPUT);
  digitalWrite(ledr,stater);
  digitalWrite(ledg,stateg);
  digitalWrite(ledy,statey);
  digitalWrite(ledb,stateb);
  digitalWrite(speaker, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  digitalWrite(ledr,!stater);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(ledg,0);

  // Start the server
  server.begin();
  Serial.println("Server started");
  digitalWrite(ledy,0);

  // Print the IP address
  Serial.println(WiFi.localIP());
  digitalWrite(ledb,1);
}

void loop() {
  // Check if a client has connected

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();
  Serial.println(req);
  // Match the request
  if (req.indexOf("") != -1) {  //checks if you're on the main page

    if (req.indexOf("/red") != -1) {  //checks if you clicked OFF
      stater=!stater;
      digitalWrite(ledr,stater);
      Serial.println("You clicked red");
    }
    if (req.indexOf("/green") != -1) {  //checks if you clicked ON
      stateg=!stateg;
      digitalWrite(ledg,stateg);
      Serial.println("You clicked green");
    }
    if (req.indexOf("/yellow") != -1) {  //checks if you clicked ON
      statey=!statey;
      digitalWrite(ledy,statey);
      Serial.println("You clicked yellow");
    }
    if (req.indexOf("/blue") != -1) {  //checks if you clicked ON
      stateb=!stateb;
      digitalWrite(ledb,stateb);
      Serial.println("You clicked blue");
    }
    if (req.indexOf("/apuPuka") != -1) {  //checks if you clicked ON
      digitalWrite(ledr,0);
      digitalWrite(ledg,0);
      digitalWrite(ledy,0);
      digitalWrite(ledb,1);
      Serial.println("You clicked pataPadu");
    }
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE html><html lang='en'><head><title>LearnCodeOnline</title></head> <style> body { background: rgb(66, 66, 66); min-height: 100vh; display: flex; justify-content: center; align-items: center; position: relative; }  .color { width: 120px; height: 30px; border-radius: 5%; margin-bottom: 30px; } </style> <body> <div class='color-hover'> <div style='cursor: pointer; background-color: red' class='color' name='color' ></div> <div style='cursor: pointer; background-color: green' class='color' name='color' ></div> <div style='cursor: pointer; background-color: yellow' class='color' name='color' ></div> <div style='cursor: pointer; background-color: blue' class='color' name='color' ></div> <div style='cursor: pointer; background-color: pink' class='color' name='color' > <p style='text-align: center'>Dj Dengu</p> </div> <div style='cursor: pointer; background-color: orange' class='color' name='color' > <p style='text-align: center'>Apey puka</p> </div> </div> <script> let color = document.getElementsByName('color'); color.forEach((ele) => { ele.addEventListener('mouseover', () => { if (ele.style.backgroundColor == 'pink') { location.href = '/pataPadu'; return; } if (ele.style.backgroundColor == 'orange') { location.href = '/apuPuka'; return; } location.href = `/${ele.style.backgroundColor}`; }); }); </script> </body> </html> ";
  client.flush();


  // Send the response to the client
  client.print(s);
  delay(1);
}
