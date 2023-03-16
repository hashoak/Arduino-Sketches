#include <Servo.h>

Servo myservo;
int led=5;
int ldr=A6;
float reading;
unsigned long time;
int n;
int accuracy=100;     // more the accuracy, more the delay scanning
int threshold=7.75;  // set according to the screen brightness
int servoPin=3;
int pos=170;    // adjust according to your setup                                              
int press=9;    // no.of degrees to be pressed

void setup()
{
  Serial.begin(9600);
  pinMode(ldr,INPUT);
  pinMode(led,OUTPUT);
  analogReference(EXTERNAL);
  myservo.attach(servoPin);
  myservo.write(pos);
}

void loop()           
{
  reading=0;
  time=millis();
  n=1;
  while(millis()-time<accuracy)
  {
    reading+=analogRead(ldr);
    n++;
  }
  reading/=n;

  // Serial.println(reading);       
  if(reading>threshold)
  {
    Serial.println(reading);
    digitalWrite(led,1);
    myservo.write(pos-press);
    delay(100);
    myservo.write(pos);
  }
  else digitalWrite(led,0);
}
