#include <Arduino.h>
#include <VL53L0X.h>

#include "CAS.h"
#include "MK_encoder_motor_driver.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define NUMBER_OF_MEASYREMENTS 5  //  测量次数
#define TRIGGER_DISTANCE 100  //  防碰撞触发距离(单位:mm)
VL53L0X sensor;

TaskHandle_t CAS_Task_TaskHandle; // CAS_Task任务句柄

/* 防碰撞初始化 */
int CAS_Init(){
  sensor.setTimeout(500);
  Wire.begin();
  if (!sensor.init())
  {
    Serial.println("[CAS_Task]Failed to detect and initialize sensor!");
    return 1;
  }
  sensor.startContinuous();
  return 0;
}

/* 防碰撞保护任务 */
void CAS_Task(void * pvParameters){
  #if CAS_DEBUG_MODE == 1
    Serial.print("[CAS_Task]CAS_Task running on core ");
    Serial.println(xPortGetCoreID());
  #endif

  //esp_task_wdt_add(NULL);  // 添加当前任务到看门狗监控列表

  if(CAS_Init() == 1){
    // 初始化失败删除任务
    #if CAS_DEBUG_MODE == 1
      Serial.println("[CAS_Task]Init ERROR!");
      Serial.println("[CAS_Task]Ending MK_Task");
    #endif
    vTaskDelete(NULL);
  }

  uint32_t ranging_distance = 0;  // 测距距离变量初始化
  CAS_flag = CAS_READY;
  while(1){
    ranging_distance = 0; //  清空数据

    if(CAS_flag == CAS_TRIGGERED){
      // 已触发防碰撞保护，尚未复位，则直接跳出
      #if CAS_DEBUG_MODE == 1
        Serial.println("[CAS_Task]CAS wait.");
      #endif
      vTaskDelay(pdMS_TO_TICKS(20));
      //esp_task_wdt_reset(); //  喂狗
      continue;
    } 

    if(CAS_flag == CAS_TEMPORARY_RELEASE){
      // 临时关闭防碰撞保护30s
      #if CAS_DEBUG_MODE == 1
        Serial.println("[CAS_Task]CAS Temporary release.");
      #endif
      vTaskDelay(pdMS_TO_TICKS(30000)); 
      CAS_flag = CAS_READY; // 启动防碰撞保护
    }

    for(int i=0;i < NUMBER_OF_MEASYREMENTS ;i++){
      // 读取多次数据取平均值
      ranging_distance = sensor.readRangeContinuousMillimeters() + ranging_distance;
    }
    

    if(sensor.timeoutOccurred()){
      #if CAS_DEBUG_MODE == 1
        Serial.println("[CAS_Task]VL53L0X read timeout!");
      #endif
      continue;  // 读取发生超时错误，结束本次测量
    }
    else{
      ranging_distance = ranging_distance / NUMBER_OF_MEASYREMENTS;
    }

    #if CAS_DEBUG_MODE == 1
        Serial.printf("[CAS_Task]ranging_distance = %d\n",ranging_distance);
    #endif

    if(CAS_flag == CAS_READY && ranging_distance <= 100){
      #if CAS_DEBUG_MODE == 1
        Serial.println("[CAS_Task]CAS Triggered!");
      #endif
      CAS_flag = CAS_TRIGGERED; //  设置状态标记为已触发碰撞保护
      //  清空目标速度数据
      motor0_L_TargetSpeed = 0;
      motor1_L_TargetSpeed = 0;
      motor2_L_TargetSpeed = 0;
      motor3_L_TargetSpeed = 0;
      motor0_R_TargetSpeed = 0;
      motor1_R_TargetSpeed = 0;
      motor2_R_TargetSpeed = 0;
      motor3_R_TargetSpeed = 0;
      //  强制停止底盘运动
      MK_stop();
    }

    vTaskDelay(pdMS_TO_TICKS(50));
    //esp_task_wdt_reset(); //  喂狗

  }

  #if CAS_DEBUG_MODE == 1
    Serial.println("[CAS_Task]Ending CAS_Task");
  #endif
    vTaskDelete(NULL);
}

/* 防碰撞任务初始化 */
void run_CAS_Task_on_Core_0(){
  xTaskCreatePinnedToCore(
                    CAS_Task,   /* Task function. */
                    "CAS_Task",     /* name of task. */
                    4096,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &CAS_Task_TaskHandle,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
}