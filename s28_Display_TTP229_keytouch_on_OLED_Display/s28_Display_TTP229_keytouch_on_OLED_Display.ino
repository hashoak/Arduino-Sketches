#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCL_PIN 21
#define SDO_PIN 20
byte key;

#define OLED_RESET -1
Adafruit_SSD1306 display(128,64,&Wire,OLED_RESET);
GFXcanvas1 canvas(128,64);

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
  display.setTextSize(2);
  display.cp437(true);

  canvas.fillScreen(0);
  canvas.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
  canvas.setTextSize(2);
  canvas.cp437(true);
  canvas.drawRect(1,1,30,30,SSD1306_WHITE);
  canvas.drawRect(33,1,30,30,SSD1306_WHITE);
  canvas.drawRect(65,1,30,30,SSD1306_WHITE);
  canvas.drawRect(97,1,30,30,SSD1306_WHITE);
  canvas.drawRect(1,33,30,30,SSD1306_WHITE);
  canvas.drawRect(33,33,30,30,SSD1306_WHITE);
  canvas.drawRect(65,33,30,30,SSD1306_WHITE);
  canvas.drawRect(97,33,30,30,SSD1306_WHITE);
  drawCentreString("1",16,16);
  drawCentreString("2",48,16);
  drawCentreString("3",80,16);
  drawCentreString("4",112,16);
  drawCentreString("5",16,48);
  drawCentreString("6",48,48);
  drawCentreString("7",80,48);
  drawCentreString("8",112,48);

  Serial.begin(9600);
  pinMode(SCL_PIN,OUTPUT);
  pinMode(SDO_PIN,INPUT);
}

void loop()
{
  display.drawBitmap(0,0,canvas.getBuffer(),128,64,SSD1306_WHITE,SSD1306_BLACK);
  display.display();
  key=read_keypad();
  if(key)
  {
    if(key==1) display.fillRect(1,1,30,30,SSD1306_WHITE);
    else if(key==2) display.fillRect(33,1,30,30,SSD1306_WHITE);
    else if(key==3) display.fillRect(65,1,30,30,SSD1306_WHITE);
    else if(key==4) display.fillRect(97,1,30,30,SSD1306_WHITE);
    else if(key==5) display.fillRect(1,33,30,30,SSD1306_WHITE);
    else if(key==6) display.fillRect(33,33,30,30,SSD1306_WHITE);
    else if(key==7) display.fillRect(65,33,30,30,SSD1306_WHITE);
    else if(key==8) display.fillRect(97,33,30,30,SSD1306_WHITE);
    display.display();
    delay(50);
  }
}

void drawCentreString(const char *buf,int x,int y)
{
  int16_t x1,y1;
  uint16_t w,h;
  canvas.getTextBounds(buf,x,y,&x1,&y1,&w,&h);
  canvas.setCursor(x-w/2,y-h/2);
  canvas.print(buf);
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
