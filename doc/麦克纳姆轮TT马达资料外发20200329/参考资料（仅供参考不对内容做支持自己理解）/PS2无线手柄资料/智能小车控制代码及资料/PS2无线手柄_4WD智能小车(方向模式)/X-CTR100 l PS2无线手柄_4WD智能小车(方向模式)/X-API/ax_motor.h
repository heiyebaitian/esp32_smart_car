/**
  ******************************************************************************
  * @file    ax_motor.h
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100板载电机控制接口函数头文件
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  *
  ******************************************************************************
  */
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_MOTOR_H
#define __AX_MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*** X-CTR100 电机控制操作函数 **********/
void AX_MOTOR_TIM1_MNOP_Init(uint8_t freq_khz); //电机MNOP接口初始化
void AX_MOTOR_TIM1_M_SetSpeed(int16_t speed);   //电机M控制
void AX_MOTOR_TIM1_N_SetSpeed(int16_t speed);   //电机N控制
void AX_MOTOR_TIM1_O_SetSpeed(int16_t speed);   //电机O控制
void AX_MOTOR_TIM1_P_SetSpeed(int16_t speed);   //电机P控制

#endif

/******************* (C) 版权 2018 XTARK **************************************/
