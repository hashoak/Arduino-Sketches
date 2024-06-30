#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <BleMouse.h>

Adafruit_MPU6050 mpu;
BleMouse bleMouse("Hash's Mouse", "Hash_Oak", 100);

int outpin1 = 36;  // Touch output detect pin 1
int outpin2 = 39;  // Touch output detect pin 2
int inpin1 = 34;   // Touch input detect pin 1
int inpin2 = 35;    // Touch input detect pin 2
int inpin3 = 32;    // Touch input detect pin 3

int mt = 500;
int ut = 2500;      // Touch upper threshold value
int lt = 1500;      // Touch lower threshold value
int wt = 100;       // Wait time for next reading
int cf = 15;         // Cursor multipier factor
float st = 1 / cf;  // Sensor movement threshold value

void setup() {
  Serial.begin(115200);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(outpin1, INPUT);
  pinMode(outpin2, INPUT);
  pinMode(inpin1, INPUT);
  pinMode(inpin2, INPUT);
  pinMode(inpin3, INPUT);

  Serial.println("Starting BLE...");
  bleMouse.begin();
  while (!bleMouse.isConnected()) delay(10);
  Serial.println("BLE Connected...");
}

int o1, o2, i1, i2, i3;

void loop() {
  // delay(wt);
  // cursor();
  // return;
  if (!bleMouse.isConnected()) return;
  o1 = analogRead(outpin1);
  o2 = analogRead(outpin2);
  i1 = analogRead(inpin1);
  i2 = analogRead(inpin2);
  i3 = analogRead(inpin3);
  if (i1 < mt && i2 < mt && i3 < mt) return;
  // Serial.print("rl:0,ru:4096,");
  // Serial.printf("o1:%d,o2:%d,i1:%d,i2:%d,i3:%d\n", o1, o2, i1, i2, i3);
  if (i1 > lt && o1 < ut) {
    Serial.println("Left click");
    Serial.printf("%d\t%d\n", i1, o1);
    bleMouse.press(MOUSE_LEFT);
    delay(wt);
    while (analogRead(inpin1) > lt) cursor();
    bleMouse.release(MOUSE_LEFT);
  } else if (i2 > lt && o1 < ut) {
    Serial.println("Cursor");
    Serial.printf("%d\t%d\n", i2, o1);
    delay(wt);
    while (analogRead(inpin2) > lt) cursor();
  } else if (i3 > lt && o1 < ut) {
    Serial.println("Right click");
    Serial.printf("%d\t%d\n", i3, o1);
    bleMouse.press(MOUSE_RIGHT);
    delay(wt);
    while (analogRead(inpin3) > lt) cursor();
    bleMouse.release(MOUSE_RIGHT);
  } else if (i1 > lt && o2 < ut) {
    Serial.print("Extra 1  ");
    Serial.printf("%d\t%d\n", i1, o2);
    bleMouse.press(MOUSE_BACK);
    delay(wt);
    while (analogRead(inpin1) > lt) cursor();
    bleMouse.release(MOUSE_BACK);
  } else if (i2 > lt && o2 < ut) {
    Serial.print("Extra 2  ");
    Serial.printf("%d\t%d\n", i2, o2);
    bleMouse.press(MOUSE_MIDDLE);
    delay(wt);
    while (analogRead(inpin2) > lt) cursor();
    bleMouse.release(MOUSE_MIDDLE);
  } else if (i3 > lt && o2 < ut) {
    Serial.print("Extra 3  ");
    Serial.printf("%d\t%d\n", i3, o2);
    bleMouse.press(MOUSE_FORWARD);
    delay(wt);
    while (analogRead(inpin3) > lt) cursor();
    bleMouse.release(MOUSE_FORWARD);
  }
}

sensors_event_t a, g, temp;

void cursor() {
  mpu.getEvent(&a, &g, &temp);
  // Serial.print("rl:-10,ru:10,");
  // Serial.printf("acc_x:%f,acc_y:%f,acc_z:%f,gyr_x:%f,gyr_y:%f,gyr_z:%f\n",
  //               a.acceleration.x,a.acceleration.y,a.acceleration.z,
  //               g.gyro.x,g.gyro.y,g.gyro.z);
  if (g.gyro.z < -st) Serial.println("Move Up");
  if (g.gyro.z > st) Serial.println("Move Down");
  if (g.gyro.y < -st) Serial.println("Move Left");
  if (g.gyro.y > st) Serial.println("Move Right");
  bleMouse.move(g.gyro.y * cf, g.gyro.z * cf);
}