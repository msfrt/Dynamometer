// Dave Yonkers, 2020

// include the servo library
#include <Servo.h>

// include the custom timer library
#include <EasyTimer.h>

// can bus decleration
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> cbus1;


#include <QuickPID.h>
//Define Variables we'll be connecting to
float throttle_setpoint = 50;
float throttle_input = 50;
float servo_output = 150;
 
//Specify the links and initial tuning parameters
float Kp=1, Ki=0, Kd=0; // default values
QuickPID throttle_PID(&throttle_input, &servo_output, &throttle_setpoint, Kp, Ki, Kd, throttle_PID.Action::reverse);

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
  throttle_PID.SetMode(throttle_PID.Control::automatic);
  USER_throttleKp = Kp;
  USER_throttleKi = Ki;
  USER_throttleKd = Kd;
  throttle_PID.SetOutputLimits(etc_servo_lowerb_pwm, etc_servo_upperb_pwm);

}


void loop() {

  // led stuffs
  onboard_led.run();

  // read the can bus every clock cycle
  if (cbus1.read(rxmsg)){
    decode_DYNO(rxmsg);
  }

  if (USER_throttleRequest.is_updated()){
    throttle_setpoint = USER_throttleRequest.value();
  }

  if (M400_throttlePosition.is_updated()){
    throttle_input = M400_throttlePosition.value();
  }
  
  // be sure that the request is within allowable bounds:
  if (throttle_setpoint < throttle_pos_min){
    throttle_setpoint = throttle_pos_min;
  } else if (throttle_setpoint > throttle_pos_max){
    throttle_setpoint = throttle_pos_max;
  }


  // check to see if the signals we want are still time-valid
  // if (timeout_check_timer.isup()){
  //   USER_throttleRequest.timeout_check();
  //   M400_throttlePosition.timeout_check();
  // }

  // see if we need to update PID values
  //if (USER_throttleKp.value() )


  throttle_PID.Compute();


  // update the CAN values that we send
  ETC_throttlePosition = throttle_setpoint;
  ETC_servoOutput = servo_output;



  etc_servo.write(servo_output);
  Serial.println();
  Serial.print("M400 Throttle %:  "); Serial.println(M400_throttlePosition.value());
  Serial.print("ETC Throttle %:   "); Serial.println(ETC_throttlePosition.value());
  Serial.print("ETC Throttle PWM: "); Serial.println(ETC_servoOutput.value());

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
