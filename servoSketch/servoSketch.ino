#include <Servo.h>

char cmd;
String string;

#define led 13

//switch servo positions
#define S_ON_POS    10
#define S_OFF_POS   170
#define S_INIT_POS  65

#define SWITCH_DELAY  600 //time to turn switch on, or off
int s_task_done;  //is the current switch task done running
int curr_s_state; //is the switch on or off (1:ON; 0:OFF)

Servo servo1;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);   //baud rate - hc05
  pinMode(led, OUTPUT); //led 13

  servo1.attach(9);     //switch servo attaches to pin 9
  servo1.write(S_INIT_POS);
  s_task_done = 0;
  curr_s_state = 1;   //assume switch is in ON pos
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

  //string is from bluetooth spp
  if(string == "D") //default mode
  {
    LEDOff(); 
    servo1.write(S_INIT_POS); 
  }
  else if (string == "LO")
    LEDOn();
  else if (string == "LF")
    LEDOff();
  else if (string == "SO")
  {
    s_task_done = 0;
    SwitchOn();
    string = "";
  }
  else if (string == "SF")
  {
    s_task_done = 0;
    SwitchOff();
    string = "";
  }
  else if (string == "ST")
  {
    s_task_done = 0;
    SwitchToggle();
    string = "";
  }
}

void SwitchOn()
{
  if (s_task_done == 0) //ensure moves once per signal sent
  {
    servo1.write(S_ON_POS);  //move up
    delay(SWITCH_DELAY);
    servo1.write(S_INIT_POS);   //move back down

    s_task_done = 1;
    curr_s_state = 1;
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
    curr_s_state = 0;
  }
}

void SwitchToggle()
{
  if (curr_s_state == 1)
    SwitchOff();
  else
    SwitchOn();
}

/*void SwitchDemo()
{
  if (curr_s_state == 1)
  {
    //s_task_done = 0;
    SwitchOff();
    //delay(2000);
  }
  else
  {
    //s_task_done = 0;
    SwitchOn();
    //delay(2000);
  }
}*/


void LEDOn() {
  digitalWrite(led, HIGH);
}
void LEDOff() {
  digitalWrite(led, LOW);
}

