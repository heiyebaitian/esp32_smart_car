/**
  ******************************************************************************
  * @file    ax_timer_int.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   X-CTR100 定时器计数溢出中断文件
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  * 
  ******************************************************************************
  */  

#include "ax_timer_int.h" 

/**
  * @简  述  TIM6定时器初始化（溢出中断）
  * @参  数  cnt_us 设置溢出计数值，单位1us，范围0-65535 
  * @返回值  无
  */
void AX_TIMER_INT_TIM6_Init(uint16_t cnt_us)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); //使能时钟
	
	 // 累计 TIM_Period个后产生一个更新或者中断
	TIM_TimeBaseInitStructure.TIM_Period = cnt_us-1;  //自动重装载值，最大65535
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=1000000Hz
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1; //定时器分频,计数周期（period x 1us）
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure); //初始化定时器
	
	//清除定时器更新中断标志位
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	//开启定时器更新中断
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM6,ENABLE); //使能定时器 
	
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn; 	
	// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @简  述  TIM6 中断处理函数
  * @参  数  无
  * @返回值  无
  */
void  TIM6_DAC_IRQHandler(void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		//中断处理内容
		AX_LEDG_Toggle();
				
		TIM_ClearITPendingBit(TIM6 , TIM_IT_Update);  		 
	}		 	
}
/******************* (C) 版权 2018 XTARK **************************************/
