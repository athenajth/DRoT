#include <Servo.h>

char cmd; 
String string; 

#define led 13

Servo servo1; 

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);   //baud rate - hc05
  pinMode(led,OUTPUT);  //led 13
  servo1.attach(9);     //switch servo
  servo1.write(90);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
    string = ""; 

   while(Serial.available() > 0)
   {
      cmd = (byte)Serial.read();

      if(cmd == ':')
        break; 
      else
        string += cmd; 

      delay(1); 
   }

    if(string == "LO")
      LEDOn(); 
    if(string == "LF")
      LEDOff(); 
    if(string == "SO")
      SwitchOn(); 
    if(string == "SF")
      SwitchOff(); 
}

void LEDOn()
{
  digitalWrite(led, HIGH); 
}

void LEDOff()
{
  digitalWrite(led, LOW); 
  delay(500); 
}

void SwitchOn()
{
  servo1.write(0);  //move up
  servo1.write(90);   //move back down
}

void SwitchOff()
{
  servo1.write(180);  //move down
  servo1.write(90);   //move back up
}

