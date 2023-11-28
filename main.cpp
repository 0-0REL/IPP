#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define ANG_INCL(acex,acey,acez) -atan(acex/sqrt(acey*acey+acez*acez))*180/PI //Anglo en x
float calibGx = 0, calibGy = 0, calibGz = 0;  //calibracion giroscopio
float GxCalib = 0, GyCalib = 0, GzCalib = 0;  //Valores para calibrar el giroscopio
float acex = 0, acey = 0, acez = 0; //valores de acelerometro en X, Y y Z
float Ay = 0; //Valor de angulo en el eje Y
float error = 0, error_anterior = 0, velocidad = 0, deriv = 0, int_prev = 0, int_act = 0;
float kalmanAngY = 0, UnKalmanAngY = 2*2;
float KalmanO[] = {0, 0};
double t1 = 0, t2 = 0, t = 0; //Tiempo
float AceCalib = 0;//Para hacer la calibracion del acelerometro
Adafruit_MPU6050 mpu;//Giroscopio
hw_timer_t *Timer0_Cfg = NULL;//Temporizador 0
void Kalman1D(float KalEst, float KalUn, float KalIn, float KalMes){
  KalEst = KalEst + 0.004*KalIn;
  KalUn = KalUn + 0.004*0.0004*4*4;
  float KalG = KalUn*1/(1*KalUn + 3*3);
  KalEst = KalEst + KalG*(KalMes - KalEst);
  KalUn = (1- KalG)*KalUn;
  KalmanO[0] = KalEst;
  KalmanO[1] = KalUn;
  return;
}
int intento_de_PID(){
  const float KP = 1.8, KD = 2, KI = 1.5;
  //error = kalmanAngY;
  error = ANG_INCL(acex,acey,acez) - 3.85;
  t1 = timerReadMicros(Timer0_Cfg);
  int_act = (error - error_anterior)*t/2 + int_prev;
  deriv = (error - error_anterior)/t;
  velocidad = KP*error + KI*int_act + KD*deriv;
  int_prev = int_act;
  error_anterior = error;
  t2 = timerReadMicros(Timer0_Cfg);
  t = t2 - t1;
  return (int)velocidad;
}
void setup() {
  Serial.begin(115200);
  /*while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }*/
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  Serial.println("");
  delay(100);
  /*for (int i = 0; i<2000 ; i++){//calibracion, tarda 2 segundos
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    //calibGx += g.gyro.x;
    calibGy += g.gyro.y;
    //calibGz += g.gyro.z;
    //AceCalib += a.acceleration.y - 9.81;
    delay(1);
  }*/
  /*AceCalib /= 2000;
  Serial.println(AceCalib);
  delay(5000);*/
  //calibGx /= 2000;
  //calibGy /= 2000;//Y
  //calibGz /= 2000;
  Timer0_Cfg = timerBegin(0,2,true);//Congiguracion de temporiador 0
  timerWrite(Timer0_Cfg, 0);//temporizador 0 = 0
  timerStart(Timer0_Cfg);//comienza a contar temporizador 0
}

void loop() {
  sensors_event_t a, g, temp;//Para lectura del giroscopio
  mpu.getEvent(&a, &g, &temp);//para lectura del giroscopio
  //GxCalib = g.gyro.x - calibGx;
  //GyCalib = g.gyro.y - calibGy;//calibracion del giroscopio Y
  //GzCalib = g.gyro.z - calibGz;
  acex = a.acceleration.x - 0.35;//Acelerometro X
  acey = a.acceleration.y + 0.21;//Acelerometro Y
  acez = a.acceleration.z - 0.36;//Acelerometro Z
  /*Ay = ANG_INCL(acex,acey,acez);
  Kalman1D(kalmanAngY, UnKalmanAngY, GyCalib, Ay);
  kalmanAngY = KalmanO[0];
  UnKalmanAngY = KalmanO[1];*/
  intento_de_PID();
  if(velocidad > 255) velocidad = 255;//limitacion a 255 porque no soporta mas la funcion analogWrite()
  else if(velocidad < -255) velocidad = -255;//Lo mismo acá
  //Serial.print(error_anterior);Serial.print(" ");Serial.print(error);Serial.print(" ");Serial.println(velocidad);
  //Serial.println(t);
  Serial.print(">error:"), Serial.println(error);
  Serial.print(">integral:"), Serial.println(int_act);
  Serial.print(">derivada:"), Serial.println(deriv);
  Serial.print(">velocidad:"), Serial.println(velocidad);
  if (velocidad>0){//Giro a direccion 1
    analogWrite(13, 0);
    analogWrite(12, abs(velocidad));
  }
  else {//Giro a direccion 2 =!direccion1
    analogWrite(13, abs(velocidad));
    analogWrite(12, 0);
  }
  //delay(1000);
  //Ax = atan(acey/sqrt(acex*acex+acez*acez))*180/PI;
  //Ay = -atan(acex/sqrt(acey*acey+acez*acez))*180/PI;
  //Serial.print(">Acel (m/s^2) X:"), Serial.println(acex);
  //Serial.print(">Acel (m/s^2) Y:"), Serial.println(acey);
  //Serial.print(">Acel (m/s^2) Z:"), Serial.println(acez);
  //Serial.print(">Rot (rad/s) X:"), Serial.println(GxCalib);
  //Serial.print(">Rot (rad/s) Y:"), Serial.println(GyCalib);
  //Serial.print(">Rot (rad/s) Z:"), Serial.println(GzCalib);
  //Serial.print(">AngX:"), Serial.println(Ax);
  //Serial.print(">AngY:"), Serial.println(Ay);
}