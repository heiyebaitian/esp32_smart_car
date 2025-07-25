#include <Servo.h>

static const int servoPin = 19;       //舵机引脚
int posDegrees = 60; //默认角度
int clawpos = 65;   //遥控角度
int clawmin = 40;  //夹取,建议先测试出合适的角度      
int clawmax = 95;  //松开，建议先测试出合适的角度 
bool clawmode = 0;  //前爪模式，一种自动呼吸，一种遥控

Servo servo1;

void servo_ini(){
  servo1.attach(
        servoPin, 
        Servo::CHANNEL_NOT_ATTACHED, 
        0,
        180                 //最小最大角度
    );
  servo1.write(posDegrees);  //先运行到默认角度
}

void run_claw(){        //前爪动作，两种模式
  if (!clawmode){
    servo1.write(posDegrees);
  } else {
    servo1.write(clawpos);
  }
}
