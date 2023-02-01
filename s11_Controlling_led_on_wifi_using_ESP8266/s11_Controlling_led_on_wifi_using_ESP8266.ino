#include <SoftwareSerial.h>                         //including the SoftwareSerial library will allow you to use the pin no. 2,3 as Rx, Tx.      
SoftwareSerial esp8266(2,3);                        //set the Rx ==> Pin 2; TX ==> Pin3.      
#define serialCommunicationSpeed 9600               // <========= define a constant named "serialCommunicationSpeed" with a value 9600. it referes to the Software and hardware serial communication speed(baud rate).      
#define DEBUG true                                  //make a constant named "DEBUG" and it's value true. we will use it later.      
int redLED =12;                                     //assign a variable named "redLED" with an integer value 12, it refers to the pin which the red LED is connected on.      
int blueLED =11;                                    //assign a variable named "blueLED" with an integer value 11, it refers to the pin which the blue LED is connected on.      
void setup()      
{      
  pinMode(redLED,OUTPUT);                           //set the pin number 12 as an output pin.      
  pinMode(blueLED,OUTPUT);                          //set the pin number 11 as an output pin.      
        
  digitalWrite(redLED,LOW);                         //turn the red LED off at the beginning of the program.      
  digitalWrite(blueLED,HIGH);                       //turn the blue LED on at the beginning of the program.      
  Serial.begin(serialCommunicationSpeed);           //begin the Hardware serial communication (0, 1) at speed 9600.      
  esp8266.begin(serialCommunicationSpeed);          //begin the software serial communication (2, 3) at speed 9600.      
  InitWifiModule();                                 //call this user-defined function "InitWifiModule()" to initialize a communication between the ESP8266 and your access point (Home Router or even your mobile hotspot).      
  digitalWrite(blueLED,LOW);                        //after finishing the initialization successfully, turn off the blue LED (just an indicator).      
}      
void loop()                                                         //our main program, some fun are about to start)      
{      
  if(esp8266.available())                                           //if there's any data received and stored in the serial receive buffer, go and excute the if-condition body. If not, dont excute the if-condition body at all.      
  {      
    if(esp8266.find("+IPD,"))                                       //search for the "+IPD," string in the incoming data. if it exists the ".find()" returns true and if not it returns false.      
    {      
      delay(1000);                                                  //wait 1 second to fill up the buffer with the data.      
      int connectionId = esp8266.read()-48;                         //Subtract 48 because the read() function returns the ASCII decimal value. And 0 (the first decimal number) starts at 48. We use it to convert from ASCI decimal value to a character value.      
      esp8266.find("pin=");                                         //Advance the cursor to the "pin=" part in the request header to read the incoming bytes after the "pin=" part which is the pinNumer and it's state.      
      int pinNumber = (esp8266.read()-48)*10;                       //read the first Byte from the Arduino input buffer(i.e. if the pin 12 then the 1st number is 1) then multiply this number by 10. So, the final value of the "pinNumber" variable will be 10.      
      pinNumber = pinNumber + (esp8266.read()-48);                  //read the second Byte from the Arduino input buffer(i.e. if the pin number is 12 then the 2nd number is 2) then add this number to the first number. So, the final value of the "pinNumber" variable will be 12.      
      int statusLed =(esp8266.read()-48);                           //read the third byte from the Arduino input buffer. then save it inside the "statusLed" variable. At any case, it will be 1 or 0.      
      digitalWrite(pinNumber, statusLed);                           //then turn the LED at "pinNumber" on or off depending on the "statusLed" variable value.      
      Serial.println(connectionId);                                 //print the "connectionId" value on the serial monitor for debugging purposes.      
      Serial.print(pinNumber);                                      //print the "pinNumber" value on the serial monitor for debugging purposes.      
      Serial.print("      ");                                       //print some spaces on the serial monitor to make it more readable.      
      Serial.println(statusLed);                                    //print the "statusLed" value on the serial monitor for debugging purposes.      
      String closeCommand = "AT+CIPCLOSE=";                         //close the TCP/IP connection.      
      closeCommand+=connectionId;                                   //append the "connectionId" value to the string.      
      closeCommand+="\r\n";                                         //append the "\r\n" to the string. it simulates the keyboard enter press.      
      sendData(closeCommand,1000,DEBUG);                            //then send this command to the ESP8266 module to excute it.      
            
    }      
  }      
}      
/******************************************************************************************************************************************************************************************      
* Name: sendData      
* Description: this Function regulates how the AT Commands will ge sent to the ESP8266.      
*       
* Params: command - the AT Command to send       
*                 - timeout - the time to wait for a response       
*                 - debug - print to Serial window?(true = yes, false = no)      
*                       
* Returns: The response from the esp8266 (if there is a reponse)      
*/      
String sendData(String command, const int timeout, boolean debug)      
{      
    String response = "";                                             //initialize a String variable named "response". we will use it later.      
          
    esp8266.print(command);                                           //send the AT command to the esp8266 (from ARDUINO to ESP8266).      
    long int time = millis();                                         //get the operating time at this specific moment and save it inside the "time" variable.      
    while( (time+timeout) > millis())                                 //excute only whitin 1 second.      
    {            
      while(esp8266.available())                                      //is there any response came from the ESP8266 and saved in the Arduino input buffer?      
      {      
        char c = esp8266.read();                                      //if yes, read the next character from the input buffer and save it in the "response" String variable.      
        response+=c;                                                  //append the next character to the response variabl. at the end we will get a string(array of characters) contains the response.      
      }        
    }          
    if(debug)                                                         //if the "debug" variable value is TRUE, print the response on the Serial monitor.      
    {      
      Serial.print(response);      
    }          
    return response;                                                  //return the String response.      
}      
/******************************************************************************************************************************************************************************************      
* Name: InitWifiModule      
* Description: this Function gives the commands that we need to send to the sendData() function to send it.      
*       
* Params: Nothing.      
*                       
* Returns: Nothing (void).      
*/      
void InitWifiModule()      
{      
  sendData("AT+RST\r\n", 2000, DEBUG);                                                  //reset the ESP8266 module.      
  delay(1000);      
  sendData("AT+CWJAP=\"ORBI12\",\"Mmfsl1234\"\r\n", 2000, DEBUG);        //connect to the WiFi network.      
  delay (20000);      
  sendData("AT+CWMODE=1\r\n", 1500, DEBUG);                                             //set the ESP8266 WiFi mode to station mode.      
  delay (1000);      
  sendData("AT+CIFSR\r\n", 1500, DEBUG);                                                //Show IP Address, and the MAC Address.      
  delay (1000);      
  sendData("AT+CIPMUX=1\r\n", 1500, DEBUG);                                             //Multiple conections.      
  delay (1000);      
  sendData("AT+CIPSERVER=1,80\r\n", 1500, DEBUG);                                       //start the communication at port 80, port 80 used to communicate with the web servers through the http requests.      
}      