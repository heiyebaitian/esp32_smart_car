#include <Arduino.h>
#include "MK_encoder_motor_driver.h"
#include "encoder_motor.h"
#include "encoder_motor_lib.h"
#include "driver/gpio.h"  // 包含 GPIO 定义的头文件

const int16_t speed_rpm = 60;  // 最大转速

namespace {
constexpr uint32_t kPPR = 12;              // Pulses per revolution.
constexpr uint32_t kReductionRation = 90;  // Reduction ratio.

em::EncoderMotor g_encoder_motor_0(  // E0
    GPIO_NUM_13,                     // The pin number of the motor's positive pole.
    GPIO_NUM_27,                     // The pin number of the motor's negative pole.
    GPIO_NUM_18,                     // The pin number of the encoder's A phase.
    GPIO_NUM_19,                     // The pin number of the encoder's B phase.
    kPPR,                            // Pulses per revolution.
    kReductionRation,                // Reduction ratio.
    em::EncoderMotor::kAPhaseLeads   // Phase relationship (A phase leads or B phase leads, referring to the situation when
                                     // the motor is rotating forward)
);

em::EncoderMotor g_encoder_motor_1(  // E1
    GPIO_NUM_4,                      // The pin number of the motor's positive pole.
    GPIO_NUM_2,                      // The pin number of the motor's negative pole.
    GPIO_NUM_5,                      // The pin number of the encoder's A phase.
    GPIO_NUM_23,                     // The pin number of the encoder's B phase.
    kPPR,                            // Pulses per revolution.
    kReductionRation,                // Reduction ratio.
    em::EncoderMotor::kBPhaseLeads   // Phase relationship (A phase leads or B phase leads, referring to the situation when
                                     // the motor is rotating forward)
);

em::EncoderMotor g_encoder_motor_2(  // E2
    GPIO_NUM_12,                     // The pin number of the motor's positive pole.
    GPIO_NUM_17,                     // The pin number of the motor's negative pole.
    GPIO_NUM_35,                     // The pin number of the encoder's A phase.
    GPIO_NUM_36,                     // The pin number of the encoder's B phase.
    kPPR,                            // Pulses per revolution.
    kReductionRation,                // Reduction ratio.
    em::EncoderMotor::kAPhaseLeads   // Phase relationship (A phase leads or B phase leads, referring to the situation when
                                     // the motor is rotating forward)
);

em::EncoderMotor g_encoder_motor_3(  // E3
    GPIO_NUM_15,                     // The pin number of the motor's positive pole.
    GPIO_NUM_14,                     // The pin number of the motor's negative pole.
    GPIO_NUM_34,                     // The pin number of the encoder's A phase.
    GPIO_NUM_39,                     // The pin number of the encoder's B phase.
    kPPR,                            // Pulses per revolution.
    kReductionRation,                // Reduction ratio.
    em::EncoderMotor::kBPhaseLeads   // Phase relationship (A phase leads or B phase leads, referring to the situation when
                                     // the motor is rotating forward)
);
}
/* 底盘初始化 */
void MK_Init(){
  g_encoder_motor_0.Init();
  g_encoder_motor_1.Init();
  g_encoder_motor_2.Init();
  g_encoder_motor_3.Init();
}

void MK_run(){
  g_encoder_motor_0.RunSpeed(motor0_L_TargetSpeed + motor0_R_TargetSpeed);
  g_encoder_motor_1.RunSpeed(motor1_L_TargetSpeed + motor1_R_TargetSpeed);
  g_encoder_motor_2.RunSpeed(motor2_L_TargetSpeed + motor2_R_TargetSpeed);
  g_encoder_motor_3.RunSpeed(motor3_L_TargetSpeed + motor3_R_TargetSpeed);
}

/* 左摇杆停止 */
void MK_L_StopSpeed(){
  motor0_L_TargetSpeed = 0;
  motor1_L_TargetSpeed = 0;
  motor2_L_TargetSpeed = 0;
  motor3_L_TargetSpeed = 0;
}

/* 左摇杆运动 */
void MK_L_RunSpeed(double groupA_percentage, double groupB_percentage){
  motor0_L_TargetSpeed = int(speed_rpm * groupA_percentage);
  motor1_L_TargetSpeed = int(speed_rpm * groupB_percentage);
  motor2_L_TargetSpeed = int(speed_rpm * groupA_percentage);
  motor3_L_TargetSpeed = int(speed_rpm * groupB_percentage);
}

/* 右摇杆停止 */
void MK_R_StopSpeed(){
  motor0_R_TargetSpeed = 0;
  motor1_R_TargetSpeed = 0;
  motor2_R_TargetSpeed = 0;
  motor3_R_TargetSpeed = 0;
}

/* 右摇杆前置圆心旋转函数 */
void MK_R_rotateFrontAroundCenter(double percentage){
  motor0_R_TargetSpeed = 0;
  motor1_R_TargetSpeed = 0;
  motor2_R_TargetSpeed = -int(speed_rpm * percentage);
  motor3_R_TargetSpeed = int(speed_rpm * percentage);
}


/* 右摇杆中置圆心旋转函数 */
void MK_R_rotateThroughCenter(double percentage){
  motor0_R_TargetSpeed = -int(speed_rpm * percentage);
  motor1_R_TargetSpeed = int(speed_rpm * percentage);
  motor2_R_TargetSpeed = -int(speed_rpm * percentage);
  motor3_R_TargetSpeed = int(speed_rpm * percentage);
}

/* 右摇杆后置圆心逆时针旋转函数 */
void MK_R_rotateBackAroundCenter(double percentage){
  motor0_R_TargetSpeed = -int(speed_rpm * percentage);
  motor1_R_TargetSpeed = int(speed_rpm * percentage);
  motor2_R_TargetSpeed = 0;
  motor3_R_TargetSpeed = 0;
}