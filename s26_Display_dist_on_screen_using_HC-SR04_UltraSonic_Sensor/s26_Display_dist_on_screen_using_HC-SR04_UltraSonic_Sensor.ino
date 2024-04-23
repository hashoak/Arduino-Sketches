#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(128,64,&Wire,OLED_RESET);
char buf[11];

const int trigPin = 9;
const int echoPin = 10;

int dist;
int n=100,i;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
  display.setTextSize(2);
  display.cp437(true);
  display.drawRect(3,9,120,12,SSD1306_WHITE);

  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  dist=0;
  for(i=0;i<n;i++)
  {
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    dist+=pulseIn(echoPin,HIGH)*0.034/2;
    delay(38);
  }
  dist/=n;
  if(dist>0 && dist<=400)
  {
    display.fillRect(5,11,116,8,SSD1306_BLACK);
    display.fillRect(5,11,116-pow(dist*33.64,0.5),8,SSD1306_WHITE);
    sprintf(buf,"Dist:%-3dcm",dist);
  }
  else
  {
    display.fillRect(5,11,116,8,SSD1306_BLACK);
    sprintf(buf,"Dist>400cm");
  }
  display.setCursor(3,39);
  display.print(buf);
  display.display();
}
