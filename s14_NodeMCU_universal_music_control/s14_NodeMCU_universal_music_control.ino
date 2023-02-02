// In progress
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <pitches.h>
#include <themes2.h>

unsigned long channel_num = 2001544;
const char* api_key = "RM3WOOPOQHIUUKUA";
const char* ssid = "hash test";
const char* password = "idontknow";
int led=D2;
int speaker=D7;
unsigned int value;
WiFiClient client;            // make the client of the WiFi which connect to the ThingSpeak webServer
ESP8266WebServer server(80);  // make the naother server

void poc()
{ 
  for (int thisNote = 0; thisNote < (sizeof(Pirates_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / Pirates_duration[thisNote];//convert duration to time delay
   if(thisNote%3==0){
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void desp()
{
  for (int thisNote = 0; thisNote < (sizeof(desp_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / desp_duration[thisNote];//convert duration to time delay
   if(thisNote%3==0){
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.50; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void cf()
{
  for (int thisNote = 0; thisNote < (sizeof(CrazyFrog_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / CrazyFrog_duration[thisNote]; //convert duration to time delay
 if(thisNote%3==0){
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.30;//Here 1.30 is tempo, decrease to play it faster
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void mario()
{
    for (int thisNote = 0; thisNote < (sizeof(MarioUW_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / MarioUW_duration[thisNote];//convert duration to time delay
     if(thisNote%3==0){
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.80;
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void ttn()
{
    for (int thisNote = 0; thisNote < (sizeof(Titanic_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / Titanic_duration[thisNote];//convert duration to time delay
     if(thisNote%3==0){
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 2.70;
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}
void newn()
{
    for (int thisNote = 0; thisNote < (sizeof(new_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / new_duration[thisNote];//convert duration to time delay
     if(thisNote%3==0){
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 2.70;
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

String SendHTML(void) {
  String str = "<!DOCTYPE html> <html>\n";
  str += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  str += "<title>LED Control</title>\n";
  str += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  str += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  str += ".button {display: block;width: 30%;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  str += ".button-1 {background-color: #1abc9c;}\n";
  str += ".button-1:active {background-color: #16a085;}\n";
  str += ".button-2 {background-color: #34495e;}\n";
  str += ".button-2:active {background-color: #2c3e50;}\n";
  str += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  str += "</style>\n";
  str += "</head>\n";
  str += "<body>\n";
  str += "<h1>ESP8266 with ThingSpeak Server</h1>\n";
  str += "<h3>Using Station(STA) Mode</h3>\n";
  str += "<h4>Select the song to play</h4>\n";
  str += "<a class=\"button button-1\" href='#' onClick=\"changePrev('https://api.thingspeak.com/update?api_key=RM3WOOPOQHIUUKUA&field1=1')\">Pirates of the Carribean</a>\n";
  str += "<a class=\"button button-2\" href='#' onClick=\"changePrev('https://api.thingspeak.com/update?api_key=RM3WOOPOQHIUUKUA&field1=2')\">Despacito</a>\n";
  str += "<a class=\"button button-1\" href='#' onClick=\"changePrev('https://api.thingspeak.com/update?api_key=RM3WOOPOQHIUUKUA&field1=3')\">Crazy Frog</a>\n";
  str += "<a class=\"button button-2\" href='#' onClick=\"changePrev('https://api.thingspeak.com/update?api_key=RM3WOOPOQHIUUKUA&field1=4')\">Mario theme</a>\n";
  str += "<a class=\"button button-1\" href='#' onClick=\"changePrev('https://api.thingspeak.com/update?api_key=RM3WOOPOQHIUUKUA&field1=5')\">Titanic theme</a>\n";
  str += "<a class=\"button button-2\" href='#' onClick=\"changePrev('https://api.thingspeak.com/update?api_key=RM3WOOPOQHIUUKUA&field1=6')\">Newn</a>\n";
  str += "<script> let changePrev = (link) => { let current = window.location.href; window.location.href = link; window.location.href = current; };</script>";
  str += "</body>\n";
  str += "</html>\n";
  return str;
}

void handleonconnect() {
  server.send(200, "text/html", SendHTML());
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(speaker, OUTPUT);
  digitalWrite(led,1);
  Serial.begin(115200);
  WiFi.begin(ssid, password);  // connect to the wifi STA connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to ");
  Serial.println(ssid);
  digitalWrite(led,0);
  Serial.print("Your IP is: ");
  Serial.println(WiFi.localIP());   // print the wifi local ip
  server.on("/", handleonconnect);  // in urt type the "/" then call the handle on connect function
  ThingSpeak.begin(client);         // connect the client to the thingSpeak server
  server.begin();                   // start the server
}

void loop() {
  server.handleClient();                            // it readly handle the Client
  value = ThingSpeak.readFloatField(channel_num,1);  // rad the last data of the field 1
  if(value) digitalWrite(led,1);
  if (value == 1) Serial.println("Playing Pirates of the Carribean"),poc();
  else if (value == 2) Serial.println("Playing Despacito"),desp();
  else if (value == 3) Serial.println("Playing Crazy Frog"),cf();
  else if (value == 4) Serial.println("Playing Mario theme"),mario();
  else if (value == 5) Serial.println("Playing Titanic theme"),ttn();
  else if (value == 6) Serial.println("Playing Newn"),newn();
  if(value) ThingSpeak.writeField(channel_num,1,0,api_key);
  digitalWrite(led,0);
}