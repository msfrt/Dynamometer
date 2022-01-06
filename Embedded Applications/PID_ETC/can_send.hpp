#ifndef CAN_SEND_LINEAR_HPP
#define CAN_SEND_LINEAR_HPP

static CAN_message_t msg;

void send_ETC_10(){
  // the following line initializes a counter that is specific to PDM_10, but is static, so it's only created once.
  static StateCounter ctr;

  // update message ID and length
  msg.id = 121;
  msg.len = 8;


  // load up the message buffer
  msg.buf[0] = ctr.value();
  msg.buf[1] = ETC_throttlePosition.can_value();
  msg.buf[2] = ETC_throttlePosition.can_value() >> 8;
  msg.buf[3] = ETC_servoOutput.can_value();
  msg.buf[4] = ETC_servoOutput.can_value() >> 8;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  // send the message
  cbus1.write(msg);
}



void send_can1(){

  static EasyTimer ETC_10_timer(50);
  if (ETC_10_timer.isup()){
    send_ETC_10();
  }

}


#endif
