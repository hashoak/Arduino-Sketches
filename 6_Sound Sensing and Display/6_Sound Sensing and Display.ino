#include "SevSeg.h"
SevSeg sevseg;

int soundSensor=20;
int reading;
int dispState=LOW;
unsigned long previousMillis=0;
const long interval=500;

void setup()
{
  pinMode(soundSensor,INPUT);
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
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis=millis();
  reading=analogRead(soundSensor);

  if(reading>200 && currentMillis-previousMillis>=interval)
  {
    Serial.println(reading);
    previousMillis=currentMillis;
    if(reading>200)
    {
      if(dispState==false)
      {
        sevseg.setChars("Off");
        dispState=true;
      }
      else
      {
        sevseg.setChars("On");
        dispState=false;
      }
    }
  }
  // for(int i=0;i<500;i++)
  // {
  //   if(dispState==false) sevseg.setChars("Off");
  //   else sevseg.setChars("On");
  //   delay(1);
  //   sevseg.refreshDisplay();
  // }
  sevseg.refreshDisplay();
}