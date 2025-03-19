#ifndef __TASKUTILS__H
#define __TASKUTILS__H
#pragma once

extern TaskHandle_t MK_Task_TaskHandle; // MK_Task任务句柄
extern TaskHandle_t CAS_Task_TaskHandle; // CAS_Task任务句柄

void FreeRTOS_Task_Daemon();
void run_TaskDaemon_Task_on_Core_1();


#endif