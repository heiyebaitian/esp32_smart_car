
#ifndef __ESPNOT__H
#define __ESPNOT__H
#pragma once

#define DEBUG_MODE 1

extern const uint8_t channel;

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