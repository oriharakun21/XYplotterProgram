/* 
 * Processingと連携して動かしていく?
 *  
 * miking log
 * 2018/8/29 作成開始 
 * 2018/10/3 初期化
 * 
 */

/* include */
#include "MeOrion.h"
#include <SoftwareSerial.h>

/* stepping moter */
MeStepper stepper1(PORT_1); 
MeStepper stepper2(PORT_2); 

/* Limit Seitch */
MeLimitSwitch limitSwitchA_1(PORT_3,  SLOT1);
MeLimitSwitch limitSwitchA_2(PORT_3,  SLOT2);
MeLimitSwitch limitSwitchB_1(PORT_6,  SLOT1);
MeLimitSwitch limitSwitchB_2(PORT_6,  SLOT2);

/* 変更できる変数たち */
const int MaxSpeed = 10000;
const int Acceleration = 10000;

/* フラグ */
int firstRun = 0;

/*----------------- SET UP ---------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("Start.");
  
  // Stepper
  stepper1.setMaxSpeed(MaxSpeed);
  stepper1.setAcceleration(Acceleration);
  stepper2.setMaxSpeed(MaxSpeed);
  stepper2.setAcceleration(Acceleration);
}

/*------------------ LOOP -------------------*/
void loop() {
  if(firstRun == 0){
    FirstSetup();
    firstRun = 1;
  }
  easyMove();
}


/* 初期移動（0地点を右前に設定・） */
void FirstSetup(){
  while(1){
    if(!limitSwitchB_2.touched()){
      stepper1.move(-500);
      stepper1.run();
      }
    if(!limitSwitchA_1.touched()){
      stepper2.move(-500);
      stepper2.run();
      }
      if(limitSwitchA_1.touched() && limitSwitchB_2.touched()){
        stepper1.setCurrentPosition(0); /* 0ポイントに設定 */
        stepper2.setCurrentPosition(0);
        break;
      }
    }
}

/* ++ ステッピングモータを簡単に動かすプログラム ++ */
void easyMove(){
  if(Serial.available()){
    char a = Serial.read();
    Serial.print("a : ");
    Serial.println(a);
    switch(a)
    {
      case '0':
      stepper1.moveTo(0);
      break;
      case '1':
      stepper1.moveTo(200);
      break;
      case '2':
      stepper1.moveTo(500);
      break;
      case '3':
      stepper1.move(500);
      break;
      case '4':
      stepper1.move(-1000);
      break;
      case '5':
      stepper1.move(4000);
      break;
      case '6':
      stepper1.move(-6000);
      break;
      case '7':
      stepper1.move(4000);
      break;
      case '8':
      stepper1.move(15000);
      break;
      case '9':
      stepper1.move(-15000);
      break;
      case 'a':
      stepper2.move(-15000);
      break;
      case 'b':
      stepper2.move(15000);
      break;
      case 'c':
      stepper2.moveTo(0);
      break;
    }
  }
  /*if(!limitSwitchB_2.touched()){*/
  stepper1.run();
  stepper2.run();
  /*}*/
}

/*
void limitSwitchOn(){
  
  if(limitSwitch.touched() )      // If the limit switch is touched, the  return value is true.
  {
    Serial.println("State: DOWN.");
    delay(1);
    while(limitSwitch.touched() )
    {
      ;// Repeat check the switch state, until released.
    }
    delay(2);
  }
  if(!limitSwitch.touched() )
  {
    Serial.println("State: UP.");
    delay(1);
    while(!limitSwitch.touched() )
    {
      ;
    }
    delay(2);
  }
}
*/
/* ----- 外部との接続 ----- */
/* シリアルから数字を受け取る(未完成) */
/*long serialNumberCatch(){
  byte data_size = Serial.available();

  if(data_size > 2){
    delay(20);
    data_size = Serial.available();
    byte buf[data_size];
 
    for (byte i = 0 ; i < data_size; i++){
      buf[i] = Serial.read() - '0';
    }

    long res = 0;
    long dub = 1;
    for(byte j = data_size-3; j > 0; j--){
      res = res + (buf[j]*dub);
      dub*=10;
    }
    res = res + (buf[0]*dub);
    Serial.println(res);
    return(res);
  }
  return(0);
}*/
