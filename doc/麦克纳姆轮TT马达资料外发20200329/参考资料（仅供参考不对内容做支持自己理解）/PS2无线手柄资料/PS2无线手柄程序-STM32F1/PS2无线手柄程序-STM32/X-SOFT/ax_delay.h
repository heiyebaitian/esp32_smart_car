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
  * @日  期  2019-10-26
  * @内  容  软件延时函数
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AX_DELAY_H
#define __AX_DELAY_H

/* Includes ------------------------------------------------------------------*/	 
#include "stm32f10x.h"

//X-SOFT接口函数
void AX_DELAY_Init(void); //延时初始化
void AX_Delayus(__IO uint16_t us);  //软件微妙延时
void AX_Delayms(__IO uint16_t ms);  //软件毫秒延时

#endif 

/******************* (C) 版权 2019 XTARK **************************************/
