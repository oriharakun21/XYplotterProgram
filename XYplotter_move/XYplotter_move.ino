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

/* サーボ */
MePort port(PORT_4);
Servo myservo1;
Servo myservo2;
int16_t servo1pin =  port.pin1();
int16_t servo2pin =  port.pin2();

/* 変更できる変数たち */
const int MaxSpeed = 10000;
const int Acceleration = 10000;

/* フラグ */
int firstRun = 0;

/*----------------- SET UP ---------------*/
void setup() {
  Serial.begin(9600);
  printReadMe();
  Serial.println("Start.");
  
  // Stepper
  stepper1.setMaxSpeed(MaxSpeed);
  stepper1.setAcceleration(Acceleration);
  stepper2.setMaxSpeed(MaxSpeed);
  stepper2.setAcceleration(Acceleration);

  // サーボ
  myservo1.attach(servo1pin);
  myservo2.attach(servo2pin);
  myservo1.write(0); 
  myservo2.write(0); 
  delay(100);
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
    if(lsFlag() != 4 ){
      stepper1.move(-500);
      stepper1.run();
      }
    if(lsFlag() !=  1){
      stepper2.move(-500);
      stepper2.run();
      }
      if(lsFlag() == 5){
        stepper1.setCurrentPosition(0); /* 0ポイントに設定 */
        stepper2.setCurrentPosition(0);
        break;
      }
    }
}

/* リミットスイッチに当たったら数値を返すだけ */
int lsFlag(){
  if(limitSwitchA_1.touched() && !limitSwitchB_2.touched()){
    return 1;
  } else if(limitSwitchA_2.touched() && !limitSwitchB_1.touched()){
    return 2;
  } else if(!limitSwitchA_2.touched() && limitSwitchB_1.touched()){
    return 3;
  } else if(!limitSwitchA_1.touched() && limitSwitchB_2.touched()){
    return 4;
  } else if(limitSwitchA_1.touched() && limitSwitchB_2.touched()){
    /* 1と4が同時に押されているとき */
    return 5;
  } else if(limitSwitchA_2.touched() && limitSwitchB_1.touched()){
    /* 2と3が同時に押されているとき */
    return 6;
  } else {
    return 0;
  }
}

/* 最初の注意書き */
void printReadMe(){
  Serial.println("0:stepper1,moveTo,0");
  Serial.println("1:stepper2,moveTo,0");
  Serial.println("2:stepper1,moveTo,1000");
  Serial.println("3:stepper2,moveTo,1000");
  Serial.println("4:stepper1,move,-2000");
  Serial.println("5:stepper1,move,2000");
  Serial.println("6:stepper2,move,-2000");
  Serial.println("7:stepper2,move,2000");
  Serial.println("8:servo1,write,0");
  Serial.println("9:servo1,write,135");
  Serial.println("a:servo2,write,0");
  Serial.println("b:servo2,write,130");
}

/* サーボの動き */
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
      stepper2.moveTo(0);
      break;
      case '2':
      stepper1.moveTo(1000);
      break;
      case '3':
      stepper2.moveTo(1000);
      break;
      case '4':
      stepper1.move(-3000);
      break;
      case '5':
      stepper1.move(3000);
      break;
      case '6':
      stepper2.move(-3000);
      break;
      case '7':
      stepper2.move(3000);
      break;
      case '8':
      myservo1.write(0); 
      delay(1000);
      break;
      case '9':
      myservo1.write(140);
      delay(1000); 
      break;
      case 'a':
      myservo2.write(0); 
      delay(1000);
      break;
      case 'b':
      myservo2.write(130);
      delay(1000); 
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
