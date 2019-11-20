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
  myservo.write(0);
  Serial.println("0");
  delay(2500);

  myservo.write(90);
  Serial.println("90");
  delay(2500);
}
