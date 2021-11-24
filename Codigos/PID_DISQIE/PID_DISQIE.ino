//librerias
#include <PID_v1.h>
#include <Servo.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include "Wire.h"

//instancias
MPU6050 sensor;
Servo ESC;

//variables del sensor
int ax, ay, az;

//declaracion de variables del pid
double punto_ajuste, entrada, salida;

//INSTANCIA PID
PID myPID(&entrada, &salida, &punto_ajuste,2,0,0.2, DIRECT);
//PID myPID(&entrada, &salida, &punto_ajuste,0.679,5,1.897, DIRECT);
//PID myPID(&entrada, &salida, &punto_ajuste,0.016,0.00005,0.000623, DIRECT); //el mejor hasta el momento
//0.6

void setup(){
  
    //obtencion del angulo actual
    entrada = angulo_actual();//-6000 a 6000
    punto_ajuste =-1431;//angulo deseado
    
    //encendido del pid
    myPID.SetOutputLimits(65,80);
    myPID.SetMode(AUTOMATIC);
    
    Serial.begin(9600);     //Iniciando puerto serial
    Wire.begin();           //Iniciando I2C  
    sensor.initialize();    //Iniciando el sensor
    
    pinMode(3,OUTPUT);
    ESC.attach(3);//pin del arduino que controlara el esc

    
    //pulsos para activar el 'ESC'
    ESC.writeMicroseconds(1000);
    //tiempo de espera para la activacion del esc
    delay(5000);
}

int angulo_actual(){
    sensor.getAcceleration(&ax, &ay, &az);//obtiene variables del sensor MPU6050
    return ay;
}


void loop(){

  long muestras = 0;
  long cont = 1;
  bool act = false;
  
  do{
    int angulo_muestra = angulo_actual();
    muestras+=angulo_muestra;
    cont++;
    if(cont==50)
      act = true;
  }while(act==false);

  
  entrada = muestras/50;//filtro de ruido
  //entrada = angulo_actual();
  myPID.Compute();

  double velocidad=salida;
        
  if(velocidad >= 0 && velocidad <=130){
      //ESC.write(velocidad);
      
 
      //Serial.print(velocidad);
      //Serial.print("   --   ");
      
      //Serial.println(" grados");
      //Serial.println();
  }
  Serial.print(velocidad);
  Serial.print(",");
  Serial.print(punto_ajuste);
  Serial.print(",");
  Serial.print(entrada);
  Serial.print(",");
  Serial.print(angulo_actual());
  Serial.println();
  
  /*Serial.println(velocidad);
  Serial.println(entrada);*/  
}
