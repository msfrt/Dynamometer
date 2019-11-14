#include "Arduino.h"
#include "Timer.h"

Timer::Timer(int freq){
  freq_ = freq;
  ms_ = 1000 / freq; // 1000 milliseconds in a second
  last_time_ = 0;
}


// this function returns true if it is time, false if time has not been reached yet
bool Timer::check(){
  if (millis() - last_time_ >= ms_){
    last_time_ = millis(); // reset the timer
    return true;
  } else {
    return false;
  }
}
