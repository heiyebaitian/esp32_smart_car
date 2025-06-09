/**
  ******************************************************************************
  * @file    ax_motor.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   X-CTR100���ص�����ƽӿں����ļ�
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  * 
  ******************************************************************************
  */  

#include "ax_motor.h" 
#include "ax_basis.h"
#include <stdio.h>
/**
  * @��  ��  ���MNOP�ӿڳ�ʼ��	
  * @��  ��  freq_khz:PWM���Ƶ�ʣ���Χ1~100,��λKHz
  * @����ֵ  ��
  */
void AX_MOTOR_TIM1_MNOP_Init(uint8_t freq_khz)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure; 
	
	//GPIO����ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		
	//TIM1 ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	

	//���ù�������
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 
	
	//����IO��Ϊ���ù���-��ʱ��ͨ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /*** ����������IO���� ***/	
	
	//IOʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |  
	                       RCC_AHB1Periph_GPIOE, ENABLE);

    //���M�������IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

    //���N�������IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

    //���O�������IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

    //���P�������IO
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//��������
	if(freq_khz>100)
		freq_khz = 100;

	//��ʱ������  ARR:200-1;PSC:����PWMƵ������
	TIM_TimeBaseStructure.TIM_Prescaler=420/freq_khz-1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=200-1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //PWM1 Mode configuration: Channel1 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;	    //ռ�ձȳ�ʼ��
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

    //ʹ�ܶ�ʱ��
    TIM_Cmd(TIM1, ENABLE);   
	
	//ʹ��MOEλ
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	//���ҵ�MDK
	freq_khz = 0;
	TIM_SetCompare4(TIM1,freq_khz);
	printf("*MDK BUG %d \r\n",freq_khz);
}

/**
  * @����   ���M����
  * @����   speed ���ת����ֵ����Χ-200~200
  * @����ֵ ��
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
  * @����   ���N����
  * @����   speed ���ת����ֵ����Χ-200~200
  * @����ֵ ��
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
  * @����   ���O����
  * @����   speed ���ת����ֵ����Χ-200~200
  * @����ֵ ��
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
  * @����   ���P����
  * @����   speed ���ת����ֵ����Χ-200~200
  * @����ֵ ��
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

/******************* (C) ��Ȩ 2018 XTARK **************************************/
