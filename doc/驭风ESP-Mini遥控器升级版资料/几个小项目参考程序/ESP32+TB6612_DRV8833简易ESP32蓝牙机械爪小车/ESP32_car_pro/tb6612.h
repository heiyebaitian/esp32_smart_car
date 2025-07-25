//先安装ESP32_AnalogWrite库，用法几乎和Arduino的AnalogWrite一样了
#include <Arduino.h>
#include <analogWrite.h>

//分别对应两个驱动的引脚编号，tb6612用法和L298N类似，有两种方式：
//一是PWMA和PWMB给PWM信号控制速度，其余四个脚给高低电平控制方向；
//二是PWMA和PWMB接高电平，其余四个脚给PWM信号控制速度和方向，此种用法时和drv8833模块可替换通用
#define PWMA 33       //TB6612的相关引脚
#define AIN2 25
#define AIN1 26
#define STBY 27       //使能引脚，没特殊需求时可以直接接高电平（3.3V）
#define BIN1 14
#define BIN2 12
#define PWMB 13

#define MAX_SPEED 180     //0-255

boolean d1,d2;  
int spd_motor1, spd_motor2;     //电机速度值
int lpad_x=127,lpad_y=127,rpad_x=127,rpad_y=127;   //摇杆初始值


void tb6612_ini(){   //选用上述第二种控制方式
  analogWriteResolution(AIN1, 13);  //13位精度的PWM
  analogWriteResolution(AIN2, 13);
  analogWriteResolution(BIN1, 13);
  analogWriteResolution(BIN2, 13);

  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(STBY,OUTPUT);
  digitalWrite(PWMA,HIGH);
  digitalWrite(PWMB,HIGH);
  digitalWrite(STBY,HIGH);
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
  analogWrite(BIN1, ( d2 ? 0 : abs(spd_motor2) ));
  analogWrite(BIN2, ( d2 ? abs(spd_motor2) : 0 ));
}
void M2Backward(){
  analogWrite(BIN2, ( d2 ? 0 : abs(spd_motor2) ));
  analogWrite(BIN1, ( d2 ? abs(spd_motor2) : 0 ));
}
void M2Release(){
  analogWrite(BIN1, 0 );
  analogWrite(BIN2, 0 );    
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
