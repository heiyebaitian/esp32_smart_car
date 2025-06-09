#include <Arduino.h>

#include "SonoLuminAlert.h"

const uint8_t tonepin = 26;  // 蜂鸣器PWM输出引脚
const uint8_t light_R_pin = 12; // 灯光红色引脚
const uint8_t light_G_pin = 14; // 灯光绿色引脚
const uint8_t light_B_pin = 15; // 灯光蓝色引脚

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

void light_control(LIGHT_MODE Light_mode){
  switch(Light_mode){
    case LIGHT_OFF : digitalWrite(light_R_pin,LOW); digitalWrite(light_G_pin,LOW); digitalWrite(light_B_pin,LOW); break;
    case LIGHT_R : digitalWrite(light_R_pin,HIGH); digitalWrite(light_G_pin,LOW); digitalWrite(light_B_pin,LOW); break;
    case LIGHT_G : digitalWrite(light_R_pin,LOW); digitalWrite(light_G_pin,HIGH); digitalWrite(light_B_pin,LOW); break;
    case LIGHT_B : digitalWrite(light_R_pin,LOW); digitalWrite(light_G_pin,LOW); digitalWrite(light_B_pin,HIGH); break;
    case LIGHT_Y : digitalWrite(light_R_pin,HIGH); digitalWrite(light_G_pin,HIGH); digitalWrite(light_B_pin,LOW); break;
    case LIGHT_W : digitalWrite(light_R_pin,HIGH); digitalWrite(light_G_pin,HIGH); digitalWrite(light_B_pin,HIGH); break;
  }
}

/* 声光报警任务 */
void SonoLuminAlert_Task(void * pvParameters){
  pinMode(tonepin,OUTPUT);  //  初始化蜂鸣器引脚
  pinMode(light_R_pin,OUTPUT);  //  初始化灯光引脚
  pinMode(light_G_pin,OUTPUT);
  pinMode(light_B_pin,OUTPUT);
  

  while(1){
    /* 播放开机提示音 */
    if(SLA_mode == SLA_BOOT_OK){
      light_control(LIGHT_G);
      power_on_tone_play();
      SLA_mode = SLA_READY; // 降级到就绪态
    }

    /* 如果防碰撞被触发 */
    if(SLA_mode == SLA_CAS_TRIGGERED){
      light_control(LIGHT_B);
      /* 持续报警 */
      while(SLA_mode == SLA_CAS_TRIGGERED){
        tone(tonepin,C5);
        delay(100);   
        noTone(tonepin);
        vTaskDelay(pdMS_TO_TICKS(500)); 
      }
    }

    /* 如果火警被触发 */
    if(SLA_mode == SLA_FIRE){
      light_control(LIGHT_Y);
      while(SLA_mode == SLA_FIRE){
        tone(tonepin,C6);
        delay(1000);   
        noTone(tonepin);
        vTaskDelay(pdMS_TO_TICKS(2000)); 
      }
    }

    /* 如果无法修正的系统错误被触发 */
    if(SLA_mode == SLA_SYS_ERROR){
      light_control(LIGHT_R);
      tone(tonepin,C6);
      delay(1000);   
      noTone(tonepin);
      while(SLA_mode == SLA_SYS_ERROR){
        vTaskDelay(pdMS_TO_TICKS(5000)); 
      }
    }



    vTaskDelay(pdMS_TO_TICKS(2000)); 
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
