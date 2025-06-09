
#ifndef __ESPNOT__H
#define __ESPNOT__H
#pragma once

#include "CAS.h"
#include "MK_encoder_motor_driver.h"
#include "SonoLuminAlert.h"



extern TaskHandle_t CAS_Task_TaskHandle; // CAS_Task任务句柄

extern const uint8_t channel;
extern CAS_STATE CAS_flag;
extern MK_STATE MK_flag;
extern SLA_MODE SLA_mode;

extern uint32_t motor0_L_TargetSpeed;
extern uint32_t motor1_L_TargetSpeed;
extern uint32_t motor2_L_TargetSpeed;
extern uint32_t motor3_L_TargetSpeed;
extern uint32_t motor0_R_TargetSpeed;
extern uint32_t motor1_R_TargetSpeed;
extern uint32_t motor2_R_TargetSpeed;
extern uint32_t motor3_R_TargetSpeed;


void resetData();
void OnDataRecv(const esp_now_recv_info_t* mac, const uint8_t *incomingData, int len);
void InitESPNow();


#endif