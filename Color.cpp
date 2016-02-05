#include "Color.h"

Color::Color() {
  Color(255,255,255);
}

Color::Color(int r, int g, int b) {
  setR(r);
  setG(g);
  setB(b);
 
  _brightness = 0.1; 
}

void Color::setBright(float bright) {
  if(bright<0) { bright=0; }
  else if(bright>1) { bright=1; }
  
  _brightness = bright; 
}

float Color::getBright() {
  return _brightness; 
}

void Color::setR(int r) {
  if(r>255) { r=255; }
  else if(r<0) { r=0; }
  
  _r = r;
}

void Color::setG(int g) {
  if(g>255) { g=255; }
  else if(g<0) { g=0; }
  
  _g = g;
}

void Color::setB(int b) {
  if(b>255) { b=255; }
  else if(b<0) { b=0; }
  
  _b = b;
}

// Adapted from the Adafruit NeoPixel library
uint32_t Color::toNeoPixel() {
  int r = _r * _brightness;
  int g = _g * _brightness;
  int b = _b * _brightness;
  
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
