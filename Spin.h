#ifndef Spin_h
#define Spin_h

#include "Animation.h"

class Spin : public Animation {
  public:
    Spin(Adafruit_NeoPixel *hourPix, Adafruit_NeoPixel *minsPix) : Animation(hourPix, minsPix) {
      Serial.println("Return from Spin constructor");
    };
    void runFor(int secs);
};

#endif
