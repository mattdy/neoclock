#ifndef Color_h
#define Color_h

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

class Color {
  public:
    Color();
    Color(int r, int g, int b);
    uint32_t toNeoPixel(); // Convert to what the Adafruit NeoPixel library wants to see
    
    void setBright(float bright);
    float getBright();
    
    void setR(int r);
    void setG(int g);
    void setB(int b);
    
  private:
    int _r;
    int _g;
    int _b;
    float _brightness; // Value between 0 and 1 representing how bright we should be
};

#endif
