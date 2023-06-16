#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(128,64,&Wire,OLED_RESET);

MPU6050 mpu6050(Wire);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

float xang,yang;
int prot=0,rot=0;

void loop() {
  mpu6050.update();
  xang=mpu6050.getAngleX();
  yang=mpu6050.getAngleY();

  if(yang>45 && abs(xang)<45) rot=0;
  else if(xang>45 && abs(yang)<45) rot=1;
  else if(yang<-45 && abs(xang)<45) rot=2;
  else if(xang<-45 && abs(yang)<45) rot=3;
  if(prot!=rot)
  {
    Serial.print(xang);
    Serial.print("\t");
    Serial.println(yang);
    prot=rot;
    display.setRotation(rot);
    displayMessage("#HASH");
    display.display();
  }
}

void displayMessage(String msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.cp437(true);
  display.println(msg);
}
