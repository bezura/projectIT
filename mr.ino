#include <GyverTimer.h>
#include "effects.h"
#include <FastLED.h>

#define jQbert 

#define ECHO 13
#define TRIG 14
#define LIGHT_SENSOR 0

#define NUM_LEDS 256
#define DATA_PIN 5
#define BRIGHTNESS 15

CRGB leds[NUM_LEDS];

int Max_Dist_Cm = 15;
int Light_To_Sleep = 2800; 

int Timeout_to_Animation = 350; 
GTimer Timer_Animation;

int Time_Smile = 10000; 
GTimer Timer_Smlie(MS,Time_Smile);

GTimer Timer_Off_Wow;
int Time_Off_Wow = 10000; 

int Tim_Check_Dist = 350; 
GTimer Timer_Check_Dist(MS,Tim_Check_Dist);

GTimer Timer_Frames(MS,200);

GTimer Timer_Alarm_Smile;
GTimer Timer_Alarm_Wow;

unsigned int kol_smile = 4;

unsigned int kol_wow = 6;

unsigned int kol_sleep = 6;

bool Alarm_Wow = 0;
bool Alarm_Smile = 0;
bool Alarm_Sleep = 0;

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LIGHT_SENSOR, INPUT);
  analogWrite(LIGHT_SENSOR, LOW);

  #ifdef jQbert
    pinMode(22, INPUT_PULLUP);
  #endif
}

void loop(){

  #ifdef jQbert
    while(!digitalRead(22)){
      byte frame;
      for (frame=0;frame<2;frame++){
        setFrame(Qbert, frame);
        FastLED.show();
        delay(500);
     }
     delay(600);
      }
  #endif

  if (Alarm_Sleep){
    kol_smile = 4;
    kol_wow = 6;
    Sleep();
  }
  else if (Alarm_Wow){
    kol_sleep = 6;
    kol_smile = 4;
    Wow();
    if (Timer_Alarm_Wow.isReady()){
      kol_wow=6;
      Alarm_Wow = 0;
      Timer_Off_Wow.setTimeout(Time_Off_Wow);
      Timer_Smlie.reset();
      Timer_Alarm_Wow.reset();
      Timer_Alarm_Wow.stop();
    }
  }
  else{
    kol_sleep = 6;
    kol_wow = 6;
    if (Timer_Smlie.isReady()){
      Alarm_Smile = 1;
      Timer_Alarm_Smile.setTimeout(Timeout_to_Animation*kol_smile+20);
    } 
    if (Timer_Alarm_Smile.isReady()){
      kol_smile=4;
      Alarm_Smile = 0;
      Timer_Alarm_Smile.reset();
    }
    if (Alarm_Smile) Smile();
    else{
		setFrame(standartSmile, 0);
	} 

    if(!Alarm_Wow and Timer_Check_Dist.isReady() and (!Timer_Off_Wow.isEnabled() or Timer_Off_Wow.isReady())) CheckDist();
  }

  Alarm_Sleep = (!Light_on());
  
  if(Timer_Frames.isReady()) FastLED.show();
}

void setFrame(const uint32_t imgArr[][256], byte frame) {
  int i;

  for (i=0;i<NUM_LEDS;i++){
    uint32_t ledColor=pgm_read_dword(&(imgArr[frame][i]));
    leds[i]=ledColor;
  }
  
}

void CheckDist(){
  long duration, cm;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);

  
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  
  duration = pulseIn(ECHO, HIGH);

  
  cm = (duration / 2) / 29.1;
  
  
  
  if( cm <= Max_Dist_Cm ){
    Timer_Alarm_Wow.setTimeout(Timeout_to_Animation*kol_wow+20);
    Alarm_Wow=1;
  }
}

bool Light_on(){
  
  
  
  return( Light_To_Sleep -  analogRead(LIGHT_SENSOR) > 0);
}

void Sleep(){
  if (!Timer_Animation.isEnabled()){
    setFrame(sleepSmile, 0);
    kol_sleep = 5;
    Timer_Animation.setTimeout(Timeout_to_Animation);
  }
  if (Timer_Animation.isReady()){
    if (kol_sleep == 6){
      setFrame(sleepSmile, 0);
      kol_sleep = 5;
	  Timer_Animation.setTimeout(Timeout_to_Animation);
    }
    else if (kol_sleep == 5){
      setFrame(sleepSmile, 1);
      kol_sleep = 4;
      Timer_Animation.start();
    }
    else if (kol_sleep == 4){
      setFrame(sleepSmile, 2);
      kol_sleep = 3;
      Timer_Animation.start();
    }
    else if (kol_sleep == 3){
      setFrame(sleepSmile, 3);
      kol_sleep=2;
      Timer_Animation.start();
    }
	else if (kol_sleep == 2){
      setFrame(sleepSmile, 4);
      kol_sleep=1;
      Timer_Animation.start();
    }
	else if (kol_sleep == 1){
      setFrame(sleepSmile, 5);;
      kol_sleep=6;
      Timer_Animation.start();
    }
  }
  
}

void Smile(){
  if (!Timer_Animation.isEnabled()){
    setFrame(standartSmile, 0);
    kol_smile = 3;
    Timer_Animation.setTimeout(Timeout_to_Animation);
  }
  if (Timer_Animation.isReady()){
    if (kol_smile == 4){
      setFrame(standartSmile, 0);
      kol_smile = 3;
    Timer_Animation.setTimeout(Timeout_to_Animation);
    }
    else if (kol_smile == 3){
      setFrame(standartSmile, 1);
      kol_smile = 2;
      Timer_Animation.start();
    }
    else if (kol_smile == 2){
      setFrame(standartSmile, 2);
      kol_smile = 1;
      Timer_Animation.start();
    }
    else if (kol_smile == 1){
      setFrame(standartSmile, 3);
      kol_smile = 4;
      Timer_Animation.start();
    }
  }
  
}

void Wow(){
  if (!Timer_Animation.isEnabled()){
      setFrame(wowSmile, 0);
      kol_wow = 5;
    Timer_Animation.setTimeout(Timeout_to_Animation+200);
  }
  if (Timer_Animation.isReady()){
    if (kol_wow == 6){
      setFrame(wowSmile, 0);
      kol_wow = 5;
    Timer_Animation.setTimeout(Timeout_to_Animation+200);
    }
    else if (kol_wow == 5){
      setFrame(wowSmile, 1);
      kol_wow = 4;
      Timer_Animation.start();
    }
    else if (kol_wow == 4){
      setFrame(wowSmile, 2);
      kol_wow = 3;
      Timer_Animation.start();
    }
    else if (kol_wow == 3){
      setFrame(wowSmile, 3);
      kol_wow = 2;
      Timer_Animation.start();
    }
    else if (kol_wow == 2){
      setFrame(wowSmile, 4);
      kol_wow = 1;
      Timer_Animation.start();
    }
    else if (kol_wow == 1){
      setFrame(wowSmile, 5);
      kol_wow=6;
      Timer_Animation.start();
    }
  }
  
}