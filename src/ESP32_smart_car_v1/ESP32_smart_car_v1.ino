#include <Arduino.h>
#include <Wire.h>
#include "ESP32_NOW.h"
#include "WiFi.h"
#include "espnow.h"
#include "MK_encoder_motor_driver.h"
#include "joystick_analysis.h"
#include "CAS.h"
#include "TaskUtils.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

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

CAS_STATE CAS_flag = CAS_READY;  // 防碰撞系统触发标记 CAS_TRIGGERED：触发碰撞保护   CAS_READY：无碰撞风险   CAS_TEMPORARY_RELEASE：临时解除碰撞保护



/* FreeRTOS任务初始化 */
void FreeRTOS_Task_Init(){
  run_MK_Task_on_Core_0(); // MK底盘驱动任务初始化
  delay(500);
  run_CAS_Task_on_Core_0(); // CAS防碰撞系统任务初始化
  run_TaskDaemon_Task_on_Core_1(); // 任务守护进程
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  
  Wire.begin(); //  I2C初始化
  InitESPNow(); // 初始化espnow
  FreeRTOS_Task_Init(); // FreeRTOS任务初始化

}

void loop() {
  // put your main code here, to run repeatedly:

}
