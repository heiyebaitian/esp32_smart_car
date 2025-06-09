#ifndef __CAS__H
#define __CAS__H
#pragma once

#include "SonoLuminAlert.h"

typedef enum
{
      CAS_TRIGGERED = 0, CAS_READY, CAS_TEMPORARY_RELEASE
} CAS_STATE;

extern CAS_STATE CAS_flag;
extern SLA_MODE SLA_mode;

int CAS_Init();
void CAS_Task(void * pvParameters);
void run_CAS_Task_on_Core_0();


#endif