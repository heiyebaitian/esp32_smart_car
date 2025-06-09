/**
	 __  _______  _    ____  _  __      __  __    ____   ___  _____ _____ 
	 \ \/ /_   _|/ \  |  _ \| |/ / 塔   \ \/ /   / ___| / _ \|  ___|_   _| 软
	  \  /  | | / _ \ | |_) | ' /  克    \  /____\___ \| | | | |_    | |   件
	  /  \  | |/ ___ \|  _ <| . \  创    /  \_____|__) | |_| |  _|   | |   生
	 /_/\_\ |_/_/   \_\_| \_\_|\_\ 新   /_/\_\   |____/ \___/|_|     |_|   态
                              
  ****************************************************************************** 
  *           
  * 版权所有： XTARK@塔克创新  版权所有，盗版必究
  * 官网网站： www.xtark.cn
  * 淘宝店铺： https://shop246676508.taobao.com  
  * 塔克媒体： www.cnblogs.com/xtark（博客）
	* 塔克微信： 微信公众号：塔克创新（获取最新资讯）
  *      
  ******************************************************************************
  * @作  者  Musk Han@XTARK
  * @版  本  V1.0
  * @日  期  2019-8-8
  * @内  容  PS2手柄驱动
  *
  ******************************************************************************
  * @说  明
  *  
  *   PS2数据定义
  *   BYTE   DATA   解释
  *   01     idle
  *   02     0x73   手柄的工作模式
  *   03     0x5A   Bit0  Bit1  Bit2  Bit3  Bit4  Bit5  Bit6  Bit7
  *   04     data   SLCT  JOYR  JOYL  STRT   UP   RGIHT  DOWN   L
  *   05     data   L2     R2     L1    R1   /\     O     X    口
  *   06     data   右边摇杆  0x00 = 左    0xff = 右
  *   07     data   右边摇杆  0x00 = 上    0xff = 下
  *   08     data   左边摇杆  0x00 = 左    0xff = 右
  *   09     data   左边摇杆  0x00 = 上    0xff = 下
  * 
  ******************************************************************************
  */
#include "ax_ps2.h"
#include "ax_sys.h"
#include "ax_delay.h"

//PS手柄的输入输出口
#define PS2_READ_DATA()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)
#define PS2_CMD_H()      GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define PS2_CMD_L()      GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define PS2_ATT_H()      GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define PS2_ATT_L()      GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define PS2_CLK_H()      GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define PS2_CLK_L()      GPIO_ResetBits(GPIOB, GPIO_Pin_12)

/**
  * @简  述  PS2 手柄初始化
  * @参  数  无
  * @返回值  无
  * @说  明  配置端口
  */
void AX_PS2_Init(void)
{ 	 											  
	GPIO_InitTypeDef GPIO_InitStructure;         
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

//向手柄发送命令
static uint8_t PS2_SendCmd(uint8_t cmd)
{
	volatile uint16_t ref=0x01;
	uint8_t temp = 0;
	
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&cmd)
			PS2_CMD_H();                   //输出以为控制位
		else 
			PS2_CMD_L();

		PS2_CLK_H();                        //时钟拉高
		AX_Delayus(16);
		PS2_CLK_L();
		AX_Delayus(16);
		PS2_CLK_H();
		
		if(PS2_READ_DATA())
			temp = ref|temp;
	}
	
	return temp;
}
/**
  * @简  述  PS2 获取按键及摇杆数值。
  * @参  数  *data 手柄数据
  * @返回值  无
  */
void AX_PS2_GetKeyData(uint8_t *data)
{
	volatile uint16_t ref=0x01;
	uint8_t i;
	
	//清数据
	for(i=0;i<9;i++) 
		data[i] = 0;
	
	PS2_ATT_L();
	
	PS2_SendCmd(0x01);   //开始命令
	data[1] = PS2_SendCmd(0x42);  //请求数据
	
	for(i=2;i<9;i++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			PS2_CLK_H();
			PS2_CLK_L();
			AX_Delayus(16);
			PS2_CLK_H();
		  if(PS2_READ_DATA())
				
				data[i] = ref|data[i];
		}
    AX_Delayus(16);
	}
	
	PS2_ATT_H();
}

/******************* (C) 版权 2019 XTARK **************************************/
