/**
  ******************************************************************************
  * @file    main.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   Main program body
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  *
  ******************************************************************************
  */   

#include "stm32f4xx.h"
#include <stdio.h>

/* X-API头文件 */
#include "ax_basis.h"  //X-CTR100基础板载资源
#include "ax_ps2.h"  //PS2 索尼无线游戏手柄
#include "ax_motor.h"   //直流电机

JOYSTICK_TypeDef my_joystick;  //手柄键值结构体

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	//X-CTR100初始化
	AX_Init(115200);   
	printf("***X-CTR100 PS2控制4WD智能小车例程***\r\n\r\n");	
	
    //功能配置
	AX_MOTOR_TIM1_MNOP_Init(20); //初始化电机控制端口，PWM频率为20KHz		
	AX_PS2_Init();	//PS2初始化
    AX_Delayms(10);

	while (1)
	{
		AX_PS2_ScanKey(&my_joystick);
		
		//LR按键控制方式(塔克模式)
		if((my_joystick.btn1|0xEF) == 0xEF) //前
		{
			AX_MOTOR_TIM1_O_SetSpeed(60); 
			AX_MOTOR_TIM1_P_SetSpeed(60); 
		}
		else if((my_joystick.btn1|0xBF) == 0xBF) //后
		{
			AX_MOTOR_TIM1_O_SetSpeed(-60); 
			AX_MOTOR_TIM1_P_SetSpeed(-60);
		}
		else if((my_joystick.btn1|0xDF) == 0xDF) //右
		{
			AX_MOTOR_TIM1_O_SetSpeed(-60); 
			AX_MOTOR_TIM1_P_SetSpeed(60); 
		}
		else if((my_joystick.btn1|0x7F) == 0x7F) //左
		{
			AX_MOTOR_TIM1_O_SetSpeed(60); 
			AX_MOTOR_TIM1_P_SetSpeed(-60); 			
		}
		else  //停止
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

/******************* (C) 版权 2018 XTARK **************************************/
