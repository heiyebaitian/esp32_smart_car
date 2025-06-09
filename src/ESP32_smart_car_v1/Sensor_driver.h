#ifndef __SENSOR_DRIVER__H
#define __SENSOR_DRIVER__H
#pragma once

#include "SonoLuminAlert.h"

extern uint16_t mq2_Value; // MQ2烟雾传感器模拟量数值
extern uint16_t fire_sensor_Value; // 火焰传感器模拟量数值

extern SLA_MODE SLA_mode;

#endif