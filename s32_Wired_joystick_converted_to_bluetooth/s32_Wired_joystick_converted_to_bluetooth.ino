#include <NimBLEDevice.h>
#include <BleGamepad.h>

BleGamepad bleGamepad("Hash's Gamepad", "Hash_Oak", 100);
BleGamepadConfiguration bleGamepadConfig;

int inpin[2] = { 36, 39 };
int outpin[4] = { 34, 35, 32, 33 };
int lt = 50;
int ut = 4090;
int wt = 100;
int cm = 1;

void setup() {
  Serial.begin(115200);
  pinMode(inpin[0], INPUT);
  pinMode(inpin[1], INPUT);
  pinMode(outpin[0], INPUT);
  pinMode(outpin[1], INPUT);
  pinMode(outpin[2], INPUT);
  pinMode(outpin[3], INPUT);

  Serial.println("Starting BLE...");
  bleGamepadConfig.setAxesMin(0x0000);  // 0 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
  bleGamepadConfig.setAxesMax(0x7FFF);  // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
  bleGamepad.begin(&bleGamepadConfig);

  Serial.println("BLE Connecting...");
  while (!bleGamepad.isConnected()) delay(10);
  Serial.println("BLE Connected...");
}

bool p14 = 0, p13 = 0, p15 = 0, p16 = 0, pa = 0, pb = 0;

void loop() {
  if (!bleGamepad.isConnected()) return;
  // Serial.printf("%d\t%d\t%d\t%d\t%d\t%d\n",analogRead(inpin[0]),analogRead(inpin[1]),analogRead(outpin[0]),analogRead(outpin[1]),analogRead(outpin[2]),analogRead(outpin[3]));
  // delay(100);
  if (!pa && analogRead(inpin[0]) > lt && analogRead(outpin[2]) < ut) {
    Serial.println("A");
    bleGamepad.press(BUTTON_1);
    pa = 1;
    delay(wt);
  } else if (pa && (analogRead(inpin[0]) < lt || analogRead(outpin[2]) > ut)) {
    Serial.println("~A");
    bleGamepad.release(BUTTON_1);
    pa = 0;
  }
  if (!pb && analogRead(inpin[0]) > lt && analogRead(outpin[3]) < ut) {
    Serial.println("B");
    bleGamepad.press(BUTTON_2);
    pb = 1;
    delay(wt);
  } else if (pb && (analogRead(inpin[0]) < lt || analogRead(outpin[3]) > ut)) {
    Serial.println("~B");
    bleGamepad.release(BUTTON_2);
    pb = 0;
  }
  if (!p14 && analogRead(inpin[1]) > lt && analogRead(outpin[0]) < ut) {
    Serial.println("DOWN");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax() / 2, bleGamepadConfig.getAxesMax());
    p14 = 1;
    delay(wt);
  } else if (p14 && (analogRead(inpin[1]) < lt || analogRead(outpin[0]) > ut)) {
    Serial.println("~DOWN");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax() / 2, bleGamepadConfig.getAxesMax() / 2);
    p14 = 0;
  }
  if (!p13 && analogRead(inpin[1]) > lt && analogRead(outpin[1]) < ut) {
    Serial.println("UP");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax() / 2, bleGamepadConfig.getAxesMin());
    p13 = 1;
    delay(wt);
  } else if (p13 && (analogRead(inpin[1]) < lt || analogRead(outpin[1]) > ut)) {
    Serial.println("~UP");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax() / 2, bleGamepadConfig.getAxesMax() / 2);
    p13 = 0;
  }
  if (!p15 && analogRead(inpin[1]) > lt && analogRead(outpin[2]) < ut) {
    Serial.println("LEFT");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMin(), bleGamepadConfig.getAxesMax() / 2);
    p15 = 1;
    delay(wt);
  } else if (p15 && (analogRead(inpin[1]) < lt || analogRead(outpin[2]) > ut)) {
    Serial.println("~LEFT");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax() / 2, bleGamepadConfig.getAxesMax() / 2);
    p15 = 0;
  }
  if (!p16 && analogRead(inpin[1]) > lt && analogRead(outpin[3]) < ut) {
    Serial.println("RIGHT");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax(), bleGamepadConfig.getAxesMax() / 2);
    p16 = 1;
    delay(wt);
  } else if (p16 && (analogRead(inpin[1]) < lt || analogRead(outpin[3]) > ut)) {
    Serial.println("~RIGHT");
    bleGamepad.setAxes(bleGamepadConfig.getAxesMax() / 2, bleGamepadConfig.getAxesMax() / 2);
    p16 = 0;
  }
}