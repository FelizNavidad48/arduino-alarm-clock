#include "SevSeg.h"
#include<time.h>
#include "pitches.h"
//#include<stdio.h>
int soundPin = 3;
int potPin = A0;
int setAlarmPin = A1;
int resetPin = A2;
int firstTime = 0;
int currentTime;
int alarmTime = 1000;
unsigned long minuteCounter;
SevSeg sevseg; //Instantiate a seven segment object
void setup() {
  pinMode(soundPin, OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(setAlarmPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  byte numDigits = 4;
  byte digitPins[] = {10,11,12,13};
  byte segmentPins[] = {1,2,4,5, 6, 7, 8, 9};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  //setDisplay();
}

void loop() {
 
 //Set the innitial time
 if(firstTime==0){
  unsigned long startMillis = millis();
  minuteCounter = millis();
  while(millis()-startMillis <= 10000){
    int value = analogRead(potPin);          //Read and save analog value from potentiometer
    value = map(value, 0, 1023, 0, 1440); //Map value 0-1023 to 0-255 (PWM)
    int startingTime = (int) value/60*100 + value%60;
    sevseg.setNumber(startingTime,2);
    sevseg.refreshDisplay();
    currentTime = (unsigned long) startingTime;
  firstTime = 1;
   }
  }

//If button is pressed then set alarm
  if(digitalRead(setAlarmPin) == LOW){

    unsigned long startMillis = millis();
    while(millis()-startMillis <= 10000){
      int value = analogRead(potPin);          //Read and save analog value from potentiometer
      value = map(value, 0, 1023, 0, 1440); //Map value 0-1023 to 0-255 (PWM)
      alarmTime = (int) value/60*100 + value%60;
      sevseg.setNumber(alarmTime,2);
      sevseg.refreshDisplay();
      }
  }


  //reset alarm and set time again
  if(digitalRead(resetPin) == LOW){
      firstTime = 0;
      alarmTime = -1;
    }
    

//Update the display and count the time
  if(millis() - minuteCounter >= 60000){
    minuteCounter = millis();
    if ((currentTime+1)%100 >=60){
      if((currentTime+100)/100 >=24){
          currentTime = 0;
        }
        else{

          currentTime = currentTime + 100;
          currentTime = currentTime - currentTime%100;
          }
      }
      
    else{
      currentTime = currentTime +1; 
      }
    
   }
  sevseg.setNumber(currentTime, 2);
  sevseg.refreshDisplay();

//Check if its time for alarm
  if(alarm() == 1){
      playSong();
    }
}





int alarm(){
    if(currentTime==alarmTime){
        return 1;
      }
      else{
          return 0;
        }
  }

void speaker(){
  tone(soundPin, 659, 1000);
  }



void playSong(){
  int melody[] = {
    NOTE_E2, NOTE_E2, NOTE_E3,0, NOTE_E2, NOTE_D3, NOTE_E3, NOTE_E2, NOTE_E2,NOTE_E3, 0, NOTE_E2, NOTE_D3, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_E3, 0, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_C3, NOTE_C3, NOTE_D3, NOTE_B3,0, NOTE_B3, NOTE_D3, };

  int durations[] = {2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4 ,4, 4, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4,}; //4     
while(1){
  for(int note = 0; note < 28; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / (durations[note]*2);
    tone(soundPin, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.10;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(soundPin);
  }
}
}
