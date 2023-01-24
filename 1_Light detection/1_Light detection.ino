int LDR;
int Lin=18;
int LED=13;

void setup()
{
  Serial.begin(9600);
  pinMode(Lin,INPUT);
  pinMode(LED,OUTPUT);
}

void loop()
{
  LDR=analogRead(Lin);
  Serial.println(LDR);
  if(LDR>450) digitalWrite(LED,HIGH);
  else digitalWrite(LED,LOW);
  delay(500);
}