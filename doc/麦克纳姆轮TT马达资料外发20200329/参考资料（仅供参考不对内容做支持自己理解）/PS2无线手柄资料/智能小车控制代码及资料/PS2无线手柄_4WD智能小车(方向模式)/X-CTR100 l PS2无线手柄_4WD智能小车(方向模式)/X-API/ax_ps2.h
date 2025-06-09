/**
  ******************************************************************************
  * @file    ax_ps2.h
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100 PS2�����ֱ�����ͷ�ļ�
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  *
  ******************************************************************************
  */
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_PS2_H
#define __AX_PS2_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "ax_basis.h"

//�ֱ���ֵ���ݽṹ��	 
typedef struct			 				
{
  uint8_t mode;							 /* �ֱ��Ĺ���ģʽ */

  uint8_t btn1;                      /* Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7 
                                           SLCT           STRT UP   R    DOWN L   */

  uint8_t btn2;                      /* Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7 
                                           L2   R2   L1   R1   /\   O    X    ��   */

  uint8_t RJoy_LR;                       /*�ұ�ҡ��  0x00 = ��    0xff = ��   */

  uint8_t RJoy_UD;                       /*�ұ�ҡ��  0x00 = ��    0xff = ��   */

  uint8_t LJoy_LR;                       /*�ұ�ҡ��  0x00 = ��    0xff = ��   */

  uint8_t LJoy_UD;                       /*�ұ�ҡ��  0x00 = ��    0xff = ��   */
	
}JOYSTICK_TypeDef;

/*** X-CTR100 PS2���������ֱ��������� **********/
void AX_PS2_Init(void);  //PS2��ʼ��
void AX_PS2_ScanKey(JOYSTICK_TypeDef* JoystickStruct);//PS2��ȡ������ҡ����ֵ

#endif

/******************* (C) ��Ȩ 2018 XTARK **************************************/
