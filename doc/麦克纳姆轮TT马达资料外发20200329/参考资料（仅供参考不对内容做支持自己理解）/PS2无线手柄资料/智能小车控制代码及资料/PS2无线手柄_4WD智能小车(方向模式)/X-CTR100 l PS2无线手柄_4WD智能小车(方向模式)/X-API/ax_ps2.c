/**
  ******************************************************************************
  * @file    ax_ps2.c
  * @author  Musk Han @ XTARK
  * @version V1.0
  * @date    2018-01-01
  * @brief   X-CTR100 PS2�����ֱ������ļ������ģ��ģʽ�����ģʽ��
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  * 
  ******************************************************************************
  */  

#include "ax_ps2.h" 

//PS�ֱ������������
#define DATA_read()  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)
#define CM_H()       GPIO_SetBits(GPIOD, GPIO_Pin_5)
#define CM_L()       GPIO_ResetBits(GPIOD, GPIO_Pin_5)
#define ATT_H()      GPIO_SetBits(GPIOD, GPIO_Pin_7)
#define ATT_L()      GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define CLK_H()      GPIO_SetBits(GPIOD, GPIO_Pin_4)
#define CLK_L()      GPIO_ResetBits(GPIOD, GPIO_Pin_4)
#define ACK_read()   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)

const uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //�����ȡ��������                         
static uint8_t PS2_data[9] = {0};//���յ�����

/**
  * @��  ��  PS2��ʼ��
  * @��  ��  ��
  * @����ֵ  ��
  */
void AX_PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	
	//DATA �źŴ��ֱ�������   �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//COMMMAND �źŴ��������ֱ�  ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//ATT �ֱ������ź� �ź���ͨ���ڼ䴦�ڵ͵�ƽ	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//CLOCK �źŴ��������ֱ�   �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	//ACK �źŴ��ֱ���������Ӧ���ź�   �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


/**
  * @��  ��  PS2���ݶ�д����
  * @��  ��  data:Ҫд�������
  * @����ֵ  ��������
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

	//���ض�������
    return res;	
}

/**
  * @��  ��  PS2��ȡ������ҡ����ֵ��
  * @��  ��  *JoystickStruct �ֱ���ֵ�ṹ��
  * @����ֵ  ��
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

/******************* (C) ��Ȩ 2018 XTARK **************************************/
