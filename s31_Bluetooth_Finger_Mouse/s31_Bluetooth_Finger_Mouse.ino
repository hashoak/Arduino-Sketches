#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <BleMouse.h>

Adafruit_MPU6050 mpu;
BleMouse bleMouse("Hash's Mouse", "Hash_Oak", 100);

int inpin1=4;
int inpin2=2;
int inpin3=15;
// int outpin=16;

int upled=5;
int downled=18;
int leftled=19;
int rightled=17;

int t=10;
int mt=1000;
int ut=3600;
int lt=1800;
float st=2;
float cf=5;

void setup() {
  Serial.begin(115200);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Starting BLE work!");
  bleMouse.begin();
  
  pinMode(upled,OUTPUT);
  pinMode(downled,OUTPUT);
  pinMode(leftled,OUTPUT);
  pinMode(rightled,OUTPUT);
  digitalWrite(upled,1); delay(100); digitalWrite(upled,0);
  digitalWrite(downled,1); delay(100); digitalWrite(downled,0);
  digitalWrite(leftled,1); delay(100); digitalWrite(leftled,0);
  digitalWrite(rightled,1); delay(100); digitalWrite(rightled,0);

  pinMode(inpin1,INPUT);
  pinMode(inpin2,INPUT);
  pinMode(inpin3,INPUT);
  // pinMode(outpin,OUTPUT);
  // digitalWrite(outpin,1);
}

int v1,v2,v3;

void loop() {
  // cursor();
  // s1=0,s2=0,s3=0;
  // for(int i=0;i<t;i++) s1+=analogRead(inpin1),s2+=analogRead(inpin2),s3+=analogRead(inpin3),delay(1);
  // s1/=t,s2/=t,s3/=t;
  // Serial.print(analogRead(inpin1));
  // Serial.print("\t");
  // Serial.print(analogRead(inpin2));
  // Serial.print("\t");
  // Serial.println(analogRead(inpin3));
  // // delay(100);
  // return;
  if(!bleMouse.isConnected()) return;
  v1=analogRead(inpin1);
  v2=analogRead(inpin2);
  v3=analogRead(inpin3);
  if(v1<mt && v2<mt && v3<mt) return;
  // delay(3);
  if(v1>ut && v2<lt && v3<lt)
  {
    Serial.println("Left click");
    bleMouse.press(MOUSE_LEFT);
    while(analogRead(inpin1)>ut) cursor();
    bleMouse.release(MOUSE_LEFT);
  }
  else if(v1<lt && v2>ut && v3<lt)
  {
    Serial.println("Cursor");
    while(analogRead(inpin2)>ut) cursor();
  }
  else if(v1<lt && v2<lt && v3>ut)
  {
    Serial.println("Right click");
    bleMouse.press(MOUSE_RIGHT);
    while(analogRead(inpin3)>ut) cursor();
    bleMouse.release(MOUSE_RIGHT);
  }
  else if(v1>lt && v2<lt && v3<lt)
  {
    Serial.print("Extra 1  ");
    Serial.println(v1);
    bleMouse.press(MOUSE_BACK);
    while(analogRead(inpin1)>lt) cursor();
    bleMouse.release(MOUSE_BACK);
  }
  else if(v1<lt && v2>lt && v3<lt)
  {
    Serial.print("Extra 2  ");
    Serial.println(v2);
    bleMouse.press(MOUSE_MIDDLE);
    while(analogRead(inpin2)>lt) cursor();
    bleMouse.release(MOUSE_MIDDLE);
  }
  else if(v1<lt && v2<lt && v3>lt)
  {
    Serial.print("Extra 3  ");
    Serial.println(v3);
    bleMouse.press(MOUSE_FORWARD);
    while(analogRead(inpin3)>lt) cursor();
    bleMouse.release(MOUSE_FORWARD);
  }
  delay(300);
}

float xgyro,ygyro,zgyro;
sensors_event_t a,g,temp;

void cursor()
{
  mpu.getEvent(&a,&g,&temp);
  xgyro=g.gyro.x;
  ygyro=g.gyro.y;
  zgyro=g.gyro.z;
  // Serial.print(xgyro);
  // Serial.print("\t");
  // Serial.print(ygyro);
  // Serial.print("\t");
  // Serial.println(zgyro);
  if(zgyro<-st)
  {
    digitalWrite(upled,0); digitalWrite(downled,1);
    digitalWrite(leftled,0); digitalWrite(rightled,0);
    Serial.println("Move Up");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(zgyro);
  }
  if(zgyro>st)
  {
    digitalWrite(upled,0); digitalWrite(downled,0);
    digitalWrite(leftled,1); digitalWrite(rightled,0);
    Serial.println("Move Down");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(zgyro);
  }
  if(ygyro<-st)
  {
    digitalWrite(upled,1); digitalWrite(downled,0);
    digitalWrite(leftled,0); digitalWrite(rightled,0);
    Serial.println("Move Left");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(ygyro);
  }
  if(ygyro>st)
  {
    digitalWrite(upled,0); digitalWrite(downled,0);
    digitalWrite(leftled,0); digitalWrite(rightled,1);
    Serial.println("Move Right");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(ygyro);
  }
  bleMouse.move(ygyro*cf,zgyro*cf);
}