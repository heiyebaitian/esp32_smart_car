#ifndef __MK_ENCODE_MOTOR_DRIVER__H
#define __MK_ENCODE_MOTOR_DRIVER__H
#pragma once

extern uint32_t motor0_L_TargetSpeed;
extern uint32_t motor1_L_TargetSpeed;
extern uint32_t motor2_L_TargetSpeed;
extern uint32_t motor3_L_TargetSpeed;
extern uint32_t motor0_R_TargetSpeed;
extern uint32_t motor1_R_TargetSpeed;
extern uint32_t motor2_R_TargetSpeed;
extern uint32_t motor3_R_TargetSpeed;

void MK_Init();
void MK_run();
void MK_stop();
void MK_L_StopSpeed(void);
void MK_L_RunSpeed(double groupA_percentage, double groupB_percentage);
void MK_R_StopSpeed(void);
void MK_R_rotateFrontAroundCenter(double percentage);
void MK_R_rotateThroughCenter(double percentage);
void MK_R_rotateBackAroundCenter(double percentage);
void MK_Task(void * pvParameters);
void run_MK_Task_on_Core_0();

#endif