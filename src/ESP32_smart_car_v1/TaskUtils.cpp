#include <Arduino.h>

#include "TaskUtils.h"
#include "MK_encoder_motor_driver.h"
#include "joystick_analysis.h"
#include "CAS.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

uint32_t TaskDaemon_error_number = 0;

TaskHandle_t TaskDaemon_Task_TaskHandle; // TaskDaemon_Task任务句柄

/* FreeRTOS任务守护进程 */
void FreeRTOS_Task_Daemon(void * pvParameters){
  #if DEBUG_MODE == 1
    Serial.print("[TaskDaemon]TaskDaemon_Task running on core ");
    Serial.println(xPortGetCoreID());
  #endif

  //esp_task_wdt_add(NULL);  // 添加当前任务到看门狗监控列表

  while(1){
    if(eTaskGetState(MK_Task_TaskHandle) == eDeleted){
      // 检查MK底盘驱动任务是否离线
      #if DEBUG_MODE == 1
        Serial.println("[TaskDaemon]MK_Task is has been deleted,Trying to restart the task.");
      #endif
      run_MK_Task_on_Core_0();
      TaskDaemon_error_number++;
    }
    if(eTaskGetState(CAS_Task_TaskHandle) == eDeleted){
      // 检查CAS防碰撞系统任务是否离线
      #if DEBUG_MODE == 1
        Serial.println("[TaskDaemon]CAS_Task is has been deleted,Trying to restart the task.");
      #endif
      run_CAS_Task_on_Core_0();
      TaskDaemon_error_number++;
    }

    if(TaskDaemon_error_number > 10){
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
                    4096,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    3,           /* priority of the task */
                    &TaskDaemon_Task_TaskHandle,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */                  
}