/*
* Need to include the SoftTimer and PCIManagerLibraries
*/

#include <BlinkTask.h>
#include <Debouncer.h>
#include <DelayRun.h>
#include <Dimmer.h>
#include <FrequencyTask.h>
#include <Heartbeat.h>
#include <Rotary.h>
#include <SoftPwmTask.h>
#include <SoftTimer.h>
#include <Task.h>
#include <TonePlayer.h>

#define max_command_length 25;
char command [25];
int index;
int optionNumber = 0;
char incomingByte = -1;
int red = 000;
int blue = 000;
int green = 000;
int serialFrequency = 9600;


void processCommand(Task * me);
void executeCommand(Task * me);

Task t1(2000, processCommand);
Task t2(500, executeCommand);

void clearCommand(){
  for(int i = 0; i < index; i++)
  {
    command[i] = 0;
  }
  index = 0;
}

void processCommand(Task * me){
  clearCommand();
  
  while(Serial.available()>0){
    incomingByte = Serial.read();
    command[index] = incomingByte;
    index++;
  }
  
  if(index > 0){
    command[index] = '\0';
    Serial.println(command);
    
    if(command[0] == 's' && command[1] == 'c'){
      optionNumber = 1;
      return;
    }
    else if(strcmp(command, "off") == 0)
    {
      optionNumber = 2;
      return;
    }
    else if(strcmp(command, "cycle") == 0)
    {
      optionNumber = 3;
      return;
    }
  }
}

int getValue(char digit1, char digit2, char digit3){
  return ((digit1 - 48) * 100) + ((digit2 - 48) * 10) + ((digit3 - 48) * 1);  
}


//Input => sc(000,000,000)
void setColorCallBack(){
  red = getValue(command[3], command[4], command[5]);
  blue = getValue(command[7], command[8], command[9]);
  green = getValue(command[11], command[12], command[13]);
  
  analogWrite(9, red);
  analogWrite(10, blue);
  analogWrite(11, green);
  delay(3000);
}

void offCallBack(){
  analogWrite(9, 255);
  analogWrite(10, 255);
  analogWrite(11, 255);
}

void cycleCallBack(){
  red = random(0, 255);
  blue = random(0, 255);
  green = random(0, 255);

  analogWrite(9, red);
  analogWrite(10, blue);
  analogWrite(11, green); 
}

void executeCommand(Task* me)
{
  if(optionNumber == 1){
    setColorCallBack();
  }
  else if(optionNumber == 2){
    offCallBack();
  }
  else if(optionNumber == 3){
    cycleCallBack();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(serialFrequency);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  SoftTimer.add(&t1);
  SoftTimer.add(&t2);
  
}

