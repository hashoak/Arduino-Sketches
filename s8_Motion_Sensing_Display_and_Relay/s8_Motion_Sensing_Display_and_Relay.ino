#include "SevSeg.h"
SevSeg sevseg;

int motionSensor=20;
int reading;
unsigned long previousMillis=0;
const long interval=10000;
int relay=18;

void setup()
{
  pinMode(motionSensor,INPUT);
  pinMode(relay,OUTPUT);
  byte numDigits=4;
  byte digitPins[]={2, 3, 4, 5};
  byte segmentPins[]={6, 8, 10, 12, 14,7,9,11};
  bool resistorsOnSegments=true;
  byte hardwareConfig=COMMON_CATHODE;
  bool updateWithDelays=false;
  bool leadingZeros=false;
  bool disableDecPoint=false;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void loop()
{
  unsigned long currentMillis=millis();
  reading=digitalRead(motionSensor);
  if(reading | currentMillis-previousMillis>=interval)
  {
    previousMillis=currentMillis;
    if(reading)
    {
      sevseg.setChars("On");
      digitalWrite(relay,LOW);
    }
    else
    {
      sevseg.setChars("Off");
      digitalWrite(relay,HIGH);
    }
  }
  sevseg.refreshDisplay();
}