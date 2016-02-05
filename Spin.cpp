#include "Spin.h"

void Spin::runFor(int secs) {
  before();
  Serial.println("runFor called from Spin.cpp");
  
  unsigned long start = millis();
  int i = 0;
  
  while(millis() < (start + (secs * 1000))) {
    for(int x=0;x<12;x++) {
      if(x==i) {
        _hourPix->setPixelColor(x, _hourPix->Color(30, 0, 0));
        _minsPix->setPixelColor(x*2, _minsPix->Color(30, 0, 0));
      } else {
        _hourPix->setPixelColor(x, _hourPix->Color(0, 0, 0));
        _minsPix->setPixelColor(x*2, _minsPix->Color(0, 0, 0));
      }
    }
    i++;
    if(i==12) {
      i=0;
    }
    
    _hourPix->show();
    _minsPix->show();
    delay(100);
  } 
  
  Serial.println("Ending Spin animation");
  after();
}
