#include <Arduino.h>
#include "ESP32_NOW.h"
#include "WiFi.h"
#include "espnow.h"
#include "MK_encoder_motor_driver.h"
#include "joystick_analysis.h"

#define DEBUG_MODE 1

const uint8_t channel = 1;

uint32_t motor0_L_TargetSpeed = 0;
uint32_t motor1_L_TargetSpeed = 0;
uint32_t motor2_L_TargetSpeed = 0;
uint32_t motor3_L_TargetSpeed = 0;
uint32_t motor0_R_TargetSpeed = 0;
uint32_t motor1_R_TargetSpeed = 0;
uint32_t motor2_R_TargetSpeed = 0;
uint32_t motor3_R_TargetSpeed = 0;

void FreeRTOS_Task_Init(){
  run_MK_Task_on_Core_0(); // MK底盘驱动任务初始化
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  InitESPNow(); // 初始化espnow
  FreeRTOS_Task_Init(); // FreeRTOS任务初始化

}

void loop() {
  // put your main code here, to run repeatedly:

}
