*/
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
char comand [25];
int index;
int opcionNumber = 0;
char incomingByte = -1;
int red = 000;
int blue = 000;
int green = 000;
int buidSetup = 9600;


void processCommand(Task * me);
void executeCommand(Task * me);

Task t1(2000, processCommand);
Task t2(500, executeCommand);

void clearCommand(){
  for(int i = 0; i < index; i++)
  {
    comand[i] = 0;
  }
  index = 0;
}

void processCommand(Task * me){
  clearCommand();
  
  while(Serial.available()>0){
    incomingByte = Serial.read();
    comand[index] = incomingByte;
    index++;
  }
  
  if(index>0){
    comand[index] = '\0';
    Serial.println(comand);
    
    if(comand[0] == 's' && comand[1] == 'c'){
      opcionNumber = 1;
      return;
    }
    else if(strcmp(comand, "off") == 0)
    {
      opcionNumber = 2;
      return;
    }
    else if(strcmp(comand, "cycle") == 0)
    {
      opcionNumber = 3;
      return;
    }
  }
}

int getColor(char digit1, char digit2, char digit3){
  return ((digit1 - 48) * 100) + ((digit2 - 48) * 10) + ((digit3 - 48) * 1);  
}


//Input => sc(000,000,000)
void setColorCallBack(){
  red = getColor(comand[3], comand[4], comand[5]);
  blue = getColor(comand[7], comand[8], comand[9]);
  green = getColor(comand[11], comand[12], comand[13]);
  
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
  if(opcionNumber == 1){
    setColorCallBack();
  }
  else if(opcionNumber == 2){
    offCallBack();
  }
  else if(opcionNumber == 3){
    cycleCallBack();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(buidSetup);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  SoftTimer.add(&t1);
  SoftTimer.add(&t2);
  
}

