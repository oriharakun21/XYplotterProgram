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
int autoCount = 0;
double sycle = 0.00;

/* 受け取るフラグ */
char chara;
int flag[2] = {0, 0};
int fServo[2] = {0, 0};
int mode = 0;
long modeTimer ;

/*----------------- SET UP ---------------*/
void setup() {
  Serial.begin(9600);
  // printReadMe();
  
  // Stepper
  stepper1.setMaxSpeed(MaxSpeed);
  stepper1.setAcceleration(Acceleration);
  stepper2.setMaxSpeed(MaxSpeed);
  stepper2.setAcceleration(Acceleration);

  // サーボ
  myservo1.attach(servo1pin);
  myservo2.attach(servo2pin);
}

/*------------------ LOOP -------------------*/
void loop() {
  if(firstRun == 0){
    FirstSetup();
    firstRun = 1;
  }

  // easyMove();
  serialNumberCatch();
}


/* 初期移動（0地点を右前に設定・） */
void FirstSetup(){
  myservo1.write(0);
  delay(100);
  myservo2.write(0); 
  delay(100);
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
  Serial.println("Start.");
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
  stepper1.run();
  stepper2.run();
}

/* ----- 外部との接続 ----- */
/* シリアルから座標を受け取る */
void serialNumberCatch(){
  if(Serial.available() > 2 ){
    chara = Serial.read();
    if(chara == 'h'){
      flag[0] = Serial.read();
      flag[1] = Serial.read();
    }else if(chara == 'g'){
      fServo[0] = Serial.read();
      fServo[0] = Serial.read();
    } else if(chara == 'f'){
      fServo[1] = Serial.read();
      fServo[1] = Serial.read();
    } else if(chara == 'e'){
      mode = Serial.read();
      mode = Serial.read();
    }
    
    switch(flag[1]){
      case 0:
      stepper1.moveTo(0);
      break;
      case 1:
      stepper1.moveTo(30000 / 8);
      break;
       case 2:
      stepper1.moveTo(30000 / 8 * 2);
      break;
       case 3:
      stepper1.moveTo(30000 / 8 * 3);
      break;
       case 4:
      stepper1.moveTo(30000 / 8 * 4);
      break;
       case 5:
      stepper1.moveTo(30000 / 8 * 5);
      break;
      case 6:
      stepper1.moveTo(30000 / 8 * 6);
      break;
       case 7:
      stepper1.moveTo(30000 / 8 * 7);
      break;
    }
    switch(flag[0]){
      case 7:
      stepper2.moveTo(0);
      break;
      case 6:
      stepper2.moveTo(40000 / 8);
      break;
       case 5:
      stepper2.moveTo(40000 / 8 * 2);
      break;
       case 4:
      stepper2.moveTo(40000 / 8 * 3);
      break;
       case 3:
      stepper2.moveTo(40000 / 8 * 4);
      break;
       case 2:
      stepper2.moveTo(40000 / 8 * 5);
      break;
      case 1:
      stepper2.moveTo(40000 / 8 * 6);
      break;
       case 0:
      stepper2.moveTo(40000 / 8 * 7);
      break;
    }
  }
  if(fServo[0] == 0){
      myservo1.write(0);
    } else if(fServo[0] == 1){
      myservo1.write(160);
    }
  if(fServo[1] == 0){
      myservo2.write(0);
    } else if(fServo[1] == 1){
      myservo2.write(120);
    }
   if(mode == 1){
    modeTimer = millis();
    while(millis() < modeTimer + 74000){
      autoMove();
    }
     mode = 0;
   }
  stepper1.run();
  stepper2.run();
}

void autoMove(){
  /* -- フェイズ0 -- */
  if(autoCount == (0)){
    long start = millis();
    if(start == millis()){
      stepper1.moveTo(0.00);
      stepper2.moveTo(40000 / 8.00 * (2.00 + sycle));
    }
    while(millis() < start + 2000) {
      stepper1.run();
      stepper2.run();
      }
    if(millis() >= start + 2000){
      autoCount++;
    }
    /* -- フェイズ1 -- */
  } else if(autoCount == 1){
    stepper1.moveTo(30000 / 8.00 * 7.30);
    stepper2.moveTo(40000 / 8.00 * (2.00 + sycle));
    while(!limitSwitchB_1.touched()){
      stepper1.run();
      stepper2.run();
    }
    if(limitSwitchB_1.touched()){
      myservo1.write(180);
      autoCount = 2;
    }
    /* -- フェイズ2 -- */
  } else if(autoCount == 2){
    stepper1.moveTo(0);
    stepper2.moveTo(40000 / 8.00 * (2.00 + sycle));
    while(!limitSwitchB_2.touched()){
      stepper1.run();
      stepper2.run();
    }
    if(limitSwitchB_2.touched()){
      myservo1.write(0);
      autoCount = 3;
    }
  } else {
    sycle = sycle + 1.00;
    autoCount = 0;
    if(sycle == 5){
      sycle = 0;
      firstRun = 0;
    }
    Serial.println("end");
  }
}

