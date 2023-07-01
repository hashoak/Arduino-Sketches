#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_tockn.h>

#define SCL_PIN 21
#define SDO_PIN 20
byte key;

#define OLED_RESET -1
Adafruit_SSD1306 display(128,64,&Wire,OLED_RESET);
GFXcanvas1 digitKeys[]={GFXcanvas1(32,32),GFXcanvas1(32,32),GFXcanvas1(32,32),GFXcanvas1(32,32),GFXcanvas1(32,32),GFXcanvas1(32,32),GFXcanvas1(32,32),GFXcanvas1(32,32)};
// GFXcanvas1* digitKeys=(GFXcanvas1*)malloc(8*sizeof(GFXcanvas1));
uint8_t* keyBuf[8];
GFXcanvas1 rotCanvas[]={GFXcanvas1(128,64),GFXcanvas1(64,128)};
// GFXcanvas1* rotCanvas=(GFXcanvas1*)malloc(2*sizeof(GFXcanvas1));
uint8_t* rotBuf[2];
char buf[]="0";
int dim[]={128,64};
int pos[]={0,32,64,96};
int a;

int speaker=10;

MPU6050 mpu6050(Wire);
float xang,yang;
int prot=1,rot=0;

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
  display.setTextSize(1);
  display.cp437(true);
  display.println("Calibrating...");
  display.display();

  pinMode(SCL_PIN,OUTPUT);
  pinMode(SDO_PIN,INPUT);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets();
  // mpu6050.calcGyroOffsets(true);

  for(int i=0;i<8;i++)
  {
    // digitKeys[i]=GFXcanvas1(32,32);
    digitKeys[i].fillScreen(0);
    digitKeys[i].setTextColor(SSD1306_WHITE,SSD1306_BLACK);
    digitKeys[i].setTextSize(2);
    digitKeys[i].cp437(true);
    digitKeys[i].drawRect(1,1,30,30,SSD1306_WHITE);
    buf[0]='1'+i;
    drawCentreString(&digitKeys[i],16,16);
    keyBuf[i]=digitKeys[i].getBuffer();
  }

  for(int i=0;i<2;i++)
  {
    (i%2) ? a=2:a=4;
    // rotCanvas[i]=GFXcanvas1(dim[i%2],dim[(i+1)%2]);
    rotCanvas[i].fillScreen(0);
    rotCanvas[i].setTextColor(SSD1306_WHITE,SSD1306_BLACK);
    rotCanvas[i].setTextSize(2);
    rotCanvas[i].cp437(true);
    for(int j=0;j<8;j++)
    {
      rotCanvas[i].drawBitmap(pos[j%a],pos[j/a],keyBuf[j],32,32,SSD1306_WHITE,SSD1306_BLACK);
    }
    rotBuf[i]=rotCanvas[i].getBuffer();
  }
}

void loop()
{
  mpu6050.update();
  xang=mpu6050.getAngleX();
  yang=mpu6050.getAngleY();
  if(yang>45 && abs(xang)<45) rot=0;
  else if(xang>45 && abs(yang)<45) rot=1;
  else if(yang<-45 && abs(xang)<45) rot=2;
  else if(xang<-45 && abs(yang)<45) rot=3;

  if(prot!=rot)
  {
    // Serial.print(xang);
    // Serial.print("\t");
    // Serial.println(yang);
    prot=rot;
    display.setRotation(rot);
    display.drawBitmap(0,0,rotBuf[rot%2],dim[rot%2],dim[(rot+1)%2],SSD1306_WHITE,SSD1306_BLACK);
    display.display();
  }

  key=read_keypad();
  if(key)
  {
    tone(speaker,5);
    (rot%2) ? a=2:a=4;
    display.fillRect(pos[(key-1)%a]+1,pos[(key-1)/a]+1,30,30,SSD1306_WHITE);
    display.display();
    delay(100);
    display.drawBitmap(pos[(key-1)%a],pos[(key-1)/a],keyBuf[key-1],32,32,SSD1306_WHITE,SSD1306_BLACK);
    display.display();
    noTone(speaker);
  }
}

void drawCentreString(GFXcanvas1* canvas,int x,int y)
{
  int16_t x1,y1;
  uint16_t w,h;
  canvas->getTextBounds(buf,x,y,&x1,&y1,&w,&h);
  canvas->setCursor(x-w/2,y-h/2);
  canvas->print(buf);
}

byte read_keypad(void)
{
  byte key_state=0;
  for(byte count=1;count<=8;count++)
  {
    digitalWrite(SCL_PIN,LOW);
    if(!digitalRead(SDO_PIN)) key_state=count;
    digitalWrite(SCL_PIN,HIGH);
  }
  return key_state;
}
