#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

int in1=A1;
int in2=A2;
int in3=A3;
int out=2;
int t=10;
int ut=550;
int lt=275;
float st=2;

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
  
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(3,1); delay(100); digitalWrite(3,0);
  digitalWrite(4,1); delay(100); digitalWrite(4,0);
  digitalWrite(5,1); delay(100); digitalWrite(5,0);
  digitalWrite(6,1); delay(100); digitalWrite(6,0);

  pinMode(in1,INPUT);
  pinMode(in2,INPUT);
  pinMode(in3,INPUT);
  pinMode(out,OUTPUT);
  digitalWrite(out,1);
}

int x1,x2,x3;
long s1,s2,s3;

void loop() {
  // s1=analogRead(in1);
  // s2=analogRead(in2);
  // s3=analogRead(in3);
  s1=0,s2=0,s3=0;
  for(int i=0;i<t;i++) s1+=analogRead(in1),s2+=analogRead(in2),s3+=analogRead(in3),delay(1);
  s1/=t,s2/=t,s3/=t;
  // Serial.print(s1);
  // Serial.print("\t");
  // Serial.print(s2);
  // Serial.print("\t");
  // Serial.println(s3);
  // scroll();
  if(s1>ut && s2<lt && s3<lt) Serial.println("Right click"),delay(300);
  else if(s1<lt && s2>ut && s3<lt) scroll();
  else if(s1<lt && s2<lt && s3>ut) Serial.println("Left click"),delay(300);
  else if(s1>lt && s2<lt && s3<lt) Serial.println("Extra 1"),delay(300);//,Serial.println(s1);
  else if(s1<lt && s2>lt && s3<lt) Serial.println("Extra 2"),delay(300);//,Serial.println(s2);
  else if(s1<lt && s2<lt && s3>lt) Serial.println("Extra 3"),delay(300);//,Serial.println(s3);

}

float xgyro,ygyro,zgyro;
sensors_event_t a,g,temp;

void scroll()
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
    digitalWrite(3,0); digitalWrite(4,1);
    digitalWrite(5,0); digitalWrite(6,0);
    Serial.println("Scroll Up");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(zgyro);
  }
  if(zgyro>st)
  {
    digitalWrite(3,0); digitalWrite(4,0);
    digitalWrite(5,1); digitalWrite(6,0);
    Serial.println("Scroll Down");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(zgyro);
  }
  if(ygyro<-st)
  {
    digitalWrite(3,1); digitalWrite(4,0);
    digitalWrite(5,0); digitalWrite(6,0);
    Serial.println("Scroll Left");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(ygyro);
  }
  if(ygyro>st)
  {
    digitalWrite(3,0); digitalWrite(4,0);
    digitalWrite(5,0); digitalWrite(6,1);
    Serial.println("Scroll Right");
    // Serial.print(ygyro);
    // Serial.print("\t");
    // Serial.println(zgyro);
    // Serial.println(ygyro);
  }
}