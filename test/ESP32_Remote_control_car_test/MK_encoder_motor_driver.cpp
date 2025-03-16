#include "MK_encoder_motor_driver.h"
#include "encoder_motor.h"
#include "encoder_motor_lib.h"
#include "driver/gpio.h"  // 包含 GPIO 定义的头文件

const int16_t speed_rpm = 60;

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

void MK_Init(){
  g_encoder_motor_0.Init();
  g_encoder_motor_1.Init();
  g_encoder_motor_2.Init();
  g_encoder_motor_3.Init();
}

void MK_Stop(){
  g_encoder_motor_0.Stop();
  g_encoder_motor_1.Stop();
  g_encoder_motor_2.Stop();
  g_encoder_motor_3.Stop();
}

void MK_RunSpeed(double groupA_percentage, double groupB_percentage){
  g_encoder_motor_0.RunSpeed(int(speed_rpm * groupA_percentage));
  g_encoder_motor_1.RunSpeed(int(speed_rpm * groupB_percentage));
  g_encoder_motor_2.RunSpeed(int(speed_rpm * groupA_percentage));
  g_encoder_motor_3.RunSpeed(int(speed_rpm * groupB_percentage));
}

