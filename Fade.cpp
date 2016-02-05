#include "Fade.h"

Fade::Fade(Adafruit_NeoPixel *hourPix, Adafruit_NeoPixel *minsPix, Color color, int intensity) : Animation(hourPix, minsPix) {
  _color = color;
  _intensity = intensity;
}

void Fade::runFor(int secs) {
  before();
  Serial.println("runFor called from Fade.cpp");
  
  unsigned long start = millis();
  int i = 0;
  float dir = 1;
  
  while(millis() < (start + (secs * 1000))) {
    _color.setBright(i / 100.0);
    
    for(int x=0; x<12; x++) {
      _hourPix->setPixelColor(x, _color.toNeoPixel());  
    }
    for(int x=0; x<24; x++) {
      _minsPix->setPixelColor(x, _color.toNeoPixel()); 
    }
    
    i+=dir;
    
    if(i>=_intensity) {
      dir = -1;
    } else if(i<=0) {
      dir = 1; 
    }
    
    _hourPix->show();
    _minsPix->show();
    
    delay(10 + (100 - _intensity));
  } 
  
  Serial.println("Ending Fade animation");
  after();
}
