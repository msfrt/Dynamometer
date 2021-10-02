#ifndef CAN_READ_LINEAR_HPP
#define CAN_READ_LINEAR_HPP


#include <FlexCAN_T4.h>

static CAN_message_t rxmsg;


// ID 100 on bus 1 - M400 dataset 1
void read_M400_1520(CAN_message_t &imsg){
  // multiplexer first-bit
  switch (imsg.buf[0]) {

    case 10:
      M400_throttlePosition.set_can_value(imsg.buf[2] << 8 | imsg.buf[3]);
      break;
  }
}


// ID 101 on bus 1 - M400 dataset 2
void read_M400_1519(CAN_message_t &imsg){
  // multiplexer first-bit
  switch (imsg.buf[0]) {
    case 10:
      break;
  }
}



void read_USER_request1(CAN_message_t &imsg){
  USER_throttleRequest.set_can_value(imsg.buf[0] | imsg.buf[1] << 8);
}

void read_USER_request2(CAN_message_t &imsg){
  USER_throttleKp.set_can_value(imsg.buf[0] | imsg.buf[1] << 8);
  USER_throttleKi.set_can_value(imsg.buf[2] | imsg.buf[3] << 8);
  USER_throttleKd.set_can_value(imsg.buf[4] | imsg.buf[5] << 8);
}



// function that reads the msg and then directs that data elsewhere
void read_can1(){
  if (cbus1.read(rxmsg)){

    switch (rxmsg.id) {
      case 1520:
        read_M400_1520(rxmsg);
        break;
      case 1519:
        read_M400_1519(rxmsg);
        break;
      case 120:
        read_USER_request1(rxmsg);
        break;
      case 128:
        read_USER_request2(rxmsg);
        break;
    } // end switch statement

  }
}


#endif
