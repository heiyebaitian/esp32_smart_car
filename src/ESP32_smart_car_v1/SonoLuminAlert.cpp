#include <Arduino.h>

#include "SonoLuminAlert.h"

const uint8_t tonepin = 26;  // 设置PWM输出引脚

/* 开机提示音 */
const uint32_t power_on_tone_tune[]={
  C5,D5,G5
};
const uint32_t power_on_tone_time[]={
  200,200,500
};

uint32_t length = 0;  //  乐谱长度

TaskHandle_t SonoLuminAlert_Task_TaskHandle; // SonoLuminAlert_Task任务句柄


/* 播放开机提示音 */
void power_on_tone_play(){
  length=sizeof(power_on_tone_tune)/sizeof(power_on_tone_tune[0]);
  for(int x=0;x<length;x++)
  {
    tone(tonepin,power_on_tone_tune[x]);
    delay(power_on_tone_time[x]);   
    noTone(tonepin);
  }
}

/* 声光报警任务 */
void SonoLuminAlert_Task(void * pvParameters){
  pinMode(tonepin,OUTPUT);  //  初始化蜂鸣器引脚
  

  while(1){
    /* 播放开机提示音 */
    if(Speaker_mode == SPEAKER_POWER_ON_TONE){
      power_on_tone_play();
      Speaker_mode = SPEAKER_READY;
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }

  #if DEBUG_MODE == 1
      Serial.println("[SonoLuminAlert_Task]Ending SonoLuminAlert_Task");
  #endif
  vTaskDelete(NULL);
}

/* 声光报警任务初始化 */
void run_SonoLuminAlert_Task_on_Core_1(){
  xTaskCreatePinnedToCore(
                    SonoLuminAlert_Task,   /* Task function. */
                    "SonoLuminAlert_Task",     /* name of task. */
                    2048,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    3,           /* priority of the task */
                    &SonoLuminAlert_Task_TaskHandle,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */                  
}
