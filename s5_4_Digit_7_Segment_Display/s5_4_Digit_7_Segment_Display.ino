#include "SevSeg.h"
SevSeg sevseg;

void setup()
{
  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 8, 10, 12, 14,7,9,11};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;
  bool updateWithDelays = false;
  bool leadingZeros = false;
  bool disableDecPoint = false;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void loop()
{
  sevseg.setNumber(1974,-1);
  sevseg.refreshDisplay();
}