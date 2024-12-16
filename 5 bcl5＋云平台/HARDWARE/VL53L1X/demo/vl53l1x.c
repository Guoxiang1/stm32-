#include "vl53l1x.h"
#include "delay.h"
#include "usart.h"

VL53L1_Dev_t VL53L1_dev[2];	//device param, include I2C
//VL53L1_DeviceInfo_t VL53L1_dev_info[2];//device ID version info
uint8_t Ajusted[]={0,0};//adjusted sign, 0-not, 1-had
vu16 Distance = 0;//保存测距数据
VL53L1_RangingMeasurementData_t VL53L1_data[2];//ranging result struct, distance, max distance,etc.

//VL53L1X mode param
//0：default, 1: high accuracy, 2: long distance, 3: high speed
mode_data Mode_data[]=
{
	{(FixPoint1616_t)(16384), //its uint32_t, uint16_.uint16_t, 0.25*65536
	 (FixPoint1616_t)(1179648),	 //18*65536
	 33000,
	 14,
	 10},//default
		
	{(FixPoint1616_t)(16384),	//0.25*65536
	 (FixPoint1616_t)(1179648),		//18*65536
	 200000, 
	 14,
	 10},//high accuracy
		
  {(FixPoint1616_t)(6554),		//0.1*65536
	 (FixPoint1616_t)(3932160),		//60*65536
	 33000,
	 18,
	 14},//long distance
	
  {(FixPoint1616_t)(16384),	//0.25*65536
	 (FixPoint1616_t)(2097152),		//32*65536
	 20000,
	 14,
	 10},//high speed
};

//config VL53L1X I2C addr
//dev: I2C param struct
//newaddr: device new I2C addr
VL53L1_Error VL53L1_addr_set(VL53L1_Dev_t *dev, uint8_t newaddr)
{
	uint16_t Id;
	VL53L1_Error Status = VL53L1_Error_NONE;
	
	if(newaddr == dev->I2cDevAddr) //if new addr == old addr, exit
		return VL53L1_Error_NONE;
	
	//try to access a reg with old addr(default 0x52) 
	Status = VL53L1_RdWord(dev, VL53L1_IDENTIFICATION__MODEL_ID, &Id);
	if(Status!=VL53L1_Error_NONE) 
		return Status; 

	if(Id == 0xEACC)
	{
		//set new I2C addr
		Status = VL53L1_SetDeviceAddress(dev,newaddr);
		if(Status!=VL53L1_Error_NONE) 
			return Status; 
		
		//update struct I2C addr
		dev->I2cDevAddr = newaddr;
		//check new I2C addr
		Status = VL53L1_RdWord(dev, VL53L1_IDENTIFICATION__MODEL_ID, &Id);
	}

	return Status;
}


VL53L1_Error VL53L1_reset(VL53L1_Dev_t *pDev)
{
	return VL53L1_init(pDev);
}

void VL53l1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = XShut_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//GPIO_ResetBits(GPIOA,XShut2_Pin | XShut_Pin);
}
//init vl53l1x
//dev: I2C param struct	
//before call this func, set:
//    pDev->I2cDevAddr = DevAddr;	---VL53L1X I2C addr (default 0x52 at power on)
// 		pDev->comms_type = 1;        	//I2C comm mode
//		pDev->comms_speed_khz = 400; 	//I2C bps
//if resetting device, func(pDev),DevAddr=0x52 
//Xshut为输入管脚，用于模式选择（休眠），需要上拉电阻防止漏电流。
VL53L1_Error VL53L1_init(VL53L1_Dev_t *pDev)
{
	VL53L1_Error Status = VL53L1_Error_NONE;
	VL53L1_Dev_t *pMyDevice = pDev;
	//uint8_t I2cAddr; 
	pMyDevice->I2cDevAddr = 0x52;//I2C地址(上电默认0x52)
	pMyDevice->comms_type = 1;           //I2C通信模式
	pMyDevice->comms_speed_khz = 400;    //I2C通信速率
	//I2cAddr=pMyDevice->I2cDevAddr;
	
//	VL53L1_i2c_init();//init I2C bus
	VL53l1_GPIO_Init();
	VL53L1_I2C_Init();
	
	XShut_Off;   //传感器电源关闭
	delay_ms(30);
	XShut_On;
	delay_ms(30);
	
	//确保设备启动且准备就绪 该功能可选	这是一个阻塞函数，内部有个轮询 假设400kHz I2C和每个事务延迟2ms 此功能阻塞不能超过4ms
	Status = VL53L1_WaitDeviceBooted(pMyDevice);  
    if(Status!=VL53L1_Error_NONE)
	{
		printf("Wait device Boot failed!\r\n");
		return Status;
	}

  delay_ms(2);
  Status = VL53L1_addr_set(pMyDevice, 0x54);//set provious VL53L1X I2C addr
  if(Status!=VL53L1_Error_NONE)
  {
	  printf("set addr failed!\r\n");
	  return Status;
  }

	Status = VL53L1_DataInit(pMyDevice);//数据初始化 设备初始化 器件退出复位后被调用且仅被调用一次
	if(Status!=VL53L1_Error_NONE) 
	{
		printf("datainit failed!\r\n");
		return Status;
	}

	delay_ms(2);
//--may be delete
	Status = VL53L1_StaticInit(pMyDevice); //静态初始化 
	if(Status!=VL53L1_Error_NONE) 
	{
		printf("static init failed!\r\n");
		return Status;
	}
	 
/*	HAL_Delay(2);
	Status = VL53L1_PerformRefSpadManagement(pMyDevice);//perform ref SPAD management
	if(Status!=VL53L1_Error_NONE) 
		return Status;
*/
	Status = VL53L1_SetDistanceMode(pMyDevice, VL53L1_DISTANCEMODE_LONG);	//short,medium,long
	if(Status!=VL53L1_Error_NONE) 
	{
		printf("set discance mode failed!\r\n");
		return Status;
	}
	delay_ms(2);
	/*Status = VL53L1_SetInterMeasurementPeriodMilliSeconds(pMyDevice,500);  //设置测试时间间隔
	//Status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,50000);
	if(Status!=VL53L1_Error_NONE) 
	{
		printf("set time failed!\r\n");
		return Status;
	}
	delay_ms(2);
	Status = VL53L1_StartMeasurement(pMyDevice);
	if(Status!=VL53L1_Error_NONE) 
	{
		printf("start measurement failed!\r\n");
		return Status;
	}*/
//--may be finished

	
	return Status;
}


//VL53L1X test mode config
//dev: device I2C param struct
//mode: 0:default, 1:high accuracy, 2:long distance

VL53L1_Error VL53L1_set_mode(VL53L1_Dev_t *dev, uint8_t mode)
{
	VL53L1_Error status = VL53L1_Error_NONE;
	//VL53L1_reset(dev);//reset vl53l1x (to avoid the distance result error due to frequently changing running mode)
	//status = VL53L1_StaticInit(dev);
	 
	delay_ms(2);
	status = VL53L1_PerformRefSpadManagement(dev);//perform ref SPAD management 校准
	if(status!=VL53L1_Error_NONE) 
	{
		printf("refspad failed!\r\n");
		return status;
	}
	delay_ms(2);
	
	status = VL53L1_SetDistanceMode(dev, VL53L1_DISTANCEMODE_LONG);	//short,medium,long
	if(status!=VL53L1_Error_NONE) 
		return status;

	//HAL_Delay(2);
	delay_ms(30);
	//限定测距测量
	status = VL53L1_SetLimitCheckEnable(dev,VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,1);//sigma--standard deviation, enable SIGMA limit check 测量标准偏差估计
	if(status!=VL53L1_Error_NONE) 
		return status;
	
	//HAL_Delay(2);
	delay_ms(2);
	status = VL53L1_SetLimitCheckEnable(dev,VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//signal--amplitude of the signal-
	//信号速率																																											//-reflected. enable signal rate limit check
	if(status!=VL53L1_Error_NONE) 
		return status;
	
	//HAL_Delay(2);
	delay_ms(2);
	status = VL53L1_SetLimitCheckValue(dev,VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//set SIGMA limit
	if(status!=VL53L1_Error_NONE) 
		return status;
	
	//HAL_Delay(2);
	delay_ms(2);
	status = VL53L1_SetLimitCheckValue(dev,VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//set signal rate limit
	if(status!=VL53L1_Error_NONE) 
		return status;
	
	status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);//set the max interval for a whole diatance test
	if(status!=VL53L1_Error_NONE) 
		return status;

	//HAL_Delay(2);
	delay_ms(2);
	status = VL53L1_SetInterMeasurementPeriodMilliSeconds(dev, 500);
	if(status!=VL53L1_Error_NONE) 
	{
		printf("SetInterMeasurementPeriodMilliSeconds failed!\r\n");
		return status;
	}
	delay_ms(2);
	status = VL53L1_StartMeasurement(dev);  //开始测量
	if(status!=VL53L1_Error_NONE) 
	{
		printf("start measurement failed!\r\n");
		return status;
	}
	
	return status;
}

//VL53L1X single ranging
//dev: device I2C param struct
//pdata: result struct
/*
等待结果：轮询或中断
三种知道数据可用 
1、主机可调用轮询功能，等待测距距离可用
	函数VL53L1_Error VL53L1_WaitMeasurementDataReady(VL53L1_DEV Dev) 查询设备中断，直到测距数据就绪，只要该功能
未完成，该功能就阻止主机上其他所有操作，因为执行了内部轮询。

2、主机可轮询功能询问测距数据是否可用
	函数VL53L1_Error VL53L1_GetMeasurementDataReady(VL53L1_DEV Dev,uint8_t *pMeasurementDataReady) 该功能不会阻止其他
操作，是使用轮询功能首选

3、主机可以等待物理中断
	使用物理中断：默认情况下，当新的数据就绪时，GPIO被拉低

*/
VL53L1_Error VL53L1_single_test(VL53L1_Dev_t *dev,VL53L1_RangingMeasurementData_t *pdata) 
{
	VL53L1_Error Status = VL53L1_Error_NONE;
	u8 isDataReady = 0;
	/*Status = VL53L1_StartMeasurement(dev);
	if(Status != VL53L1_Error_NONE)
		return Status;*/
	
//	Status = VL53L1_poll_for_range_completion(dev, VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS);
	Status = VL53L1_GetMeasurementDataReady(dev,&isDataReady);//数据是否就绪
	//Status = VL53L1_WaitMeasurementDataReady(dev);
	if(Status != VL53L1_Error_NONE)
		return Status;
    if(1 == isDataReady)
	{
		Status = VL53L1_GetRangingMeasurementData(dev, pdata); //获取测量数据
		Distance = pdata->RangeMilliMeter;    //测量距离赋值给Distance
	}
	
	/*清除中断来源 为了获得一致的结果，在进行测距测量后必须调用该函数，如果未调用该函数，下一个测距开始且测量结果会更新
	但是数据就绪状态标志不能正常更新*/
	Status = VL53L1_ClearInterruptAndStartMeasurement(dev);
	return Status;
}
   
//	Distance_data = pdata->RangeMilliMeter;//output the final distance data

//start general test. repeatly. not used
//dev:device I2C param struct
//mode0:default, 1:high accuracy, 2:long distance
void VL53L1_general_start(VL53L1_Dev_t *dev,uint8_t mode)
{
	VL53L1_Error Status=VL53L1_Error_NONE;
	uint8_t i=0;
	
	while(VL53L1_set_mode(dev,mode)) //config test mode
	{
		i++;
		if(i==2) 
			return;
	}
	
	while(Status==VL53L1_Error_NONE)
	{
		Status = VL53L1_single_test(dev,&VL53L1_data[0]);//perform a test
//		Distance_data=VL53L1_data[0].RangeMilliMeter;
	}
	//HAL_Delay(50);
	delay_ms(50);
}




void test()
{
	u32 da;
	VL53L1_RangingMeasurementData_t *pRangingMeasure;
	u8	*pMeasuremenreade;
	VL53L1_Dev_t *pMyDevice;		//配置IIC参数,模式，设备地址，速率
	pMyDevice->I2cDevAddr = 0x52;
	VL53L1_I2C_Init();		//iic初始化
	VL53l1_GPIO_Init();

	VL53L1_WaitDeviceBooted(pMyDevice);	//等待设备就绪
	
	VL53L1_DataInit(pMyDevice);	//数据初始化
	VL53L1_StaticInit(pMyDevice);  //静态初始化
	
	VL53L1_SetDistanceMode(pMyDevice,VL53L1_DISTANCEMODE_LONG); 	//设置模式为长
	VL53L1_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,200);	//单次测量时间估算
	VL53L1_GetInterMeasurementPeriodMilliSeconds(pMyDevice,&da);		// 间期2次测量间期
	
	VL53L1_StartMeasurement(pMyDevice); //开始测量
	VL53L1_GetMeasurementDataReady(pMyDevice,pMeasuremenreade); //等待测量结果就续
	
	VL53L1_GetRangingMeasurementData(pMyDevice,pRangingMeasure);//获取结果
	VL53L1_ClearInterruptAndStartMeasurement(pMyDevice);	//清楚中断标志位
	
} 





