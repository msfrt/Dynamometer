// if CAN is not working make sure that the original FlexCAN is not
// installed. (Check the onenote for instructions)
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

// include the servo library
#include <Servo.h>

// include the custom timer library
#include "EasyTimer.h"

// define message type - do not change these names
static CAN_message_t msg; // this is the outgoing message buffer
static CAN_message_t rxmsg; // this is the recieving message buffer


// define the struct for CAN signals
typedef struct {
  bool state = 0;
  unsigned long last_recieve = 0;
  unsigned long last_send = 0;
  int value = 0;
  int lower_bound = 0;
  int upper_bound = 0;
} canSignal;

// signals from the DBC that we want to read/send
canSignal USER_throttleRequest, USER_rpmRequest;
canSignal ETC_throttlePosition, ETC_status;
canSignal M400_throttlePosition, M400_rpm;

// create timer variables
EasyTimer can_timer_ETC_output(20); // creates a timer that sends at 20Hz
EasyTimer servo_output_timer(1000);

// initializations for our servo
Servo etc_servo;
int etc_servo_pin = A6;
int etc_servo_lowerb_deg = 165; // 0% throttle servo pos (verify on throttle if servo other than Hitec HS-5645MG)
int etc_servo_upperb_deg = 75; // 100% throttle servo pos
uint etc_servo_timeout_safety_factor = 1000; // time in ms before the throttle needs to be shut
double etc_servo_desired_throttle = 0;
int etc_servo_desired_throttle_int = 0; // used for casting to CAN (can't bitshift a double)
int etc_servo_output_angle = 0;


EasyTimer onboard_led_timer(10); // Hz
bool led_on = false;


void setup() {

  // initialize the CAN bus
  Can0.begin(1000000);

  // Serial bus for arduino debugging
  Serial.begin(9600);

  // led pin
  pinMode(13, OUTPUT);

  // attatch the servo to its pin
  etc_servo.attach(etc_servo_pin);

  // set the bounds for the user input (found in the CANalyzer GUI)
  USER_throttleRequest.lower_bound = 0;
  USER_throttleRequest.upper_bound = 100;


}


void loop() {


  // led stuffs
  if (onboard_led_timer.isup()){
    if (led_on){led_on = false; digitalWrite(13, LOW);}
    else { led_on = true; digitalWrite(13, HIGH);}
  }

  read_can();


  // first we need to check if it's no longer safe to open the throttle
  if (USER_throttleRequest.last_recieve - millis() >= etc_servo_timeout_safety_factor){
    etc_servo_desired_throttle = 0; // close it!!!
    // digitalWrite(13, LOW); //turn the LED off
  } else {
    etc_servo_desired_throttle = USER_throttleRequest.value * 0.1;
  }


  // map the desired throttle input (0-100) to the settable range of the servo
  etc_servo_output_angle = map(etc_servo_desired_throttle,
                               USER_throttleRequest.lower_bound, USER_throttleRequest.upper_bound,
                               etc_servo_lowerb_deg, etc_servo_upperb_deg);

  etc_servo.write(etc_servo_output_angle);



  messages_for_can();
}





// this function is simply a place to store the can messages and their respective timers
void messages_for_can(){

  if (can_timer_ETC_output.isup()){
    // ETC_output, ID 121
    etc_servo_desired_throttle *= 10; // change later to remove hard coding!
    etc_servo_desired_throttle_int = etc_servo_desired_throttle;
    msg.buf[0] = ETC_status.value;
    msg.buf[1] = etc_servo_desired_throttle_int;
    msg.buf[2] = etc_servo_desired_throttle_int >> 8;
    msg.buf[3] = etc_servo_output_angle;
    msg.buf[4] = etc_servo_output_angle >> 8;
    msg.buf[5] = 0;
    msg.buf[6] = 0;
    msg.buf[7] = 0;
    send_can(121, 8, 0);
  }

}




// This function simplifies sending a can message. However, you must ensure that the message buffer "msg" has
// all of the desired data in their respective bits before calling this function.
void send_can(int id, int len, int busNo)
{
  msg.len = len; //CAN message length unit: Byte (pretty much always 8 bits)
  msg.id = id; //CAN message ID (look at DBC to verify)

  switch(busNo)
  {
    case 0:
      Can0.write(msg); // let 'er rip
      break;
  }
}



void read_can()
{

  // "if a message was read on bus 0..."
  if ( Can0.read(rxmsg) )
  {

    // store the message ID and length
    int rxID = rxmsg.id;
    int rxDataLen = rxmsg.len;

    // assign the message data into an easily readable array, rxData
    int rxData[rxDataLen];
    for(int i=0; i<rxDataLen; i++)
    {
      rxData[i]=rxmsg.buf[i];
    }

    // assign the first byte as the multiplexer ID
    int rxMultID = rxData[0];

    switch(rxID){

      case 120: // USER_request1
        USER_throttleRequest.value = rxData[0] + rxData[1] * 256;
        USER_rpmRequest.value = rxData[2] + rxData[3] * 256; // opposite order!
        USER_throttleRequest.last_recieve = millis();
        USER_rpmRequest.last_recieve = millis();
        break; // break USER_request1


      case 1520: // M400_dataSet1
        switch(rxMultID){

          case 9:
            M400_rpm.value = rxData[6] * 256 + rxData[7];
            break;
          case 10:
            M400_throttlePosition.value = rxData[2] * 256 + rxData[3];
            break;
        }
        break; // break M400_datsSet1
    }

  }
}
