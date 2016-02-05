#ifndef Fade_h
#define Fade_h

#include "Animation.h"

class Fade : public Animation {
  public:
    Fade(Adafruit_NeoPixel *hourPix, Adafruit_NeoPixel *minsPix, Color color, int intensity);
    void runFor(int secs);
    
  protected:
    Color _color;
    int _intensity;
};

#endif
