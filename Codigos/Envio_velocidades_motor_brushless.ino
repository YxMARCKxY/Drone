#include <Servo.h>//libreria

Servo ESC;//instancia de servo = 'ESC'

int velocidad = 10;

void setup() {
  Serial.begin(9600);
  pinMode(7,INPUT);
  pinMode(7,OUTPUT);
  pinMode(8,INPUT);
  pinMode(8,OUTPUT);
  
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);

  
  ESC.attach(13);//pin del arduino que controlara el esc
  //pulsos para activar el 'ESC'
  ESC.writeMicroseconds(1000);
  //tiempo de espera para la activacion del esc
  delay(5000);
  Serial.println("Velocidad establecida en 10");
}



void impresion(int velocidad, int decremento, int incremento){
  Serial.print("velocidad : ");
  Serial.print(velocidad);


  Serial.println();
  if(velocidad == 180)
    digitalWrite(3,HIGH);
  else
    digitalWrite(3,LOW);

  if(velocidad == 0)
    digitalWrite(4,HIGH);
  else
    digitalWrite(4,LOW);
}


void loop(){
  
int incremento = digitalRead(8);
int decremento = digitalRead(7);


if(incremento == 1){
  if(velocidad<180){
    velocidad+=10;
    impresion(velocidad, decremento, incremento);
  }
}else if(decremento == 1){
  if(velocidad>0){
    velocidad -=10;
    impresion(velocidad, decremento, incremento);
  }
}

  // ancho de pulso controlado al iniciar el loop manualmente
  //ESC.writeMicroseconds(1300);
  //grados de la libreria servo que especifican la velocidad del motor brushless
  ESC.write(velocidad);//0=0 de velocidad y 180= VELOCIDAD_TOPE
  delay(20);


digitalWrite(7,LOW);
digitalWrite(8,LOW);
delay(100);
}
