import processing.serial.*;

Serial serial;

int moX, moY; // マウスの座標をとる（ほぼデバック用）
int[] gridFlag = new int [2]; // 座標を保持してフラグとして送る
int[] servoFlag = new int [2]; // サーボのオンオフ

/* 枠組みやサイズなどを指定 */
// XYプロッターの操作部分
final int BORDER = 700; // 外枠
final int UPPERLEFT_X = 100; // 左上のX
final int UPPERLEFT_Y = 400; // 左上のY
final int POINTDOT = 30; // 座標のポイント
final int POINTMARGIN = 50; // ポイントのマージン
final int POINTDM = POINTDOT+POINTMARGIN; // サイズとマージンをまとめたもの
final int UPPERLEFTPOINT_X = UPPERLEFT_X+POINTMARGIN; // ポイントの左上のX
final int UPPERLEFTPOINT_Y = UPPERLEFT_Y+POINTMARGIN; // ポイントの左上のY

// サーボの操作部分
final int SERVOBORDER_W = 160; 
final int SERVOBORDER_H = 90;
int[] SERVOBOTTON_X = {320, 500};
int[] SERVOBOTTON_Y = {180, 180};


// 色
final int R1 = 30, G1 = 80, B1 = 255;  // 基本カラー
final int BGC = 255; // 背景色

// 画像
PImage img;

/* ----- ここからセットアップ ----- */
void setup(){
  size(900,1200);
  // serial = new Serial(this, "COM3", 9600);
  img = loadImage("img1.png");
}

/* ----- ここからドロー ------ */
void draw(){
   background(BGC);
   image(img, 100, 100, 180, 220);
   drawServoUpDown(0);
   drawServoUpDown(1);
   drawPlotter();
   println(servoFlag[1]);
}

/* ------ マウスが押して話されたとき ------ */
void mouseClicked(){
  /*arduinoSerialSendGrid();
  arduinoSerialSendServo(0);
  arduinoSerialSendServo(1);*/
}

/* 座標の送る部分 */
void arduinoSerialSendGrid(){
  /* 座標のフラグを送る */
  if((mouseX > UPPERLEFT_X) && (mouseX < UPPERLEFT_X + BORDER)){
    if((mouseY > UPPERLEFT_Y) && (mouseY < UPPERLEFT_Y + BORDER)){
      serial.write('H'); // フラグ
      serial.write(gridFlag[0]);
      serial.write(gridFlag[1]);
    }
  }
}

  /* サーボのオンオフ(送信) */ 
void arduinoSerialSendServo(int x){
  if((mouseX > SERVOBOTTON_X[x]) && (mouseX < SERVOBOTTON_X[x] + SERVOBORDER_W)){
    if((mouseY > SERVOBOTTON_Y[x]) && (mouseY < SERVOBOTTON_Y[x] + SERVOBORDER_H)){
      serial.write('G'); // フラグ
      serial.write(servoFlag[x]);
    }
  }
}

/* 描写 */
void drawPlotter(){
  /* プロッタ */
  fill(R1, G1, B1);
  rect(UPPERLEFT_X, UPPERLEFT_Y, BORDER, BORDER);
  fill(BGC);
  rect(UPPERLEFT_X+10, UPPERLEFT_Y+10, BORDER-20, BORDER-20);
  
  /* 移動位置 */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if((mouseX > (i*POINTDM+UPPERLEFTPOINT_X)) && (mouseX < (i*POINTDM+UPPERLEFTPOINT_X+POINTDOT))){
        if((mouseY > (j*POINTDM+UPPERLEFTPOINT_Y)) && (mouseY < (j*POINTDM+UPPERLEFTPOINT_Y+POINTDOT))){
          if(mousePressed){
            moX = i*POINTDM+UPPERLEFTPOINT_X;
            moY = j*POINTDM+UPPERLEFTPOINT_Y;
            gridFlag[0] = i;
            gridFlag[1] = j;
          }
          fill(R1, G1, B1);
        }
      }
    rect(i*POINTDM+UPPERLEFTPOINT_X, j*POINTDM+UPPERLEFTPOINT_Y, POINTDOT, POINTDOT);
    fill(BGC);
    }
  }
  if(!(moX == 0) && !(moY == 0)){
    fill(R1, G1, B1);
    rect(moX, moY, POINTDOT, POINTDOT); // 最後にタッチされたものをマーク
  }
}

/* スイッチ */
void drawServoUpDown(int x){
  fill(BGC);
  if((mouseX > (SERVOBOTTON_X[x])) && (mouseX < (SERVOBOTTON_X[x]+SERVOBORDER_W))) {
    if((mouseY > (SERVOBOTTON_Y[x])) && (mouseY < (SERVOBOTTON_Y[x]+SERVOBORDER_H))){
      fill(R1, G1, B1);
        if(mousePressed){
          if(servoFlag[x] == 0){
            servoFlag[x] = 1;
          } else {
            servoFlag[x] = 0;
          }
        }
        }
      }
      if(servoFlag[x] == 1){
    fill(R1, G1, B1);
  }
    rect(SERVOBOTTON_X[x], SERVOBOTTON_Y[x], SERVOBORDER_W, SERVOBORDER_H);
    fill(BGC);
}
