#ifndef __MK_ENCODE_MOTOR_DRIVER__H
#define __MK_ENCODE_MOTOR_DRIVER__H
#pragma once

void MK_Init();
void MK_Stop();
void MK_RunSpeed(double groupA_percentage, double groupB_percentage);

#endif