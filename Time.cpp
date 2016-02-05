#include "Time.h"

//Constructor
Time::Time(int hours, int minutes, int seconds)
{
  _hrs = hours;
  _min = minutes;
  _sec = seconds;
  _accurate = false;
}

// Empty constructor
Time::Time() {
  Time(0,0,0);
}

int Time::hours() {
  return _hrs;
}

int Time::smallHours() {
  if (_hrs > 12) {
    return _hrs-12;  
  } else {
    return _hrs;
  }
}

int Time::mins() {
 return _min; 
}

int Time::secs() {
 return _sec; 
}

void Time::addSecs(int toAdd) {
  for(int i=0;i<toAdd;i++) {
    inc();
  }
}

void Time::inc() {
 _sec++;
 if(_sec==60) {
   _sec=0;
   _min++;
   if(_min==60) {
     _min=0;
     _hrs++;
     if(_hrs==23) {
       _hrs=0;
     }
   }
 } 
}

bool Time::isAccurate() {
  return _accurate;
}

void Time::setAccurate() {
  _accurate = true; 
}

void Time::debug()
{
  if(_hrs < 10) Serial.print('0');
  Serial.print(_hrs, DEC);
  Serial.print(":");
  if(_min < 10) Serial.print('0');
  Serial.print(_min, DEC);
  Serial.print(":");
  if(_sec < 10) Serial.print('0');
  Serial.println(_sec, DEC);
}
