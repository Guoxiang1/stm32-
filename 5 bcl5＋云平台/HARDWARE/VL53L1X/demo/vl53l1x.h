#ifndef __VL53L1_H
#define __VL53L1_H

#include "stm32f10x.h"
#include "laser_def.h"
#include "VL53L1_register_map.h"
#include "VL53L1_api.h"
#include "VL53L1_platform.h"
//#include "VL53L1X_cali.h"
//#include "VL53L1_it.h"

//enable 2.8V IO mode
#define USE_I2C_2V8		1

//VL53L1X default I2C addr is 0x52 after power on
#define VL53L1_Addr0 0x52
#define VL53L1_Addr1 0x54

//detection mode
#define DEFAULT_MODE   0	//default,see manul 5.3.1
#define HIGH_ACCURACY  1
#define LONG_RANGE     2
#define HIGH_SPEED     3

//param struct for vl53l1x mode option, in manual 6.2 
typedef __packed struct
{
	FixPoint1616_t signalLimit;    //Signal,related to reflected amplitude 
	FixPoint1616_t sigmaLimit;     //Sigmal, related to distance mm
//	FixPoint1616_t ignoreThres;  //ignore threshold
	uint32_t timingBudget;         //When the ranging mode is set to timed ranging, user has to define the period of time
																 //between two consecutive measurements.
	uint8_t preRangeVcselPeriod ;  //VCSEL pulse cycle
	uint8_t finalRangeVcselPeriod ;//VCSEL pulse cycle period
}mode_data;


extern mode_data Mode_data[];
extern uint8_t Ajusted[];
extern VL53L1_RangingMeasurementData_t VL53L1_data[];
extern uint16_t Distance_data[];//the detected distance;
extern vu16 Distance;//保存测距数据

VL53L1_Error VL53L1_addr_set(VL53L1_Dev_t *dev, uint8_t newaddr);
VL53L1_Error VL53L1_reset(VL53L1_Dev_t *pDev);
VL53L1_Error VL53L1_init(VL53L1_Dev_t *pDev);	//init vl53l1x
void VL53L1_test(void);//vl53l1x test

void VL53L1_info(void);//get vl53l1x ID info
void One_measurement(uint8_t mode);//获取一次测量距离数据

VL53L1_Error VL53L1_set_mode(VL53L1_Dev_t *dev,uint8_t mode);
VL53L1_Error VL53L1_single_test(VL53L1_Dev_t *dev,VL53L1_RangingMeasurementData_t *pdata);
void VL53L1_general_start(VL53L1_Dev_t *dev,uint8_t mode);
void VL53L1_general_test(VL53L1_Dev_t *dev);
#endif


