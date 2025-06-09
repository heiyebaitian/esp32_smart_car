/**
  ******************************************************************************
  * @file    ax_ps2.h
  * @author  Musk Han @ XTARK
  * @version V1.1
  * @date    2018-01-01
  * @brief   X-CTR100 PS2无线手柄函数头文件
  *
  ******************************************************************************
  * @xtark  实验平台：塔克出品 X-CTR100 控制器
  *         塔克官网社区：http://www.xtark.cn
  *
  ******************************************************************************
  */
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_PS2_H
#define __AX_PS2_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "ax_basis.h"

//手柄键值数据结构体	 
typedef struct			 				
{
  uint8_t mode;							 /* 手柄的工作模式 */

  uint8_t btn1;                      /* Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7 
                                           SLCT           STRT UP   R    DOWN L   */

  uint8_t btn2;                      /* Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7 
                                           L2   R2   L1   R1   /\   O    X    口   */

  uint8_t RJoy_LR;                       /*右边摇杆  0x00 = 左    0xff = 右   */

  uint8_t RJoy_UD;                       /*右边摇杆  0x00 = 上    0xff = 下   */

  uint8_t LJoy_LR;                       /*右边摇杆  0x00 = 左    0xff = 右   */

  uint8_t LJoy_UD;                       /*右边摇杆  0x00 = 上    0xff = 下   */
	
}JOYSTICK_TypeDef;

/*** X-CTR100 PS2索尼无线手柄操作函数 **********/
void AX_PS2_Init(void);  //PS2初始化
void AX_PS2_ScanKey(JOYSTICK_TypeDef* JoystickStruct);//PS2获取按键及摇杆数值

#endif

/******************* (C) 版权 2018 XTARK **************************************/
