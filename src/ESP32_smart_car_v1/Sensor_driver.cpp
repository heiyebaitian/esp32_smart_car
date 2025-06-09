#include <Arduino.h>
#include "Sensor_driver.h"

const uint8_t MQ2_pin = 35;  // MQ2传感器引脚
const uint8_t Fire_pin = 36;  // 火焰传感器引脚

uint8_t MQ2_SLA_NUMBER = 200; // MQ2报警阈值
uint8_t FIRE_SLA_NUMBER = 200; // 火焰传感器报警阈值

TaskHandle_t Sensor_driver_Task_TaskHandle; // Sensor_driver_Task任务句柄

/* 传感器任务 */
void Sensor_driver_Task(void * pvParameters){
  pinMode(MQ2_pin, INPUT);
  pinMode(Fire_pin, INPUT);

  while(1){
    
    mq2_Value = analogRead(MQ2_pin);
    fire_sensor_Value = analogRead(Fire_pin);
    // 如果MQ2或火焰传感器达到阈值则触发报警
    if(mq2_Value >= MQ2_SLA_NUMBER || fire_sensor_Value >= FIRE_SLA_NUMBER){
      if(SLA_mode < SLA_FIRE){
        SLA_mode = SLA_FIRE;
      }
    }
    
    vTaskDelay(pdMS_TO_TICKS(2000)); 
  }

  #if DEBUG_MODE == 1
      Serial.println("[Sensor_driver_Task]Ending Sensor_driver_Task");
  #endif
  vTaskDelete(NULL);
}


/* 传感器任务初始化 */
void run_Sensor_driver_Task_on_Core_1(){
  xTaskCreatePinnedToCore(
                    Sensor_driver_Task,   /* Task function. */
                    "Sensor_driver_Task",     /* name of task. */
                    2048,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    3,           /* priority of the task */
                    &Sensor_driver_Task_TaskHandle,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */                  
}