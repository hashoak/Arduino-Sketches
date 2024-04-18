int soundSensor=18;
int LED=LED_BUILTIN;
boolean LEDStatus=LOW;

void setup() {
  pinMode(soundSensor,INPUT);
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int SensorData=analogRead(soundSensor);
  if(SensorData>250)
  {
    Serial.println(SensorData);
    LEDStatus=!LEDStatus;
    digitalWrite(LED,LEDStatus);
    delay(1000);
  }
} 