/**
  ******************************************************************************
  * @file    ax_motor.h
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100���ص�����ƽӿں���ͷ�ļ�
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  *
  ******************************************************************************
  */
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_MOTOR_H
#define __AX_MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*** X-CTR100 ������Ʋ������� **********/
void AX_MOTOR_TIM1_MNOP_Init(uint8_t freq_khz); //���MNOP�ӿڳ�ʼ��
void AX_MOTOR_TIM1_M_SetSpeed(int16_t speed);   //���M����
void AX_MOTOR_TIM1_N_SetSpeed(int16_t speed);   //���N����
void AX_MOTOR_TIM1_O_SetSpeed(int16_t speed);   //���O����
void AX_MOTOR_TIM1_P_SetSpeed(int16_t speed);   //���P����

#endif

/******************* (C) ��Ȩ 2018 XTARK **************************************/
