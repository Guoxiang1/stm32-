#ifndef __ALARM_H
#define __ALARM_H

#include "vl53l1x.h"

//void VL53L1_interrupt_test(VL53L1_Dev_t *dev);
VL53L1_Error VL53L1_interrupt_start(VL53L1_Dev_t *dev,uint8_t mode);

#endif 
