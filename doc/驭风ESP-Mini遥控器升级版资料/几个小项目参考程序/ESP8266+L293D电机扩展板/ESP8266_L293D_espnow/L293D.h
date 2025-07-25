
#define PIN_D1 5  // gpio5 = D1  PWM_A
#define PIN_D2 4  // gpio4 = D2  PWM_B
#define PIN_D3 0  // gpio0 = D3  DA (A- A+) 
#define PIN_D4 2  // gpio2 = D4  DB (B- B+)


#define MAX_SPEED 180     //0-255

boolean d1,d2;  
int spd_motor1, spd_motor2;     //电机速度值
int lpad_x=127,lpad_y=127,rpad_x=127,rpad_y=127;   //摇杆初始值


void L293D_ini(){   //
  pinMode(PIN_D1, OUTPUT); 
  pinMode(PIN_D2, OUTPUT); 
  pinMode(PIN_D3, OUTPUT); 
  pinMode(PIN_D4, OUTPUT);
  analogWrite(PIN_D1, 0);   // PWM_A 0
  analogWrite(PIN_D2, 0);   // PWM_B 0
  digitalWrite(PIN_D3, LOW);    // DA LOW
  digitalWrite(PIN_D4, LOW);    // DB LOW
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
    int valRX = map(rpad_x, 0, 255, -MAX_SPEED, MAX_SPEED);
    int valRY = map(rpad_y, 255, 0, -MAX_SPEED, MAX_SPEED);
    //差速转向
    spd_motor1 = valLY + valLX;
    spd_motor2 = valLY - valLX;
    //左右摇杆叠加
    spd_motor1 += valRY + valRX;
    spd_motor2 += valRY - valRX;
    //限制减速电机的静止启动范围
    if (abs(spd_motor1)<20) spd_motor1=0;
    if (abs(spd_motor2)<20) spd_motor2=0;
    spd_motor1 = constrain(spd_motor1,-255,255);
    spd_motor2 = constrain(spd_motor2,-255,255);
}

//根据方向及PWM值运行电机
void M1Forward(){
  analogWrite(PIN_D1, abs(spd_motor1) );
  digitalWrite(PIN_D3, ( d1 ? 1 : 0 ));
}
void M1Backward(){
  analogWrite(PIN_D1, abs(spd_motor1) );
  digitalWrite(PIN_D3, ( d1 ? 0 : 1 ));
}
void M1Release(){
  analogWrite(PIN_D1, 0 );
  digitalWrite(PIN_D3, ( d1 ? 1 : 0 ));    
}

void M2Forward(){
  analogWrite(PIN_D2, abs(spd_motor2) );
  digitalWrite(PIN_D4, ( d2 ? 1 : 0 ));
}
void M2Backward(){
  analogWrite(PIN_D2, abs(spd_motor2) );
  digitalWrite(PIN_D4, ( d2 ? 0 : 1 ));
}
void M2Release(){
  analogWrite(PIN_D2, 0 );
  digitalWrite(PIN_D4, ( d2 ? 1 : 0 ));    
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
