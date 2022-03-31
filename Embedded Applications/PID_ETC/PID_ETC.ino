// Dave Yonkers, 2020

// include the servo library
#include <Servo.h>

// include the custom timer library
#include <EasyTimer.h>

// can bus decleration
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> cbus1;


#include <PID_v1.h>
//Define Variables we'll be connecting to
double throttle_setpoint, throttle_input, throttle_output;
 
//Specify the links and initial tuning parameters
double Kp=1, Ki=0, Kd=0; // default values
PID throttle_PID(&throttle_input, &throttle_output, &throttle_setpoint, Kp, Ki, Kd, DIRECT);

// minimum and maximum allowable values for the M400 throttle sensor (i.e. idle and WOT)
double throttle_pos_min = 5.0, throttle_pos_max=100.0;


#include <StateCAN.h>
// StateSignal MOD_sigName(bitl, signed, inv_factor, offset, min, max, secondary_val, timeout);
StateSignal USER_throttleRequest(16, true, 10, 0, 0, 100, 0, 1000); // rx
StateSignal USER_throttleKp(16, true, 10, 0, 0, 100, 0, 1000); // rx
StateSignal USER_throttleKi(16, true, 100, 0, 0, 100, 0, 1000); // rx
StateSignal USER_throttleKd(16, true, 100, 0, 0, 100, 0, 1000); // rx
StateSignal M400_throttlePosition(16, true, 100, 0, 0, 100, 0, 1000); // rx
StateSignal ETC_throttlePosition(16, true, 10, 0, 0, 100, 0, 1000); // tx
StateSignal ETC_servoOutputAngle(16, true, 1, 0, 0, 100, 0, 1000); // tx

// rate to check for timeouts -- should be faster than the minimum timout duration for any CAN signal
EasyTimer timeout_check_timer(10); // 10Hz

#include "can_send.hpp"
#include "can_read.hpp"


// initializations for our servo
Servo etc_servo;
int etc_servo_pin = A6;
int etc_servo_lowerb_deg = 127; // 0% throttle servo pos (verify on throttle if servo other than Hitec HS-5645MG)
int etc_servo_upperb_deg = 28; // 100% throttle servo pos (right now, servo range is 25-165)
int etc_servo_increment = 1; // number of servo degrees to
int etc_servo_current = 0; // current position
float etc_servo_degrees_dif_accepted = 0.5; // will stop moving the servo when within 'X' degrees of desired position
EasyTimer etc_servo_update_timer(15); // rate at which to update the servo


// the good stuff
LEDBlink onboard_led(13, 10); // onboard led, pin13, blink at 10hz

EasyTimer debug(10);



void setup() {

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

  //turn the PID on
  throttle_PID.SetMode(AUTOMATIC);
  USER_throttleKp = Kp;
  USER_throttleKi = Ki;
  USER_throttleKd = Kd;
  throttle_PID.SetOutputLimits(throttle_pos_min, throttle_pos_max);

}


void loop() {

  // led stuffs
  onboard_led.run();

  // read the can bus every clock cycle
  read_can1();
  throttle_setpoint = USER_throttleRequest.value();
  throttle_input = M400_throttlePosition.value();


  // check to see if the signals we want are still time-valid
  if (timeout_check_timer.isup()){
    USER_throttleRequest.timeout_check();
    M400_throttlePosition.timeout_check();
  }

  // see if we need to update PID values
  //if (USER_throttleKp.value() )


  throttle_PID.Compute();

  // map the desired output angle to the top and bottom servo bounds
  etc_servo_current = float_map(throttle_output, throttle_pos_min, throttle_pos_max, etc_servo_lowerb_deg, etc_servo_upperb_deg);
  ETC_throttlePosition = throttle_output;
  ETC_servoOutputAngle = etc_servo_current;

  etc_servo.write(etc_servo_current);

  if (debug.isup()){
    Serial.println();
    Serial.println(ETC_throttlePosition.value());
    Serial.println(ETC_servoOutputAngle.value());
    //Serial.println(etc_servo_current);
    Serial.println(M400_throttlePosition.value());
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
  if (etc_servo_lowerb_deg < etc_servo_upperb_deg){
    lower = etc_servo_lowerb_deg;
    upper = etc_servo_upperb_deg;
  } else {
    upper = etc_servo_lowerb_deg;
    lower = etc_servo_upperb_deg;
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
