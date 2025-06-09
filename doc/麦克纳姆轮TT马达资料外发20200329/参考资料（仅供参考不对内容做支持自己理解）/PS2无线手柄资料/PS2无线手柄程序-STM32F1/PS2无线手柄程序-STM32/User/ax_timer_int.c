/**
  ******************************************************************************
  * @file    ax_timer_int.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   X-CTR100 ��ʱ����������ж��ļ�
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  * 
  ******************************************************************************
  */  

#include "ax_timer_int.h" 

/**
  * @��  ��  TIM6��ʱ����ʼ��������жϣ�
  * @��  ��  cnt_us �����������ֵ����λ1us����Χ0-65535 
  * @����ֵ  ��
  */
void AX_TIMER_INT_TIM6_Init(uint16_t cnt_us)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); //ʹ��ʱ��
	
	 // �ۼ� TIM_Period�������һ�����»����ж�
	TIM_TimeBaseInitStructure.TIM_Period = cnt_us-1;  //�Զ���װ��ֵ�����65535
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=1000000Hz
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1; //��ʱ����Ƶ,�������ڣ�period x 1us��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure); //��ʼ����ʱ��
	
	//�����ʱ�������жϱ�־λ
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	//������ʱ�������ж�
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ�� 
	
	// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn; 	
	// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @��  ��  TIM6 �жϴ�����
  * @��  ��  ��
  * @����ֵ  ��
  */
void  TIM6_DAC_IRQHandler(void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		//�жϴ�������
		AX_LEDG_Toggle();
				
		TIM_ClearITPendingBit(TIM6 , TIM_IT_Update);  		 
	}		 	
}
/******************* (C) ��Ȩ 2018 XTARK **************************************/
