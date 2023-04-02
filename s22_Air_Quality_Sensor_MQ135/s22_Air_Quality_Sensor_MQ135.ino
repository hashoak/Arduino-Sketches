#include "MQ135.h"

const int MQ135pin=A7;
const int led=3;
MQ135 gasSensor = MQ135(MQ135pin);
double ppm;
int n=100;

void setup()
{
  Serial.begin(9600);
  pinMode(MQ135pin,INPUT);
  pinMode(led,OUTPUT);
}

void loop()
{
  ppm=0;
  for(int i=0;i<n;i++)
  {
    ppm+=gasSensor.getPPM();
    delay(1);
  }
  ppm/=n;
  // Serial.println(ppm);
  if(ppm>20000) digitalWrite(led,1);
  else digitalWrite(led,0);
}