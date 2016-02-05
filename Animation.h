#ifndef Animation_h
#define Animation_h

#include "Arduino.h"
#include "Color.h"
#include <Adafruit_NeoPixel.h>

class Animation {
  public:
    inline Animation(Adafruit_NeoPixel *hourPix, Adafruit_NeoPixel *minsPix) {
      Serial.println("Loaded animation");
      _hourPix = hourPix;
      _minsPix = minsPix;
      Serial.println("Returning from Animation constructor");
    };
    void runFor(int secs);
    
  protected:
    Adafruit_NeoPixel *_hourPix;
    Adafruit_NeoPixel *_minsPix;
    inline void before() {
      Serial.println("Running before animation");
      reset();
    };
    inline void after() {
      Serial.println("Running after animation");
      reset();
    };
    inline void reset() {
      for(int i=0;i<12;i++) {
        _hourPix->setPixelColor(i, _hourPix->Color(0, 0, 0));
      }
      
      for(int i=0;i<24;i++) {
        _minsPix->setPixelColor(i, _minsPix->Color(0, 0, 0));
      }
      
      _minsPix->show();
      _hourPix->show();  
    }
};
#endif
