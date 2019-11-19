// Written by Dave Yonkers, 2019
#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"


// timers!
class EasyTimer{

  public:
    EasyTimer(int freq); // constructor
    bool check(); // will check the timer to return true if time is up, or false if it is not yet time
    void set_frequency(int freq); // frequency in Hz
    void set_delay(int delay); // delay in milliseconds

  private:
    int freq_; // Hz
    unsigned long last_time_; // for use by timer to hold last time
    int ms_; // milliseconds
};

#endif
