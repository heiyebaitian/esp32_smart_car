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
  * @日  期  2019-7-26
  * @内  容  主程序
  *
  ******************************************************************************
  * @说  明
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#include "ax_sys.h"    //系统设置
#include "ax_delay.h"  //软件延时
#include "ax_uart_db.h"  //调试串口，USB串口

#include "ax_ps2.h" //PS手柄

int main(void)
{
	uint8_t psdata[9];  //PS2手柄遥控数据
	
	//JTAG口设置
	AX_JTAG_Set(JTAG_SWD_DISABLE);  //关闭JTAG接口 
	AX_JTAG_Set(SWD_ENABLE);  //打开SWD接口 可以利用主板的SWD接口调试 
	
	//软件延时初始化
	AX_DELAY_Init(); 	

	//调试串口初始化
	AX_UART_DB_Init(115200); //调试串口
	printf("  \r\n"); //输出空格，CPUBUG
	
	AX_Delayms(10);
	AX_PS2_Init(); //PS2手柄初始化
	AX_Delayms(10);
	
	while (1)
	{	
		//PS2检测
		AX_PS2_GetKeyData(psdata);  //获取PS2键值
		
		//打印手柄键值
		printf("MODE:%2x    BTN:%2x %2x    JOY:%2x %2x %2x %2x\r\n",
		psdata[1],   psdata[3],psdata[4],
		psdata[5],psdata[6],psdata[7],psdata[8]);	
		
		AX_Delayms(10);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
