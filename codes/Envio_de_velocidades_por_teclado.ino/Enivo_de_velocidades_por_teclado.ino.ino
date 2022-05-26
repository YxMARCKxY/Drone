#include <Servo.h>//libreria

Servo ESC;//instancia de servo = 'ESC'

int velocidad = 0;

void setup() {
  Serial.begin(9600);
  ESC.attach(5);//pin del arduino que controlara el esc
  //pulsos para activar el 'ESC'
  ESC.writeMicroseconds(1000);
  //tiempo de espera para la activacion del esc
  delay(5000);
}



void loop(){
  
int option = Serial.read();


if(option == 49){
  if(velocidad<2000){
    velocidad+=50;
  }
}else if(option == 48){
  if(velocidad>0){
    velocidad -=50;
  }
}



  if(velocidad >= 0 && velocidad <= 2000){
    ESC.writeMicroseconds(velocidad);
    Serial.println(velocidad);
  }
  delay(20);

delay(100);
}
