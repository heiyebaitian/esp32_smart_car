#include <Arduino.h>

#include "TaskUtils.h"
#include "MK_encoder_motor_driver.h"
#include "joystick_analysis.h"
#include "CAS.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define DEBUG_MODE 1

#define TASK_ERROR_NUMBER_MAX 5 // 任务错误允许的最大次数，超过此次数将自动重启系统

uint32_t TaskDaemon_error_number = 0; //  任务错误计数器

TaskHandle_t TaskDaemon_Task_TaskHandle; // TaskDaemon_Task任务句柄

/* 任务状态检查函数 */
int Taks_State_Check(){
  if(eTaskGetState(MK_Task_TaskHandle) == eDeleted){
    // 检查MK底盘驱动任务是否删除
    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]ERROR:MK_Task is has been deleted,Trying to restart the task.");
    #endif
    vTaskDelay(pdMS_TO_TICKS(1000));
    run_MK_Task_on_Core_0();
    TaskDaemon_error_number++;
  }

  if(eTaskGetState(MK_Task_TaskHandle) == eSuspended){
    // 检查MK底盘驱动任务是否挂起
    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]Warning:MK_Task is suspended");
    #endif
  }
  
  if(eTaskGetState(CAS_Task_TaskHandle) == eDeleted){
    // 检查CAS防碰撞系统任务是否删除
    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]ERROR:CAS_Task is has been deleted,Trying to restart the task.");
    #endif
    vTaskDelay(pdMS_TO_TICKS(1000));
    run_CAS_Task_on_Core_0();
    TaskDaemon_error_number++;
  }

  if(eTaskGetState(CAS_Task_TaskHandle) == eSuspended){
    // 检查MCAS防碰撞系统任务是否挂起
    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]Warning:CAS_Task is suspended");
    #endif
  }

  if(eTaskGetState(SonoLuminAlert_Task_TaskHandle) == eDeleted){
    // 检查声光报警任务是否删除
    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]ERROR:SonoLuminAlert_Task is has been deleted,Trying to restart the task.");
    #endif
    vTaskDelay(pdMS_TO_TICKS(1000));
    run_SonoLuminAlert_Task_on_Core_1();
    TaskDaemon_error_number++;
  }

  if(eTaskGetState(SonoLuminAlert_Task_TaskHandle) == eSuspended){
    // 检查声光报警任务是否挂起
    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]Warning:SonoLuminAlert_Task is suspended");
    #endif
  }
  return 0;
}



/* FreeRTOS任务守护进程 */
void FreeRTOS_Task_Daemon(void * pvParameters){
  #if DEBUG_MODE == 1
    Serial.print("[TaskDaemon]TaskDaemon_Task running on core ");
    Serial.println(xPortGetCoreID());
  #endif

  //esp_task_wdt_add(NULL);  // 添加当前任务到看门狗监控列表
  Taks_State_Check();

  while(1){
    Taks_State_Check();

    if(TaskDaemon_error_number > TASK_ERROR_NUMBER_MAX-1){
      // 如果错误次数超限制，则重启系统
      Serial.println("[TaskDaemon]The number of TaskDaemon_error_number is too much.");
      // 触发系统错误声光报警
      if(SLA_mode < SLA_SYS_ERROR){
        SLA_mode = SLA_SYS_ERROR;
      }
      vTaskDelay(pdMS_TO_TICKS(2000));
    }

    if(TaskDaemon_error_number > TASK_ERROR_NUMBER_MAX){
      // 如果错误次数超限制，则重启系统
      Serial.println("[TaskDaemon]The number of times the task has gone offline has exceeded the preset threshold. The system will now initiate a restart.");
      vTaskDelay(pdMS_TO_TICKS(2000));
      ESP.restart();
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }

    #if DEBUG_MODE == 1
      Serial.println("[TaskDaemon]Ending TaskDaemon_Task");
    #endif
    vTaskDelete(NULL);
}



/* FreeRTOS任务守护进程任务初始化 */
void run_TaskDaemon_Task_on_Core_1(){
  xTaskCreatePinnedToCore(
                    FreeRTOS_Task_Daemon,   /* Task function. */
                    "TaskDaemon",     /* name of task. */
                    2048,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    5,           /* priority of the task */
                    &TaskDaemon_Task_TaskHandle,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */                  
}