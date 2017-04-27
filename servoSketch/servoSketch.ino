#include <Servo.h>

char cmd;
String string;

#define led 13

//switch servo positions
#define S_ON_POS    0
#define S_OFF_POS   180
#define S_INIT_POS  90

int s_task_done;
#define SWITCH_DELAY  1000 //time to turn switch on, or off

Servo servo1;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);   //baud rate - hc05
  pinMode(led, OUTPUT); //led 13

  servo1.attach(9);     //switch servo attaches to pin 9
  servo1.write(S_INIT_POS);
  s_task_done = 0;
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
    string = "";

  while (Serial.available() > 0)
  {
    cmd = (byte)Serial.read();

    if (cmd == ':')
      break;
    else
      string += cmd;

    delay(1);
  }

  //s_task_done = 0;

  if (string == "LO")
    LEDOn();
  if (string == "LF")
    LEDOff();
  if (string == "SO")
  {
    s_task_done = 0;
    SwitchOn();
    string = "";
  }
  if (string == "SF")
  {
    s_task_done = 0;
    SwitchOff();
    string = "";
  }

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
  if (s_task_done == 0) //ensure moves once per signal sent
  {
    servo1.write(S_ON_POS);  //move up
    delay(SWITCH_DELAY);
    servo1.write(S_INIT_POS);   //move back down

    s_task_done = 1;
  }
}

void SwitchOff()
{
  if (s_task_done == 0) //ensures moves once per signal sent
  {
    servo1.write(S_OFF_POS);  //move down
    delay(SWITCH_DELAY);
    servo1.write(S_INIT_POS);   //move back up

    s_task_done = 1;
  }
}

