#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"


// timers!
class Timer{

  public:
    Timer(int freq); // constructor
    bool check(); // will check the timer to return true if time is up, or false if it is not yet time

  private:
    int freq_; // Hz
    unsigned long last_time_; // for use by timer to hold last time
    int ms_; // milliseconds
};

#endif
