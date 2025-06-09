/**
  ******************************************************************************
  * @file    main.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   Main program body
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  *
  ******************************************************************************
  */   

#include "stm32f4xx.h"
#include <stdio.h>

/* X-APIͷ�ļ� */
#include "ax_basis.h"  //X-CTR100����������Դ
#include "ax_ps2.h"  //PS2 ����������Ϸ�ֱ�
#include "ax_motor.h"   //ֱ�����

JOYSTICK_TypeDef my_joystick;  //�ֱ���ֵ�ṹ��

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	//X-CTR100��ʼ��
	AX_Init(115200);   
	printf("***X-CTR100 PS2����4WD����С������***\r\n\r\n");	
	
    //��������
	AX_MOTOR_TIM1_MNOP_Init(20); //��ʼ��������ƶ˿ڣ�PWMƵ��Ϊ20KHz		
	AX_PS2_Init();	//PS2��ʼ��
    AX_Delayms(10);

	while (1)
	{
		AX_PS2_ScanKey(&my_joystick);
		
		//LR�������Ʒ�ʽ(����ģʽ)
		if((my_joystick.btn1|0xEF) == 0xEF) //ǰ
		{
			AX_MOTOR_TIM1_O_SetSpeed(60); 
			AX_MOTOR_TIM1_P_SetSpeed(60); 
		}
		else if((my_joystick.btn1|0xBF) == 0xBF) //��
		{
			AX_MOTOR_TIM1_O_SetSpeed(-60); 
			AX_MOTOR_TIM1_P_SetSpeed(-60);
		}
		else if((my_joystick.btn1|0xDF) == 0xDF) //��
		{
			AX_MOTOR_TIM1_O_SetSpeed(-60); 
			AX_MOTOR_TIM1_P_SetSpeed(60); 
		}
		else if((my_joystick.btn1|0x7F) == 0x7F) //��
		{
			AX_MOTOR_TIM1_O_SetSpeed(60); 
			AX_MOTOR_TIM1_P_SetSpeed(-60); 			
		}
		else  //ֹͣ
		{
			AX_MOTOR_TIM1_O_SetSpeed(0); 
			AX_MOTOR_TIM1_P_SetSpeed(0); 	
		}
		
		printf("MODE:%2x BTN1:%2x BTN2:%2x RJOY_LR:%2x RJOY_UD:%2x LJOY_LR:%2x LJOY_UD:%2x\r\n",
		my_joystick.mode, my_joystick.btn1, my_joystick.btn2, 
		my_joystick.RJoy_LR, my_joystick.RJoy_UD, my_joystick.LJoy_LR, my_joystick.LJoy_UD);	

		AX_Delayms(30);		
	}
}

/******************* (C) ��Ȩ 2018 XTARK **************************************/
