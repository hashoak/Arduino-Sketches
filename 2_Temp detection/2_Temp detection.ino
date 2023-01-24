int tempPin=18;
double temp;
double tempSum;

void setup() {
  analogReference(1.1);
  Serial.begin(9600);
}

void loop() {
  tempSum=0.0;
  for(int i=0;i<1000;i++)
  {
    temp=analogRead(tempPin)/9.31;
    tempSum+=temp;
    delay(1);
  }
  Serial.print("TEMPERATURE = ");
  Serial.print(tempSum/1000); // display temperature value
  Serial.print("*C");
  Serial.println();
}