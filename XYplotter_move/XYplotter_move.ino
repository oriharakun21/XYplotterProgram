/* 
 * Processingと連携して動かしていく 
 *  
 * miking log
 * 2018/8/29 作成開始 
 * 
 */

/* include */
#include "MeOrion.h"
#include <SoftwareSerial.h>

/* stepping moter */
MeStepper stepper1(PORT_1); 
MeStepper stepper2(PORT_2); 

/* 変更できる変数たち */
int MaxSpeed = 1000;
int Acceleration = 20000;

/*----------------- SET UP ---------------*/
void setup() {
  Serial.begin(9600);
  
  // Stepper
  stepper1.setMaxSpeed(MaxSpeed);
  stepper1.setAcceleration(Acceleration);
  stepper2.setMaxSpeed(MaxSpeed);
  stepper2.setAcceleration(Acceleration);
  
}

/*------------------ LOOP -------------------*/
void loop() {
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      stepper1.moveTo(0);
      break;
      case '1':
      stepper1.moveTo(2000);
      break;
      case '2':
      stepper2.move(50);
      break;
      case '3':
      stepper2.move(100);
      break;
      case '4':
      stepper2.move(200);
      break;
      case '5':
      stepper2.move(400);
      break;
      case '6':
      stepper2.move(600);
      break;
      case '7':
      stepper2.move(4000);
      break;
      case '8':
      stepper2.move(8000);
      break;
      case '9':
      stepper2.move(3200);
      break;
    }
  }
  stepper1.run();
  stepper2.run();
}
