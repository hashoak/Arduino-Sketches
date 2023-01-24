int ledPin=LED_BUILTIN;
int inputPin=18;
int pirState=LOW;
int val=0;

void setup()
{
  pinMode(inputPin,INPUT);
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  val=digitalRead(inputPin);
  if(val==HIGH)
  {
    digitalWrite(ledPin,HIGH);
    if(pirState==LOW)
    {
      Serial.println("Motion Detected!");
      pirState=HIGH;
    }
  }
  else
  {
    digitalWrite(ledPin,LOW);
    if(pirState==HIGH)
    {
      Serial.println("Motion ended!");
      pirState=LOW;
    }
  }
  delay(100);
}
