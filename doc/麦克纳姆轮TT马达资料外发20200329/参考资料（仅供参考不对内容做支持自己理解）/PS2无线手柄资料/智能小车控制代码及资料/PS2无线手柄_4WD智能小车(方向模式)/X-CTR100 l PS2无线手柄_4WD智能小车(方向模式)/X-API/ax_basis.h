/**
  ******************************************************************************
  * @file    ax_basis.h
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100������Դ������������ͷ�ļ�
  *          LED��BEEP�����뿪�ء��δ�ʱ��UART1
  *
  ******************************************************************************
  * @xtark  ʵ��ƽ̨�����˳�Ʒ X-CTR100 ������
  *         ���˹���������http://www.xtark.cn
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_BASIS_H
#define __AX_BASIS_H

/* Includes ------------------------------------------------------------------*/	 
#include "stm32f4xx.h"

/*** X-CTR100 λ��������ʵ����51GPIO���� **********/
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     
#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO�ڲ���
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����
#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����
#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����


/*** X-CTR100 �������ܲ������� **********/
//X-CTR100 ��ʼ������
void AX_Init(uint32_t baud);    //X-CTR100��ʼ��

//X-CTR100 LED���������궨��
#define AX_LEDG_Off()  	    GPIO_SetBits(GPIOD, GPIO_Pin_11)      //LEDG��ɫϨ��
#define AX_LEDG_On()		GPIO_ResetBits(GPIOD, GPIO_Pin_11)    //LEDG��ɫ����
#define AX_LEDG_Toggle()    GPIO_WriteBit(GPIOD, GPIO_Pin_11, (BitAction) (1 - GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)))	//LEDG��ɫ״̬��ת
#define AX_LEDR_Off()       GPIO_SetBits(GPIOD, GPIO_Pin_10)      //LEDR��ɫϨ��
#define AX_LEDR_On()  	    GPIO_ResetBits(GPIOD, GPIO_Pin_10)    //LEDR��ɫ����
#define AX_LEDR_Toggle()    GPIO_WriteBit(GPIOD, GPIO_Pin_10, (BitAction) (1 - GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)))	//LEDR��ɫ״̬��ת

//X-CTR100 ���������������궨��
#define AX_BEEP_Off()       GPIO_SetBits(GPIOE, GPIO_Pin_6)       //����������
#define AX_BEEP_On()		GPIO_ResetBits(GPIOE, GPIO_Pin_6)     //����������
#define AX_BEEP_Toggle()    GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction) (1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)))    //������״̬��ת

//X-CTR100 ��ȡ���뿪��״̬����
uint8_t AX_SW_GetSW12Status(void);   //SW���ѡ��״̬
uint8_t AX_SW_GetSW1Status(void);    //SW1���ѡ��״̬
uint8_t AX_SW_GetSW2Status(void);    //SW2���ѡ��״̬

//X-CTR100 �����ʱ����
void AX_Delayus(__IO uint16_t us);    //���΢����ʱ
void AX_Delayms(__IO uint16_t ms);    //���������ʱ

#endif 

/******************* (C) ��Ȩ 2018 XTARK **************************************/
