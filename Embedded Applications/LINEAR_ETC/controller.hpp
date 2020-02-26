#ifndef CONTROLLA_CONTROLLLAAAAAAAA
#define CONTROLLA_CONTROLLLAAAAAAAA


// uses a linear control method to increment the current throttle position servo by <servo_inc> every call
int controller(StateSignal &desired, StateSignal &actual, int servo_current,
               int servo_min, int servo_max, int servo_inc, float degrees_within_accepted){

  int desired_tps = 0;
  bool reverse = false;
  bool valid = true;
  bool good_enough = false;

  if (servo_min > servo_max){
    reverse = true;
  }

  // only dynamically set the throttle is the signals are valid
  if (desired.is_valid() && actual.is_valid()){
    desired_tps = desired.value();
    valid = true;
  } else {
    valid = false;
  }

  // calculate it we're within an acceptable range
  if (abs(desired_tps - actual.value()) > degrees_within_accepted){
    good_enough = false;
  } else {
    good_enough = true;
  }


  if (!reverse){
    // if we want the throttle to be lower and there's still room to go lower...
    if (desired_tps < actual.value() && servo_current >= (servo_min + servo_inc) && !good_enough){
      servo_current -= servo_inc;

    // same as earlier, but we want to open the trottle
    } else if (desired_tps > actual.value() && servo_current <= (servo_max - servo_inc) && !good_enough){
      servo_current += servo_inc;

    // otherwise stay the same
    } else {
      servo_current = servo_current;
    }

  // servo bounds are flipped, so do reverse operations
} else {

  // if we want the throttle to be lower and there's still room to go lower...
  if (desired_tps < actual.value() && servo_current < (servo_min - servo_inc) && !good_enough){
    servo_current += servo_inc;

  // same as earlier, but we want to open the trottle
  } else if (desired_tps > actual.value() && servo_current > (servo_max + servo_inc) && !good_enough){
    servo_current -= servo_inc;
  // otherwise stay the same
  } else {
    servo_current = servo_current;
  }
}

  // return the updated value
  if (valid){
    return servo_current;
  } else {
    return servo_min;
  }

}




#endif
