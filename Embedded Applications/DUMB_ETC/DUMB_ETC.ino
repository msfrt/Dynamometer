//CHECK GITHUB FOR LATEST VERSION

// if CAN is not working make sure that the original FlexCAN is not
// installed. (Check the onenote for instructions)
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

// include the custom timer library
#include "Timer.h"

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
canSignal USER_throttleRequest, ETC_throttlePosition, ETC_status;


// create timer variables 
Timer can_send_timer(20); // creates a timer that sends at 20Hz


void setup() {

  // initialize the CAN bus
  Can0.begin(1000000);

  // Serial bus for arduino debugging
  Serial.begin(9600);

}


void loop() {


  // to write a message, simply fill the message buffer and use Dave's handy sendCAN function. Beware, bit shifting
  // can be VERY confusing, so be sure to read the tutorial in the OneNote. Also, verify the signal bit order (Intel
  // or Motorola) in the DBC.
  msg.buf[0] = my_intel_var; // this is the first 8 bits
  msg.buf[1] = my_intel_var >> 8; // this is the last 8 bits
  msg.buf[2] = 0; //empty
  msg.buf[3] = 0;
  msg.buf[4] = my_motorola_var >> 8; // this is the last 8 bits
  msg.buf[5] = my_motorola_var; // this is the first 8 bits
  msg.buf[6] = 69; // you can do this (as long as it's a maximum of 8-bits long)
  msg.buf[7] = 0;
  sendCAN(420, 8, 0); // here, we send an 8-byte message with ID 420 on CAN bus 0

  readCAN(); // generally speaking, we will try to read the CAN bus on every loop through the main function.
  // essentially, we want to continuously check the bus in case a message appears.

}




// This function simplifies sending a can message. However, you must ensure that the message buffer "msg" has
// all of the desired data in their respective bits before calling this function.
void sendCAN(int id, int len, int busNo)
{
  msg.len = len; //CAN message length unit: Byte (pretty much always 8 bits)
  msg.id = id; //CAN message ID (look at DBC to verify)

  switch(busNo)
  {
    // if there was more than one bus, this is where you would put another case
    case 0:
      Can0.write(msg); // let 'er rip
      break;
  }
}


void readCAN()
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

      case 420: // this is message with ID 420
        // now, we can access the bits that we desire
        my_intel_var = rxData[0] + rxData[1] * 256; // we multiply by 256, which is like shifting 8 bits
        my_motorola_var = rxData[3] * 256 + rxData[4]; // opposite order!
        break; // very important - do not forget to break


      case 69: //let's say this is a multiplexed message
        switch(rxMultID){

          case 4: // this essentially means that the first byte (the multiplexer) == 4
            my_motorola_var = rxData[3] * 256 + rxData[4];
            break;
        }
        break; // break from message 69
    }

  }
}
