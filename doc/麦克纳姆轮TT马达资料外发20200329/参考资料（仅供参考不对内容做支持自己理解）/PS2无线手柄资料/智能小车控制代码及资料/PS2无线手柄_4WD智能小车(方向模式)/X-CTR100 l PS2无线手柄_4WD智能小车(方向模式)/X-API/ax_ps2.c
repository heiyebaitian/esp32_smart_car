/**
  ******************************************************************************
  * @file    ax_ps2.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   X-CTR100 PS2无线手柄函数文件，针对模拟模式“红灯模式”
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  * 
  ******************************************************************************
  */  

#include "ax_ps2.h" 

//PS手柄的输入输出口
#define DATA_read()  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)
#define CM_H()       GPIO_SetBits(GPIOD, GPIO_Pin_5)
#define CM_L()       GPIO_ResetBits(GPIOD, GPIO_Pin_5)
#define ATT_H()      GPIO_SetBits(GPIOD, GPIO_Pin_7)
#define ATT_L()      GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define CLK_H()      GPIO_SetBits(GPIOD, GPIO_Pin_4)
#define CLK_L()      GPIO_ResetBits(GPIOD, GPIO_Pin_4)
#define ACK_read()   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)

const uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //请求获取数据命令                         
static uint8_t PS2_data[9] = {0};//接收的数据

/**
  * @简  述  PS2初始化
  * @参  数  无
  * @返回值  无
  */
void AX_PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	
	//DATA 信号从手柄到主机   输入口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//COMMMAND 信号从主机到手柄  输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//ATT 手柄出发信号 信号在通信期间处于低电平	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//CLOCK 信号从主机到手柄   输出口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	//ACK 信号从手柄到主机的应答信号   输入口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


/**
  * @简  述  PS2数据读写函数
  * @参  数  data:要写入的数据
  * @返回值  读出数据
  */
static uint8_t PS2_ReadWriteData(uint8_t data)
{
	uint8_t ref,res;
	
	for(ref = 0x01; ref > 0x00; ref <<= 1)
	{
		CLK_L();
		if(ref&data)
			CM_H();
		else
			CM_L();	
		AX_Delayus(5);
		CLK_H();
		if(DATA_read())
			res |= ref; 
		AX_Delayus(5);		
	}
	CM_H();

	//返回读出数据
    return res;	
}

/**
  * @简  述  PS2获取按键及摇杆数值。
  * @参  数  *JoystickStruct 手柄键值结构体
  * @返回值  无
  */
void AX_PS2_ScanKey(JOYSTICK_TypeDef *JoystickStruct)
{
	uint8_t i;
	
	ATT_L();
	AX_Delayus(1);
	for(i=0; i<9; i++)
	{
		PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
		AX_Delayus(16);
	}
	
	ATT_H();

	JoystickStruct->mode = PS2_data[1];
	JoystickStruct->btn1 = PS2_data[3];
	JoystickStruct->btn2 = PS2_data[4];
	JoystickStruct->RJoy_LR = PS2_data[5];
	JoystickStruct->RJoy_UD = PS2_data[6];
	JoystickStruct->LJoy_LR = PS2_data[7];
	JoystickStruct->LJoy_UD = PS2_data[8];
}

/******************* (C) 版权 2018 XTARK **************************************/
