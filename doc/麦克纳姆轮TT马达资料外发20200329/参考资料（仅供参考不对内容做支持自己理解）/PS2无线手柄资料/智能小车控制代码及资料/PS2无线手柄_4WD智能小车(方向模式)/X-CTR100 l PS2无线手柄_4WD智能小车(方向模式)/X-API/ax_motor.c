/**
  ******************************************************************************
  * @file    ax_motor.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   X-CTR100板载电机控制接口函数文件
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  * 
  ******************************************************************************
  */  

#include "ax_motor.h" 
#include "ax_basis.h"
#include <stdio.h>
/**
  * @简  述  电机MNOP接口初始化	
  * @参  数  freq_khz:PWM输出频率，范围1~100,单位KHz
  * @返回值  无
  */
void AX_MOTOR_TIM1_MNOP_Init(uint8_t freq_khz)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure; 
	
	//GPIO功能时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		
	//TIM1 时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	

	//复用功能配置
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 
	
	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /*** 电机方向控制IO配置 ***/	
	
	//IO时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |  
	                       RCC_AHB1Periph_GPIOE, ENABLE);

    //电机M方向控制IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

    //电机N方向控制IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

    //电机O方向控制IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

    //电机P方向控制IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//参数过滤
	if(freq_khz>100)
		freq_khz = 100;

	//定时器配置  ARR:200-1;PSC:根据PWM频率设置
	TIM_TimeBaseStructure.TIM_Prescaler=420/freq_khz-1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=200-1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //PWM1 Mode configuration: Channel1 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;	    //占空比初始化
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    //PWM1 Mode configuration: Channel2
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    //PWM1 Mode configuration: Channel3
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    //PWM1 Mode configuration: Channel4
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    //使能定时器
    TIM_Cmd(TIM1, ENABLE);   
	
	//使能MOE位
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	//混乱的MDK
	freq_khz = 0;
	TIM_SetCompare4(TIM1,freq_khz);
	printf("*MDK BUG %d \r\n",freq_khz);
}

/**
  * @简述   电机M控制
  * @参数   speed 电机转速数值，范围-200~200
  * @返回值 无
  */
void AX_MOTOR_TIM1_M_SetSpeed(int16_t speed)
{
	uint16_t temp;

    if(speed > 0)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	    GPIO_SetBits(GPIOA, GPIO_Pin_3);
		temp = speed;	
	}
	else if(speed < 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	    GPIO_SetBits(GPIOB, GPIO_Pin_9);
		temp = (-speed);	
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		temp = 0;
	}
	
	if(temp>200)
		temp = 200;
	
	TIM_SetCompare4(TIM1,temp);
}

/**
  * @简述   电机N控制
  * @参数   speed 电机转速数值，范围-200~200
  * @返回值 无
  */
void AX_MOTOR_TIM1_N_SetSpeed(int16_t speed)
{
	uint16_t temp;
	
    if(speed > 0)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	    GPIO_SetBits(GPIOE, GPIO_Pin_0);
		temp = speed;
		
	}
	else if(speed < 0)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	    GPIO_SetBits(GPIOE, GPIO_Pin_1);
		temp = (-speed);	
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	    GPIO_ResetBits(GPIOE, GPIO_Pin_0);
		temp = 0;
	}
	
	if(temp>200)
		temp = 200;
	
	TIM_SetCompare3(TIM1,temp);
}
/**
  * @简述   电机O控制
  * @参数   speed 电机转速数值，范围-200~200
  * @返回值 无
  */
void AX_MOTOR_TIM1_O_SetSpeed(int16_t speed)
{
	uint16_t temp = 0;

    if(speed > 0)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
	    GPIO_SetBits(GPIOE, GPIO_Pin_2);
		temp = speed;	  
	}
	else if(speed < 0)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_2);
	    GPIO_SetBits(GPIOE, GPIO_Pin_3);
		temp = (-speed);	
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
	    GPIO_ResetBits(GPIOE, GPIO_Pin_2);	
		temp = 0;
	}
	
	if(temp>200)
		temp = 200;
	
	TIM_SetCompare2(TIM1,temp);
}

/**
  * @简述   电机P控制
  * @参数   speed 电机转速数值，范围-200~200
  * @返回值 无
  */
void AX_MOTOR_TIM1_P_SetSpeed(int16_t speed)
{
	uint16_t temp;

    if(speed > 0)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	    GPIO_SetBits(GPIOE, GPIO_Pin_4);
			temp = speed;	  
	}
	else if(speed < 0)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_4);
	    GPIO_SetBits(GPIOE, GPIO_Pin_5);
			temp = (-speed);	
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	    GPIO_ResetBits(GPIOE, GPIO_Pin_4);	
		temp = 0;
	}

	if(temp>200)
		temp = 200;
	
	TIM_SetCompare1(TIM1,temp);
}

/******************* (C) 版权 2018 XTARK **************************************/
