/**
  ******************************************************************************
  * @file    ax_basis.c
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100板载资源基础操作函数文件
  *          LED、BEEP、编码开关、滴答定时、UART1
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  * 
  ******************************************************************************
  */   

#include "ax_basis.h"
#include <stdio.h>

/**
  * @简  述  X-CTR100初始化
  * @参  数  baud：串口波特率(例如，9600,115200)
  * @返回值  无
  * @说  明  包含LED、BEEP、编码开关、SysTick、UART1
  */
void AX_Init(uint32_t baud) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	/* LED BEEP 编码开关 GPIO配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                       	   RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE); //启动GPIO时钟 
	//配置红色LED指示灯GPIO  推挽输出模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,  GPIO_Pin_10);	

	//配置绿色LED指示灯GPIO  推挽输出模式  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,  GPIO_Pin_11);

	//配置蜂鸣器GPIO  推挽输出模式	 
	GPIO_SetBits(GPIOE,  GPIO_Pin_6);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//配置模式选择S1 GPIO  输入模式
	GPIO_SetBits(GPIOE,  GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//配置模式选择S2 GPIO  输入模式 
	GPIO_SetBits(GPIOE,  GPIO_Pin_10);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* 延时函数 SysTick 配置 */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	//时钟固定为AHB时钟的1/8

	/* 串口 USART1配置 */
	//打开GPIO和USART部件的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//USART1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

	//USART1 端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	//USART1参数配置
	USART_InitStructure.USART_BaudRate = baud;    //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	//使能 USART， 配置完毕
	USART_Cmd(USART1, ENABLE); 
	
	//规避第一个字符不能输出的BUG
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

/**
  * @简  述  SW获得选择状态
  * @说  明	 开关拨动到ON侧为1，对侧为0。
  * @参  数  无	  
  * @返回值  编码状态
  *			SW1(PA8)  SW2(PE10)    返回值
  *			 0         0             0  
  *          0         1             1  
  *          1         0             2
  *          1         1             3   
  */
uint8_t AX_SW_GetSW12Status(void)
{
    uint8_t sw = 0;

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)) 	
	{sw <<= 1;}
	else
	{sw=sw+1; sw<<=1;}

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10))
	{;}
	else
	{sw=sw+1;}

	return sw;
}

/**
  * @简  述  SW1获得选择状态
  * @参  数  无	  
  * @返回值  编码开关状态
  *			 1 - 开关拨动到ON侧
  *			 0 - 开关拨动到ON对侧
  */
uint8_t AX_SW_GetSW1Status(void)
{
   	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15))
	return 0;
	else
	return 1;
}

/**
  * @简  述  SW2获得选择状态
  * @参  数  无	  
  * @返回值  编码开关状态
  *			 1 - 开关拨动到ON侧
  *			 0 - 开关拨动到ON对侧
  */
uint8_t AX_SW_GetSW2Status(void)
{
   	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10))
	return 0;
	else
	return 1;
}

/**
  * @简  述  软件微妙延时
  * @参  数  us：延时长度，单位us	  
  * @返回值  无
  */
void AX_Delayus(uint16_t us)
{
	uint32_t temp;
	
	SysTick->LOAD=21*us; 				 		 
	SysTick->VAL=0x00;        				
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  	 
	
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));	 
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 
	SysTick->VAL =0X00;       				
}

/**
  * @简  述  软件毫妙级延时函数。
  * @参  数  ms：延时长度，单位ms	  	  
  * @返回值  无
  * @说  明  注意ms的范围，SysTick->LOAD为24位寄存器,所以,最大延时为:nTime<=0xffffff*8*1000/SYSCLK
  *          对168M条件下,ms<=798ms 
  */
static void Delay_ms(uint16_t ms)
{	 		  	  
	uint32_t temp;	
	
	SysTick->LOAD=(uint32_t)21000*ms;			
	SysTick->VAL =0x00;           		
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 
	
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      
	SysTick->VAL =0X00;     		  		 	    
}

/**
  * @简  述  软件毫秒延时
  * @参  数  ms：延时长度，单位ms	  	 	  
  * @返回值  无
  */
void AX_Delayms(uint16_t ms)
{
	uint8_t repeat=ms/500;																
	uint16_t remain=ms%500;
	
	while(repeat)
	{
		Delay_ms(500);
		repeat--;
	}
	
	if(remain)
	{
		Delay_ms(remain);
	}
		
}

/*** 串口打印相关函数重定义 ***********/
/**
  * @简  述  重定义putc函数（USART1）	
  */
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/**
  * @简  述  重定义getc函数（USART1）	
  */
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{}

	return (int)USART_ReceiveData(USART1);
}

/******************* (C) 版权 2018 XTARK **************************************/
