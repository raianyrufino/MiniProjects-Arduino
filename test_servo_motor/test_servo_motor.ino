
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int val = 0;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  
  myservo.write(180);                  // sets the servo position according to the scaled value
  delay(800);   
   myservo.write(0);                  // sets the servo position according to the scaled value
  delay(800);   

}

