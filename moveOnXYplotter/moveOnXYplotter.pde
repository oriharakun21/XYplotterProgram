import processing.serial.*;

Serial serial;

int moX, moY;
int[] flag = new int [2];

void setup(){
  size(900,1200);
  serial = new Serial(this, "COM3", 9600);
}

void draw(){
  drawPicMain();
}

void mouseClicked(){
  arduinoSerialSend();
}


/* 座標の送る部分 */
void arduinoSerialSend(){
  serial.write('H');
  serial.write(flag[0]);
  serial.write(flag[1]);
  
  // print(flag[0]);
  // println(flag[1]);
}

/* サーボのオンオフ(送信) */ 

/* 描写 */
void drawPicMain(){
  background(255);
  /* プロッタ */
  fill(30, 80, 255);
  rect(100, 400, 700, 700);
  fill(255, 255, 255);
  rect(110, 410, 680, 680);
  
  /* 移動位置 */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(mouseX > (i*80+150) && mouseX < (i*80+180) && mouseY > (j*80+450) && mouseY < (j*80+480)){
        if(mousePressed){
            moX = i*80+150;
            moY = j*80+450;
            flag[0] = i;
            flag[1] = j;
        }
        fill(30, 80, 255);
      }
    rect(i*80+150, j*80+450, 30, 30);
    fill(255);
    }
  }
  if(!(moX == 0) && !(moY == 0)){
    fill(30, 80, 255);
    rect(moX, moY, 30, 30);
  }
  
  /* スイッチ */
  for(int k = 0; k < 4; k++){
    if(mouseX > (k*150+100) && mouseX < (k*150+200) && mouseY > (100) && mouseY < (150)){
      fill(255, 0, 0);
      if(mousePressed){
        serial.write('G');
        serial.write(k);
      }
    }
    rect(k * 150 +100, 100, 100, 50);
    fill(255);
  }
}
