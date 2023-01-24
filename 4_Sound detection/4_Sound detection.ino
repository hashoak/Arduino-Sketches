int soundSensor=18;
int LED=LED_BUILTIN;
boolean LEDStatus=false;

void setup() {
  pinMode(soundSensor,INPUT);
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int SensorData=analogRead(soundSensor);
  if(SensorData>200)
  {
    Serial.println(SensorData);
    if(LEDStatus==false)
    {
      LEDStatus=true;
      digitalWrite(LED,HIGH);
    }
    else
    {
      LEDStatus=false;
      digitalWrite(LED,LOW);
    }
    delay(1000);
  }
 } 