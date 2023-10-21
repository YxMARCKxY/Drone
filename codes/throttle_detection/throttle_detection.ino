#include <Servo.h>

Servo ESC;

void setup() {
  Serial.begin(9600);

  ESC.attach(2);
  ESC.writeMicroseconds(1000);

  pinMode(9, INPUT);

  delay(5000);
}

void send_acceleration(int throttle) {
  if(throttle < 1000) throttle = 0;
  if(throttle > 2000) throttle = 2000;
 
  ESC.writeMicroseconds(throttle);
  Serial.println(throttle);
}

void loop(){
  send_acceleration(pulseIn(9, HIGH));

  delay(100);
}

