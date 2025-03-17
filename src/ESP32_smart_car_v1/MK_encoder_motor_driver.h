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
/* 左摇杆停止 */
void MK_L_StopSpeed(void);
/* 左摇杆运动 */
void MK_L_RunSpeed(double groupA_percentage, double groupB_percentage);
/* 右摇杆停止 */
void MK_R_StopSpeed(void);
/* 右摇杆前置圆心旋转函数 */
void MK_R_rotateFrontAroundCenter(double percentage);
/* 右摇杆中置圆心旋转函数 */
void MK_R_rotateThroughCenter(double percentage);
/* 右摇杆后置圆心逆时针旋转函数 */
void MK_R_rotateBackAroundCenter(double percentage);  // 注意：原函数缺少参数类型声明

void MK_Task(void * pvParameters);
void run_MK_Task_on_Core_0();
#endif