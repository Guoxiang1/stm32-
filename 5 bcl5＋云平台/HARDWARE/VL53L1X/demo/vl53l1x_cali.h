#ifndef __VL53L1_CAIL_H
#define __VL53L1_CAIL_H

#include "vl53l1x.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniV3 STM32开发板
//VL53L1X-校准模式 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/7/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//vl53l1x传感器校准信息结构体定义
typedef __packed struct
{
	uint8_t  adjustok;                    //校准成功标志，0XAA，已校准;其他，未校准
	uint8_t  isApertureSpads;             //ApertureSpads值
	uint8_t  VhvSettings;                 //VhvSettings值
	uint8_t  PhaseCal;                    //PhaseCal值
	uint32_t XTalkCalDistance;            //XTalkCalDistance值
	uint32_t XTalkCompensationRateMegaCps;//XTalkCompensationRateMegaCps值
	uint32_t CalDistanceMilliMeter;       //CalDistanceMilliMeter值
	int32_t  OffsetMicroMeter;            //OffsetMicroMeter值
	uint32_t refSpadCount;                //refSpadCount值
	
}_VL53L1_adjust;

extern _VL53L1_adjust Vl53l0x_data;

void VL53L1_calibration_test(VL53L1_Dev_t *dev);

#endif
