/**
  ******************************************************************************
  * @file    ax_basis.h
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100板载资源基础操作函数头文件
  *          LED、BEEP、编码开关、滴答定时、UART1
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_BASIS_H
#define __AX_BASIS_H

/* Includes ------------------------------------------------------------------*/	 
#include "stm32f4xx.h"

/*** X-CTR100 位带操作，实现类51GPIO控制 **********/
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO口地址映射
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
 
//IO口操作
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入
#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入


/*** X-CTR100 基本功能操作函数 **********/
//X-CTR100 初始化函数
void AX_Init(uint32_t baud);    //X-CTR100初始化

//X-CTR100 LED操作函数宏定义
#define AX_LEDG_Off()  	    GPIO_SetBits(GPIOD, GPIO_Pin_11)      //LEDG绿色熄灭
#define AX_LEDG_On()		GPIO_ResetBits(GPIOD, GPIO_Pin_11)    //LEDG绿色点亮
#define AX_LEDG_Toggle()    GPIO_WriteBit(GPIOD, GPIO_Pin_11, (BitAction) (1 - GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)))	//LEDG绿色状态翻转
#define AX_LEDR_Off()       GPIO_SetBits(GPIOD, GPIO_Pin_10)      //LEDR红色熄灭
#define AX_LEDR_On()  	    GPIO_ResetBits(GPIOD, GPIO_Pin_10)    //LEDR红色点亮
#define AX_LEDR_Toggle()    GPIO_WriteBit(GPIOD, GPIO_Pin_10, (BitAction) (1 - GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)))	//LEDR红色状态翻转

//X-CTR100 蜂鸣器操作函数宏定义
#define AX_BEEP_Off()       GPIO_SetBits(GPIOE, GPIO_Pin_6)       //蜂鸣器不鸣
#define AX_BEEP_On()		GPIO_ResetBits(GPIOE, GPIO_Pin_6)     //蜂鸣器鸣叫
#define AX_BEEP_Toggle()    GPIO_WriteBit(GPIOE, GPIO_Pin_6, (BitAction) (1 - GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)))    //蜂鸣器状态翻转

//X-CTR100 获取拨码开关状态函数
uint8_t AX_SW_GetSW12Status(void);   //SW获得选择状态
uint8_t AX_SW_GetSW1Status(void);    //SW1获得选择状态
uint8_t AX_SW_GetSW2Status(void);    //SW2获得选择状态

//X-CTR100 软件延时函数
void AX_Delayus(__IO uint16_t us);    //软件微妙延时
void AX_Delayms(__IO uint16_t ms);    //软件毫秒延时

#endif 

/******************* (C) 版权 2018 XTARK **************************************/
