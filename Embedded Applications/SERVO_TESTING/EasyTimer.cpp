// Written by Dave Yonkers, 2019
#include "Arduino.h"
#include "EasyTimer.h"

EasyTimer::EasyTimer(int freq){
  // if frequency is <=0 (why??), it defaults to 1Hz
  if (freq <= 0){
    freq = 1; // be aware, this changes the variable that is referenced, too
  }
  freq_ = freq;
  ms_ = 1000 / freq; // 1000 milliseconds in a second
  last_time_ = 0;
}


// this function returns true if it is time, false if time has not been reached yet
bool EasyTimer::check(){
  if (millis() - last_time_ >= ms_){
    last_time_ = millis(); // reset the timer
    return true;
  } else {
    return false;
  }
}


void EasyTimer::set_frequency(int freq){
  // if frequency is <=0 (why??), it defaults to 1Hz
  if (freq <= 0){
    freq = 1; // be aware, this changes the variable that is referenced, too
  }
  freq_ = freq;
  ms_ = 1000 / freq; // 1000 milliseconds in a second
}


void EasyTimer::set_delay(int ms){
  if (ms <= 0){
    ms = 1000;
  }
  freq_ = 1000 / ms;
  ms_ = ms; // 1000 milliseconds in a second
}
