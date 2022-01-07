// Dave Yonkers, 2020

// include the servo library
#include <Servo.h>

// include the custom timer library
#include <EasyTimer.h>

// can bus decleration
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> cbus1;


// minimum and maximum allowable values for the M400 throttle sensor (i.e. idle and WOT)
double throttle_pos_min = 10.0, throttle_pos_max=100.0;


// rate to check for timeouts -- should be faster than the minimum timout duration for any CAN signal
EasyTimer timeout_check_timer(10); // 10Hz

#include <StateCAN.h>
#include "CAN1.hpp"  // can reading definitions
#include "can_send.hpp"
static CAN_message_t rxmsg;


// initializations for our servo
Servo etc_servo;
int etc_servo_pin = A6;
int etc_servo_lowerb_pwm = 154; // 0% throttle servo pos (verify on throttle if servo other than Hitec HS-5645MG)
int etc_servo_upperb_pwm = 55; // 100% throttle servo pos (right now, servo range is 25-165)
int etc_servo_increment = 1; // number of servo degrees to
int etc_servo_current = 0; // current position
float etc_servo_degrees_dif_accepted = 0.5; // will stop moving the servo when within 'X' degrees of desired position
EasyTimer etc_servo_update_timer(15); // rate at which to update the servo


// the good stuff
LEDBlink onboard_led(13, 10); // onboard led, pin13, blink at 10hz


EasyTimer debug(10);


void setup() {

  // set secondary values for if a fault happens
  USER_throttleRequest.set_timeout_delay(1000);
  USER_throttleRequest.set_secondary_value(0);

  M400_throttlePosition.set_timeout_delay(1000);
  M400_throttlePosition.set_secondary_value(100);

  // initialize the CAN bus
  cbus1.begin();
  cbus1.setBaudRate(1000000);

  // Serial bus for arduino debugging
  Serial.begin(115200);

  // led pin
  pinMode(13, OUTPUT);

  // attatch the servo to its pin
  etc_servo.attach(etc_servo_pin);

  // sweep the servo with <param>ms between increments/decrements
  servo_sweeper(25);

  EasyTimer debug(10);


}


void loop() {

  // led stuffs
  onboard_led.run();

  // read the can bus every clock cycle
  if (cbus1.read(rxmsg)){
    decode_DYNO(rxmsg);
  }


  
  // be sure that the request is within allowable bounds:
  if (USER_throttleRequest.value() < throttle_pos_min){
    ETC_throttlePosition = throttle_pos_min;
  } else if (USER_throttleRequest.value() > throttle_pos_max){
    ETC_throttlePosition = throttle_pos_max;
  }


  // check to see if the signals we want are still time-valid
  if (timeout_check_timer.isup()){
    USER_throttleRequest.timeout_check();
    M400_throttlePosition.timeout_check();
  }

  // update the servo----------------------------------------------------------

  if (etc_servo_update_timer.isup()){

    // calculate the delta 
    int error = ETC_throttlePosition.value() - M400_throttlePosition.value();

    // if the error is MORE than the allowable amount, we need to update the servo
    if (abs(error) > etc_servo_degrees_dif_accepted){

      // determine if the servo is reversed
      int direction_multiplier = 0;
      if (etc_servo_lowerb_pwm > etc_servo_upperb_pwm){
        direction_multiplier = -1; // reversed
      } else {
        direction_multiplier = 1;  // normal
      }

      // if the error is greater, then we need to close the throttle by decrementing the position
      if (error > 0 ){

        etc_servo_current -= etc_servo_increment * direction_multiplier;

      // the error is less, therefore we must increase the throttle to get to the desired open position
      } else {

        etc_servo_current += etc_servo_increment * direction_multiplier;

      }

    }

    // update the servo
    etc_servo.write(etc_servo_current);

  }

  if (debug.isup()){
    Serial.println();
    Serial.print("M400 Throttle %:  "); Serial.println(M400_throttlePosition.value());
    Serial.print("ETC Throttle %:   "); Serial.println(ETC_throttlePosition.value());
    Serial.print("ETC Throttle PWM: "); Serial.println(ETC_servoOutput.value());
  }

  // send it!
  send_can1();
}



float float_map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// sweeps the servo between it's lower and upper bounds, then ends in the middle of the two
void servo_sweeper(int delayms){

  int lower = 0;
  int upper = 0;
  int mid = 0;

  // assign the easily-incrementable boundaries
  if (etc_servo_lowerb_pwm < etc_servo_upperb_pwm){
    lower = etc_servo_lowerb_pwm;
    upper = etc_servo_upperb_pwm;
  } else {
    upper = etc_servo_lowerb_pwm;
    lower = etc_servo_upperb_pwm;
  }

  etc_servo_current = lower;
  mid = (lower + upper) / 2;

  etc_servo.write(etc_servo_current);
  delay(delayms);

  // sweep to upper bound
  while (etc_servo_current < upper){
    etc_servo_current++;
    etc_servo.write(etc_servo_current);
    delay(delayms);
  }

  // sweep down
  while (etc_servo_current > lower){
    etc_servo_current--;
    etc_servo.write(etc_servo_current);
    delay(delayms);
  }

  //sweep back up to middle
  while (etc_servo_current < mid){
    etc_servo_current++;
    etc_servo.write(etc_servo_current);
    delay(delayms);
  }


}
