const byte CUBE_SIZE=5;

// 595 control
const int latchPin = 7;
const int clockPin = 8;
const int dataPin = 12;
const int enablePin = 11;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  digitalWrite(latchPin, LOW);
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, LOW);

  pinMode(enablePin, OUTPUT);
  digitalWrite(latchPin, LOW);
  //Serial.begin(9600);
  //Serial.println("reset");
}

byte fontE[5]={
  B11111,
  B10000,
  B11110,
  B10000,
  B11111
};

byte CubeBuff[CUBE_SIZE][CUBE_SIZE];

void CubeClear(boolean val) {
  for (byte i = 0; i < CUBE_SIZE; ++i) {
    for (byte j = 0; j < CUBE_SIZE; ++j) {
      CubeBuff[i][j] = 0;//val ? B11111111 : B00000000;
    }
  }
}

void CubeShow(){
  for (int plane=0;plane<CUBE_SIZE;plane++){   
    byte b1 = (CubeBuff[plane][0] << 3) | (CubeBuff[plane][1] >> 2);
    byte b2 = (CubeBuff[plane][1] << 6) | (CubeBuff[plane][2] << 1) | (CubeBuff[plane][3] >> 4);
    byte b3 = (CubeBuff[plane][3] << 4) | (CubeBuff[plane][4] >> 1);
    byte b4 = (CubeBuff[plane][4] << 7) | (1 << (4-plane));

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, b4);  // MSB
    shiftOut(dataPin, clockPin, LSBFIRST, b3);
    shiftOut(dataPin, clockPin, LSBFIRST, b2);
    shiftOut(dataPin, clockPin, LSBFIRST, b1);    
    digitalWrite(latchPin, HIGH);
  }
}

// Bresenham 3D
// based on https://gist.github.com/yamamushi/5823518
void DrawLine(int x1, int y1, int z1, int x2, int y2, int z2){
    
    int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    int point[3];
    
    point[0] = x1;
    point[1] = y1;
    point[2] = z1;
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    x_inc = (dx < 0) ? -1 : 1;
    l = abs(dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = abs(dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;
    
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            CubeBuff[point[2]][point[0]] = CubeBuff[point[2]][point[0]] | (1 << point[1]);
            // output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point[0] += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            CubeBuff[point[2]][point[0]] = CubeBuff[point[2]][point[0]] | (1 << point[1]);
            // output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point[1] += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            CubeBuff[point[2]][point[0]] = CubeBuff[point[2]][point[0]] | (1 << point[1]);
            // output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point[2] += z_inc;
        }
    }
    CubeBuff[point[2]][point[0]] = CubeBuff[point[2]][point[0]] | (1 << point[1]);
    //output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
}

void RandomTwinkle(){
  CubeClear(false);

  byte randPlane = random(5);
  byte randRow = random(5);
  byte randCol = random(5);
  CubeBuff[randPlane][randRow] = (1 << randCol);
}

void RandomVerticalLine(){
  CubeClear(false);

  byte randRow = random(5);
  byte randCol = random(5);

  byte randRow2 = random(5);
  byte randCol2 = random(5);
  DrawLine(randCol, randRow, 0, randCol2, randRow2, 4);
}

byte plane=0;
unsigned long EndTime = millis() + 1000;
byte animationStep = 1;
byte animationType = 0;

void loop() {
    CubeShow();

     
     if (millis() > EndTime) {
        
        /*
        for(byte row=0; row < CUBE_SIZE; row++){
           CubeBuff[plane][row] = fontE[row];
        }
        plane++;
        if(plane==CUBE_SIZE) { plane=0; }
        */


        
        // RandomTwinkle();
        
        //RandomVerticalLine();

        Stickman(true);
        EndTime = millis() + 10000;
        Stickman(false);
        EndTime = millis() + 30000; //30s

        // AnimateCube();
        // AnimateXX();
        // ShowCube();

      /*
        if(animationType == 0){
            AnimateCube();
        }
        else if (animationType == 1){
            AnimateXX();            
        }
        else if (animationType > 1){
          animationType = 0;
        }
        
        if (animationStep == 0){
          animationType++;
        }        
        
        animationStep++;
        EndTime = millis() + 500;
        */
     }

    
  //}

/*
    byte b1 = B10000000;
    byte b2 = 0;
    byte b3 = 0;
    byte b4 = B10000;


    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, b4);  // MSB
    shiftOut(dataPin, clockPin, LSBFIRST, b3);
    shiftOut(dataPin, clockPin, LSBFIRST, b2);
    shiftOut(dataPin, clockPin, LSBFIRST, b1);
    digitalWrite(latchPin, HIGH);

    delay(10000);
*/
  
/*
 // Keep this as intro test
    for (int fl=5; fl>0; fl--){
      for (int i=1; i<26;i++){
        int b4=0;
        int b3=0;
        int b2=0;
        int b1=0;
  
        b4 = 1;
        b4 = b4 << (fl+2); // floor
        
        if(i > 24){
          b4 = b4 + 1;
        } 
        
        if(i>16 && i<25){
          b3 = 1;
          b3 = b3 << (i-17);
        } 
        if (i>8 && i<17){
          b2 = 1;
          b2 = b2 << (i-9);
        }
        if (i<9){
          b1 = 1;
          b1 = b1 << (i-1);
        }
  
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, b4);
        shiftOut(dataPin, clockPin, MSBFIRST, b3);
        shiftOut(dataPin, clockPin, MSBFIRST, b2);
        shiftOut(dataPin, clockPin, MSBFIRST, b1);
        digitalWrite(latchPin, HIGH);
        delay(10);
      }
    }
    */
}

void AnimateCube(){
    switch(animationStep){
          // Bottom plane z=0
          case 1:
            DrawLine(0,0,0, 4,0,0);
            break;
          case 2:
            DrawLine(0,0,0, 0,4,0);
            break;
          case 3:
            DrawLine(0,4,0, 4,4,0);
            break;
          case 4:
            DrawLine(4,0,0, 4,4,0);
            break;

          // Vertical lines
          case 5:
            DrawLine(0,0,0, 0,0,4);
            break;
          case 6:
            DrawLine(0,4,0, 0,4,4);
            break;
          case 7:
            DrawLine(4,4,0, 4,4,4);
            break;
          case 8:
            DrawLine(4,0,0, 4,0,4);            
            break;
  
          // top plane z=4
          case 9:
            DrawLine(0,0,4, 4,0,4);
            break;
          case 10:
            DrawLine(0,0,4, 0,4,4);
            break;
          case 11:
            DrawLine(0,4,4, 4,4,4);
            break;
          case 12:
            DrawLine(4,0,4, 4,4,4);
            break;
          default:
            animationStep=0;
            CubeClear(false);
            break;
    }
}


//  CubeBuff[point[2]][point[0]] | (1 << point[1]);
void ShowCube(){
    DrawLine(0,0,0, 4,0,0);
    DrawLine(0,0,0, 0,4,0);
    DrawLine(0,4,0, 4,4,0);
    DrawLine(4,0,0, 4,4,0);
    // Vertical lines
    DrawLine(0,0,0, 0,0,4);
    DrawLine(0,4,0, 0,4,4);
    DrawLine(4,4,0, 4,4,4);
    DrawLine(4,0,0, 4,0,4);
    // top plane z=4
    DrawLine(0,0,4, 4,0,4);
    DrawLine(0,0,4, 0,4,4);
    DrawLine(0,4,4, 4,4,4);
    DrawLine(4,0,4, 4,4,4);
}

void AnimateXX(){
    switch(animationStep){                      
          case 1:
            CubeClear(false);
            DrawLine(0,0,0, 4,4,4);
            break;
          case 2:
            DrawLine(0,0,4, 4,4,0);            
            break;
          case 3:
            DrawLine(0,2,0, 4,2,4);
            break;
          case 4:
            DrawLine(0,2,4, 4,2,0);
            break;
          case 5:
            DrawLine(0,4,0, 4,0,4);
            break;
          case 6:
            DrawLine(4,0,0, 0,4,4);
            break;    
          case 7:
            DrawLine(2,0,0, 2,4,4);
            break;
          case 8:
            DrawLine(2,0,4, 2,4,0);
            break;    
          default:
            animationStep=0;
            CubeClear(false);
            break;           
    }  
}

void Stickman(bool handsHigh){
  CubeClear(false);
  if(handsHigh){
    CubeBuff[4][2] = B10101;
    CubeBuff[3][2] = B01110;
  }
  else{
    CubeBuff[4][2] = B00100;
    CubeBuff[3][2] = B11111;
  }
  CubeBuff[2][2] = B00100;
  CubeBuff[1][2] = B01010;
  CubeBuff[0][2] = B01010;
}

