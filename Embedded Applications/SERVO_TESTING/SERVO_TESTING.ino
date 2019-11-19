/*
 Try this test sketch with the Servo library to see how your
 servo responds to different settings, type a position
 (0 to 180) or if you type a number greater than 180 it will be
 interpreted as microseconds(544 to 2400), in the top of serial
 monitor and hit [ENTER], start at 90 (or 1472) and work your
 way toward zero (544) 5 degrees (or 50 micros) at a time, then
 toward 180 (2400).
*/
#include <Servo.h>
Servo servo;

void setup() {
  // initialize serial:
  Serial.begin(9600); //set serial monitor baud rate to match
  servo.write(90);
  servo.attach(9);
  int pos = 0;

  // let the servo find it's position
  for (int mpos = 600; mpos <= 2400; mpos += 10){
    servo.writeMicroseconds(mpos);
    delay(10);
    Serial.print("micro:"); Serial.println(mpos);
  }

  for (int mpos = 2400; mpos >= 600; mpos -= 10){
    servo.writeMicroseconds(mpos);
    delay(10);
    Serial.print("micro:"); Serial.println(mpos);
  }
}

void loop() {

  // let the servo find it's position
  for (int mpos = 600; mpos <= 2400; mpos += 1){
    servo.writeMicroseconds(mpos);
    delay(1);
    //Serial.print("micro:"); Serial.println(mpos);
  }

  for (int mpos = 2400; mpos >= 600; mpos -= 1){
    servo.writeMicroseconds(mpos);
    delay(1);
    //Serial.print("micro:"); Serial.println(mpos);
  }
}
