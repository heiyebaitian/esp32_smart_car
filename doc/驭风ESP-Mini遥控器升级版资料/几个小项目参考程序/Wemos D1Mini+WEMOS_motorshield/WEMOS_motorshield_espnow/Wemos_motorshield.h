#include <WEMOS_Motor.h>

//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30,_MOTOR_A, 1000);//Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B

#define MAX_SPEED 180     //0-255

boolean d1,d2;  
int spd_motor1, spd_motor2;     //电机速度值
int lpad_x=127,lpad_y=127,rpad_x=127,rpad_y=127;   //摇杆初始值


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
  M1.setmotor( ( d1 ? _CW : _CCW ), (map(abs(spd_motor1),0,255,0,100)));
}
void M1Backward(){
  M1.setmotor( ( d1 ? _CCW : _CW ), (map(abs(spd_motor1),0,255,0,100)));
}
void M1Release(){
  M1.setmotor( ( d1 ? _CW : _CCW ), 0);    
}

void M2Forward(){
  M2.setmotor( ( d2 ? _CW : _CCW ), (map(abs(spd_motor2),0,255,0,100)));
}
void M2Backward(){
  M2.setmotor( ( d2 ? _CCW : _CW ), (map(abs(spd_motor2),0,255,0,100)));
}
void M2Release(){
  M2.setmotor( ( d2 ? _CW : _CCW ), 0);    
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
