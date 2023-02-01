#include <pitches.h> //add Equivalent frequency for musical note
#include <themes2.h> //add Note vale and duration 
#include <Keypad.h>

int speaker=10;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void poc()
{ 
  for (int thisNote = 0; thisNote < (sizeof(Pirates_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / Pirates_duration[thisNote];//convert duration to time delay
   if(thisNote%3==0){
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, Pirates_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void desp()
{
  for (int thisNote = 0; thisNote < (sizeof(desp_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / desp_duration[thisNote];//convert duration to time delay
   if(thisNote%3==0){
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, desp_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.50; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void cf()
{
  for (int thisNote = 0; thisNote < (sizeof(CrazyFrog_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / CrazyFrog_duration[thisNote]; //convert duration to time delay
 if(thisNote%3==0){
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, CrazyFrog_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.30;//Here 1.30 is tempo, decrease to play it faster
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void mario()
{
    for (int thisNote = 0; thisNote < (sizeof(MarioUW_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / MarioUW_duration[thisNote];//convert duration to time delay
     if(thisNote%3==0){
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, MarioUW_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 1.80;
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void ttn()
{
    for (int thisNote = 0; thisNote < (sizeof(Titanic_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / Titanic_duration[thisNote];//convert duration to time delay
     if(thisNote%3==0){
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, Titanic_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 2.70;
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}
void newn()
{
    for (int thisNote = 0; thisNote < (sizeof(new_note)/sizeof(int)); thisNote++) {

    int noteDuration = 1000 / new_duration[thisNote];//convert duration to time delay
     if(thisNote%3==0){
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   }
     if(thisNote%4==0){
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   }   if(thisNote%5==0){
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   }   else{
    
   
    tone(speaker, new_note[thisNote], noteDuration);
   } 
    int pauseBetweenNotes = noteDuration * 2.70;
    delay(pauseBetweenNotes);
    noTone(speaker); //stop music on pin 8 
    }
}

void setup() {
  //Button 4 with internal pull up
  Serial.begin(9600);
  Serial.println("Press the following buttons:");
  Serial.println("1. Pirates of Caribbean");
  Serial.println("2. Despacito");
  Serial.println("3. Crazy Frog");
  Serial.println("4. Mario");
  Serial.println("5. Titatic");
  Serial.println("6. Newn");
}

void loop()
{
  char keyPress=myKeypad.getKey();
  if(keyPress)
  {
    Serial.print("\nPressed ");
    Serial.println(keyPress);
  }
  if(keyPress=='1')
  {
    Serial.println("Pirates of Caribbean playing.");
    poc();
  }
  else if(keyPress=='2')
  {
    Serial.println("Despacito playing.");
    desp();
  }
  else if(keyPress=='3')
  {
    Serial.println("Crazy Frog playing.");
    cf();
  }
  else if(keyPress=='4')
  {
    Serial.println("Mario playing.");
    mario();
  }
  else if(keyPress=='5')
  {
    Serial.println("Titatic playing.");
    ttn();
  }
  else if(keyPress=='6')
  {
    Serial.println("Newn playing.");
    newn();
  }
  else if(keyPress) Serial.println("Wrong key.");
}