#ifndef Time_h
#define Time_h

#include "Arduino.h"

class Time
{
  public:
    Time();
    Time(int hours, int minutes, int seconds);
    void debug();
    int hours();
    int smallHours();
    int mins();
    int secs();
    
    void inc();
    void addSecs(int toAdd);
    
    bool isAccurate();
    void setAccurate();
  private:
    int _hrs;
    int _min;
    int _sec;
    
    bool _accurate;
};

#endif
