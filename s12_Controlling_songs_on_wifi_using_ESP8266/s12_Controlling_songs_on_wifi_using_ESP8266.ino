#include <pitches.h>
#include <themes2.h>
#include <SoftwareSerial.h>
SoftwareSerial esp8266(2, 3);
#define serialCommunicationSpeed 9600
#define DEBUG true

int speaker = 10;

void poc() {
  for (int thisNote = 0; thisNote < (sizeof(Pirates_note) / sizeof(int)); thisNote++) {

    int noteDuration = 1000 / Pirates_duration[thisNote];
    if (thisNote % 3 == 0) {


      tone(speaker, Pirates_note[thisNote], noteDuration);
    }
    if (thisNote % 4 == 0) {


      tone(speaker, Pirates_note[thisNote], noteDuration);
    }
    if (thisNote % 5 == 0) {


      tone(speaker, Pirates_note[thisNote], noteDuration);
    } else {


      tone(speaker, Pirates_note[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 1.05;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

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

void cf() {
  for (int thisNote = 0; thisNote < (sizeof(CrazyFrog_note) / sizeof(int)); thisNote++) {

    int noteDuration = 1000 / CrazyFrog_duration[thisNote];
    if (thisNote % 3 == 0) {


      tone(speaker, CrazyFrog_note[thisNote], noteDuration);
    }
    if (thisNote % 4 == 0) {


      tone(speaker, CrazyFrog_note[thisNote], noteDuration);
    }
    if (thisNote % 5 == 0) {


      tone(speaker, CrazyFrog_note[thisNote], noteDuration);
    } else {


      tone(speaker, CrazyFrog_note[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

void mario() {
  for (int thisNote = 0; thisNote < (sizeof(MarioUW_note) / sizeof(int)); thisNote++) {

    int noteDuration = 1000 / MarioUW_duration[thisNote];
    if (thisNote % 3 == 0) {


      tone(speaker, MarioUW_note[thisNote], noteDuration);
    }
    if (thisNote % 4 == 0) {


      tone(speaker, MarioUW_note[thisNote], noteDuration);
    }
    if (thisNote % 5 == 0) {


      tone(speaker, MarioUW_note[thisNote], noteDuration);
    } else {


      tone(speaker, MarioUW_note[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 1.80;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

void ttn() {
  for (int thisNote = 0; thisNote < (sizeof(Titanic_note) / sizeof(int)); thisNote++) {

    int noteDuration = 1000 / Titanic_duration[thisNote];
    if (thisNote % 3 == 0) {


      tone(speaker, Titanic_note[thisNote], noteDuration);
    }
    if (thisNote % 4 == 0) {


      tone(speaker, Titanic_note[thisNote], noteDuration);
    }
    if (thisNote % 5 == 0) {


      tone(speaker, Titanic_note[thisNote], noteDuration);
    } else {


      tone(speaker, Titanic_note[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 2.70;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

void newn() {
  for (int thisNote = 0; thisNote < (sizeof(new_note) / sizeof(int)); thisNote++) {

    int noteDuration = 1000 / new_duration[thisNote];
    if (thisNote % 3 == 0) {


      tone(speaker, new_note[thisNote], noteDuration);
    }
    if (thisNote % 4 == 0) {


      tone(speaker, new_note[thisNote], noteDuration);
    }
    if (thisNote % 5 == 0) {


      tone(speaker, new_note[thisNote], noteDuration);
    } else {


      tone(speaker, new_note[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 2.70;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

int redLED = 11;
void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(speaker, OUTPUT);

  digitalWrite(redLED, HIGH);
  digitalWrite(speaker, LOW);
  Serial.begin(serialCommunicationSpeed);
  esp8266.begin(serialCommunicationSpeed);
  InitWifiModule();
  digitalWrite(redLED, LOW);
}
void loop() {
  if (esp8266.available()) {
    if (esp8266.find("+IPD,")) {
      delay(1000);
      int connectionId = esp8266.read() - 48;
      esp8266.find("pin=");
      int songNo = (esp8266.read() - 48) * 10;
      songNo = songNo + (esp8266.read() - 48);
      esp8266.read();
      Serial.println(songNo);
      digitalWrite(redLED, HIGH);
      if (songNo == 1) {
        Serial.println("Playing Pirates of Caribbean.");
        poc();
      } else if (songNo == 2) {
        Serial.println("Playing Despacito.");
        desp();
      } else if (songNo == 3) {
        Serial.println("Playing Crazy Frog.");
        cf();
      } else if (songNo == 4) {
        Serial.println("Playing Mario.");
        mario();
      } else if (songNo == 5) {
        Serial.println("Playing Titatic.");
        ttn();
      } else if (songNo == 6) {
        Serial.println("Playing Newn.");
        newn();
      } else if (songNo) Serial.println("Wrong key.");
      digitalWrite(redLED,LOW);
      Serial.println(connectionId);
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId;
      closeCommand += "\r\n";
      sendData(closeCommand, 1000, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";

  esp8266.print(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (esp8266.available()) {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}

void InitWifiModule() {
  sendData("AT+RST\r\n", 2000, DEBUG);
  delay(1000);
  sendData("AT+CWJAP=\"ORBI12\",\"Mmfsl1234\"\r\n", 2000, DEBUG);
  delay(20000);
  sendData("AT+CWMODE=1\r\n", 1500, DEBUG);
  delay(1000);
  sendData("AT+CIFSR\r\n", 1500, DEBUG);
  delay(1000);
  sendData("AT+CIPMUX=1\r\n", 1500, DEBUG);
  delay(1000);
  sendData("AT+CIPSERVER=1,80\r\n", 1500, DEBUG);
}