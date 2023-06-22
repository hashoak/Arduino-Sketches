#define SCL_PIN 9
#define SDO_PIN 10

byte Key;

void setup()
{
  Serial.begin(9600);
  pinMode(SCL_PIN,OUTPUT);
  pinMode(SDO_PIN,INPUT);
}

void loop()
{
  Key=Read_Keypad();
  if(Key)
  {
    Serial.println(Key);
    delay(300);
  }
}

byte Read_Keypad(void)
{
  byte Count;
  byte Key_State=0;
  for(Count=1;Count<=8;Count++)
  {
    digitalWrite(SCL_PIN,LOW);
    if(!digitalRead(SDO_PIN)) Key_State=Count;
    digitalWrite(SCL_PIN,HIGH);
  }
  return Key_State;
}
