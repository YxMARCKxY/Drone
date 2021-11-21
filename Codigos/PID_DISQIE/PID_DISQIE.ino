#include <PID_v1.h>
#include <Servo.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include "Wire.h"

MPU6050 sensor;
Servo ESC;

int ax, ay, az;

// Definir variables a las que nos conectaremos
double punto_ajuste, entrada, salida;

// Especifique los enlaces y los parámetros de ajuste iniciales
PID myPID(&entrada, &salida, &punto_ajuste,2,5,1, DIRECT);


void setup()
{
  
// inicializamos las variables a las que estamos vinculados
  entrada = angulo_actual();
  punto_ajuste = 0;

// enciende el PID
  myPID.SetMode(AUTOMATIC);

  Serial.begin(9600);     //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

   
  ESC.attach(8);//pin del arduino que controlara el esc
  //pulsos para activar el 'ESC'
  ESC.writeMicroseconds(1000);
  //tiempo de espera para la activacion del esc
  delay(5000);
}

int angulo_actual(){
    sensor.getAcceleration(&ax, &ay, &az);//obtiene variables del sensor MPU6050
    return map(ay, -6000, 6000, -45, 45);
  }


void loop()
{
  entrada = angulo_actual();
  myPID.Compute();
  int  velocidad = map(salida, 0, 255, 0, 80);

  if(velocidad >= 0 && velocidad <=130){
      ESC.write(velocidad);
      Serial.println(velocidad);
  }
  
    
}
