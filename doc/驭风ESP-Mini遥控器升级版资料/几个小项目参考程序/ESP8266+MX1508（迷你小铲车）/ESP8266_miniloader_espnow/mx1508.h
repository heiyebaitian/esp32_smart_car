
#define AIN1 4  // 电机1
#define AIN2 15  // 
#define BIN3 14  // 电机2
#define BIN4 12  // 

#define MAX_SPEED 180     //0-255

boolean d1,d2;  
int spd_motor1, spd_motor2;     //电机速度值
int lpad_x=127,lpad_y=127,rpad_x=127,rpad_y=127;   //摇杆初始值


void mx1508_ini(){   //
  pinMode(AIN1, OUTPUT); 
  pinMode(AIN2, OUTPUT); 
  pinMode(BIN3, OUTPUT); 
  pinMode(BIN4, OUTPUT);
}

//分别对应M1,M2的默认转动方向
void dcMotorIni(bool dir1, bool dir2){ 
  d1=dir1;
  d2=dir2;
}

//将遥杆数据转换成电机运行的数据
void Mapdata(){
    //分别读左右摇杆数据
    int valLX = map(lpad_x, 0, 255, -MAX_SPEED, MAX_SPEED);
    int valLY = map(lpad_y, 255, 0, -MAX_SPEED, MAX_SPEED); 
//    int valRX = map(rpad_x, 0, 255, -MAX_SPEED, MAX_SPEED);
//    int valRY = map(rpad_y, 255, 0, -MAX_SPEED, MAX_SPEED);
    //差速转向
    spd_motor1 = valLY + valLX;
    spd_motor2 = valLY - valLX;
    //左右摇杆叠加
//    spd_motor1 += valRY + valRX;
//    spd_motor2 += valRY - valRX;
    //限制减速电机的静止启动范围
    if (abs(spd_motor1)<20) spd_motor1=0;
    if (abs(spd_motor2)<20) spd_motor2=0;
    spd_motor1 = constrain(spd_motor1,-255,255);
    spd_motor2 = constrain(spd_motor2,-255,255);
}

//根据方向及PWM值运行电机
void M1Forward(){
  analogWrite(AIN1, ( d1 ? 0 : abs(spd_motor1) ));
  analogWrite(AIN2, ( d1 ? abs(spd_motor1) : 0 ));
}
void M1Backward(){
  analogWrite(AIN2, ( d1 ? 0 : abs(spd_motor1) ));
  analogWrite(AIN1, ( d1 ? abs(spd_motor1) : 0 ));
}
void M1Release(){
  analogWrite(AIN1, 0 );
  analogWrite(AIN2, 0 );    
}

void M2Forward(){
  analogWrite(BIN3, ( d2 ? 0 : abs(spd_motor2) ));
  analogWrite(BIN4, ( d2 ? abs(spd_motor2) : 0 ));
}
void M2Backward(){
  analogWrite(BIN4, ( d2 ? 0 : abs(spd_motor2) ));
  analogWrite(BIN3, ( d2 ? abs(spd_motor2) : 0 ));
}
void M2Release(){
  analogWrite(BIN3, 0 );
  analogWrite(BIN4, 0 );    
}

//根据速度值判断方向运行电机
void run_motors(){
  if (spd_motor1 > 0) {
    M1Forward();
    } else if (spd_motor1 < 0) {
    M1Backward();
    } else M1Release();
  if (spd_motor2 > 0) {
    M2Forward();
    } else if (spd_motor2 < 0) {
    M2Backward();
    } else M2Release();
}
