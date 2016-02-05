#include <Adafruit_NeoPixel.h>
#include <SoftReset.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include "Time.h"
#include "Spin.h"
#include "Fade.h"
#include "Color.h"

#define HOUR_PIN  5 // Pin that our hours LED ring is on
#define MINS_PIN  6 // Pin that our mins LED ring is on
#define NTP_UPDATE  10 // Update time from NTP every X minutes

Adafruit_NeoPixel hourPix = Adafruit_NeoPixel(12, HOUR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minsPix = Adafruit_NeoPixel(24, MINS_PIN, NEO_GRB + NEO_KHZ800);

Color hourColour = Color(255,255,0);
Color minsColour = Color(0,255,100);

byte mac[] = {
  0x90, 0xA2, 0xD4, 0x10, 0x04, 0x2F
};

unsigned int localPort = 8888; // Local port for UDP packets

char timeServer[] = "neptune.mattdyson.org"; 

const int NTP_PACKET_SIZE = 48; 

byte packetBuffer[ NTP_PACKET_SIZE];

Time currentTime;
unsigned long lastNTPupdate;

EthernetUDP Udp;

int delayVal = 500;

void setup() {
  Serial.begin(9600);
  Serial.println("Beginning setup");
  Serial.print("Starting Ethernet... ");
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println(" ERROR - Failed to configure Ethernet using DHCP");
    Fade animate = Fade(&hourPix, &minsPix, Color(255,0,0), 20);
    animate.runFor(20);
    soft_restart();
  }
  Udp.begin(localPort);
  
  Serial.println("done");
  Serial.print("Starting up pixels... ");
  
  hourPix.begin();
  minsPix.begin();
  
  Serial.println("done");
  Serial.print("Clearing face... ");
  resetFace(true);
  Serial.println("done");
  
  Serial.print("Fetching time... ");
  currentTime = getTime();
  if(currentTime.isAccurate()==false) {
    Serial.println(" ERROR - Didn't recieve an accurate time back from NTP. Cannot complete startup");
    // We didn't receive good data back from NTP, so show an error then restart
    Fade animate = Fade(&hourPix, &minsPix, Color(255,255,0), 20);
    animate.runFor(20);
    soft_restart();
  }
  Serial.print("done - ");
  currentTime.debug();
  
  //Fade animate = Fade(&hourPix, &minsPix, Color(255,255,255), 20);
  //Serial.println("Running animation");
  //animate.runFor(20);
}

// We aim to be looping this every second
void loop() { 
  unsigned long loopStart = millis();
  resetFace();

  if(true) { // TODO: replace with an enum for what we actually want to display
    timeFace();
  }
  
  unsigned long loopLength = millis() - loopStart; // The length of time the above code took to run
  if(loopLength<1000) { // Less than 1s
    delay(1000-loopLength); // Sleep by however long we need to get to 1s
    currentTime.inc();
  } else { // Greater than 1s, so we need to move time on appropriately
    int fullSecs = loopLength/1000;
    currentTime.addSecs(fullSecs);
    delay(1000-(loopLength - fullSecs)); // To get us up to the next whole second
    currentTime.inc();
  }
  
  unsigned long nextUpdate = lastNTPupdate + (NTP_UPDATE * 60000UL);
  
  // First clause determines if we need an NTP update
  // Second clause protects against wrap-around of millis()
  if(nextUpdate < millis() || millis() < lastNTPupdate) {     
    Serial.print("Updating time from NTP... ");
    Time fetchedTime = getTime();
    if(fetchedTime.isAccurate()==false) {
      Serial.print(" ERROR. Didn't recieve an accurate time back from NTP."); 
    } else {
      Serial.print("done - ");
      currentTime = fetchedTime;
      currentTime.debug();
    }
    
    lastNTPupdate = millis();
  }
}

// Set all LEDs to blank
void resetFace() {
  resetFace(false);  
}

void resetFace(bool withUpdate) {
  for(int i=0;i<12;i++) {
    hourPix.setPixelColor(i, hourPix.Color(0, 0, 0));
  }
  
  for(int i=0;i<24;i++) {
    minsPix.setPixelColor(i, minsPix.Color(0, 0, 0));
  }
  
  if(withUpdate==true) {
   minsPix.show();
   hourPix.show();
  }
}

// Function for showing time
void timeFace() {
  int i = 0;
  for(i=0;i<currentTime.smallHours();i++) {
   hourPix.setPixelColor(i, hourColour.toNeoPixel());
  }
  
  // Set remainder of hours to blank
  for(i++;i<12;i++) {
    hourPix.setPixelColor(i, hourPix.Color(0, 0, 0));
  }
  
  // We want to scale down minutes from 0-59 to 0-23
  int fullLights = currentTime.mins() / 2.5;
  for(i=0;i<fullLights;i++) {
   minsPix.setPixelColor(i, minsColour.toNeoPixel());
  }
  
  // We want to fade up the next LED by a percentage of how far we are to that minute
  float curMins = currentTime.mins() + (currentTime.secs() / 60.0);  
  float remainMins = curMins - (i * 2.5);
  float final = (remainMins / 2.5) * minsColour.getBright();
  Color cloneMinsColour = minsColour;
  cloneMinsColour.setBright(final);
  minsPix.setPixelColor(i, cloneMinsColour.toNeoPixel());
  
  // Set remainder of minutes to blank
  for(i++;i<24;i++) {
   minsPix.setPixelColor(i, minsPix.Color(0, 0, 0));
  }
  
  hourPix.show();
  minsPix.show();
}
  
// Functions for NTP functionality

// Adapted from Arduino UdpNtpClient example
Time getTime() {
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  
  int hour = 0;
  int mins = 0;
  int secs = 0;

  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;

    hour = (epoch  % 86400L) / 3600;
    mins = (epoch  % 3600) / 60;
    secs = (epoch % 60);
    
    Time t = Time(hour, mins, secs);
    t.setAccurate();
    
    return t;
  } else {
    // Didn't receive a UDP packet
  }
  
  return Time(hour, mins, secs);
}

// Taken from Arduino UdpNtpClient example
// send an NTP request to the time server at the given address
void sendNTPpacket(char* address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
