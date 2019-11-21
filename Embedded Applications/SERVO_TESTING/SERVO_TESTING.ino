#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(A6);  // attaches the servo on pin 9 to the servo object
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    pos = Serial.parseInt();

    // say what you got:
    Serial.print("Write: "); Serial.println(pos);
    myservo.write(pos);
  }


}
