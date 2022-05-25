#include <Wire.h>
#include <Servo.h>

Servo left_prop_rear;
Servo right_prop_rear;

Servo left_prop_forward;
Servo right_prop_forward;

int16_t Acc_rawX, Acc_rawY, Acc_rawZ, Gyr_rawX, Gyr_rawY, Gyr_rawZ;

float Acceleration_angle[2];
float Gyro_angle[2];
float Total_angle[2];


float elapsedTime, time, timePrev;
int i;
float rad_to_deg = 180/3.141592654;

float PID, pwmLeft, pwmRight, error, previous_error;
float pid_p=0;
float pid_i=0;
float pid_d=0;

/////////////////PID CONSTANTS/////////////////
double kp=3.53;//3.55
double ki=0.003;//0.003
double kd=2.5;//2.05
///////////////////////////////////////////////

//////////////max/min velocities///////////////
int min_vel = 1230;
int max_vel = 1350;
///////////////////////////////////////////////

double throttle=1285; //initial value of throttle to the motors
float desired_angle = 0; //This is the angle in which we whant the balance to stay steady

void setup() {
  Wire.begin(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

///////NUMBER OF PORT THAT CONTROL THE ENGINES /////////
  right_prop_rear.attach(5); //attatch the behind right motor
  left_prop_rear.attach(6);  //attatch the behind left motor

  left_prop_forward.attach(11);
  right_prop_forward.attach(10);
///////////////////////////////////////////////////////
  time = millis(); 
  
  left_prop_rear.writeMicroseconds(1000); 
  right_prop_rear.writeMicroseconds(1000);
  left_prop_forward.writeMicroseconds(1000);
  right_prop_forward.writeMicroseconds(1000);
  delay(5000); 

  
}

void loop() {
  /*----------------------------------IMU----------------------------------*/
  timePrev = time; 
  time = millis();  
  elapsedTime = (time - timePrev) / 1000; 

  Wire.beginTransmission(0x68);
  Wire.write(0x3B); //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,6,true); 

  Acc_rawX=Wire.read()<<8|Wire.read(); 
  Acc_rawY=Wire.read()<<8|Wire.read();
  Acc_rawZ=Wire.read()<<8|Wire.read();

  Acceleration_angle[0] = atan((Acc_rawY/16384.0)/sqrt(pow((Acc_rawX/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
  Acceleration_angle[1] = atan(-1*(Acc_rawX/16384.0)/sqrt(pow((Acc_rawY/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;

  Wire.beginTransmission(0x68);
  Wire.write(0x43); 
  Wire.endTransmission(false);
  Wire.requestFrom(0x68,4,true); 

  Gyr_rawX=Wire.read()<<8|Wire.read(); 
  Gyr_rawY=Wire.read()<<8|Wire.read();

  /*---X---*/
  Gyro_angle[0] = Gyr_rawX/131.0; 
  /*---Y---*/
  Gyro_angle[1] = Gyr_rawY/131.0;

  /*---X axis angle---*/
  Total_angle[0] = 0.98 *(Total_angle[0] + Gyro_angle[0]*elapsedTime) + 0.02*Acceleration_angle[0];
  /*---Y axis angle---*/
  Total_angle[1] = 0.98 *(Total_angle[1] + Gyro_angle[1]*elapsedTime) + 0.02*Acceleration_angle[1];

  /*--------------------------------------PID-------------------------------------*/
  error = Total_angle[1] - desired_angle;
  pid_p = kp*error;
  if(-3 <error <3)
  {
    pid_i = pid_i+(ki*error);  
  }
  pid_d = kd*((error - previous_error)/elapsedTime);
  PID = pid_p + pid_i + pid_d;
  if(PID < -1000)
  {
    PID=-1000;
  }
  if(PID > 1000)
  {
    PID=1000;
  }

  pwmLeft = throttle - PID;
  pwmRight = throttle + PID;

  //Right
  if(pwmRight < min_vel)
  {
    pwmRight= min_vel;
  }
  if(pwmRight > max_vel)
  {
    pwmRight=max_vel;
  }

  //Left
  if(pwmLeft < min_vel)
  {
    pwmLeft= min_vel;
  }
  if(pwmLeft > max_vel)
  {
    pwmLeft=max_vel;
  }
  /*-------------------------------FINAL CODE OF PID-----------------------------*/
  Serial.print("vel-LEFT: ");
  Serial.print(pwmLeft);
  Serial.print("---");
  Serial.print("vel-RIGHT: ");
  Serial.print(pwmRight);
  Serial.println();
  ///////////////SENDING THE MICROSECONDS TO THE ENGINES////////////
  if(pwmRight>1000 && pwmRight <2000){
      left_prop_rear.writeMicroseconds(pwmLeft);
      right_prop_rear.writeMicroseconds(pwmLeft);

      left_prop_forward.writeMicroseconds(pwmRight);
      right_prop_forward.writeMicroseconds(pwmRight);
  }
  ////////////////////////////////////////////////////////////////
  previous_error = error; 
}
