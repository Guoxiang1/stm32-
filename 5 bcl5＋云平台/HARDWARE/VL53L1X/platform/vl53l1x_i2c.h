#ifndef __VL53L1_I2C_H
#define __VL53L1_I2C_H

#include "stdint.h"
#include "sys.h"

//const
#define I2C_TX_TIMEOUT  0x02
#define I2C_RX_TIMEOUT  0x02

//Status
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01
#define VL_SDA_IN()  {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=8<<16;}
#define VL_SDA_OUT() {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=3<<16;}
#define VL_IIC_SCL    PBout(5)      //SCL
#define VL_IIC_SDA    PBout(4) 		//SDA	 
#define VL_READ_SDA   PBin(4) 		//输入SDA 
//I2C operation
//void VL53L1_i2c_init(void);

uint8_t VL53L1_write_byte(uint8_t address,uint16_t index,uint8_t data);              //IIC写一个8位数据
uint8_t VL53L1_write_word(uint8_t address,uint16_t index,uint16_t data);             //IIC写一个16位数据
uint8_t VL53L1_write_dword(uint8_t address,uint16_t index,uint32_t data);            //IIC写一个32位数据
uint8_t VL53L1_write_multi(uint8_t address, uint16_t index,uint8_t *pdata,uint16_t count);//IIC连续写

uint8_t VL53L1_read_byte(uint8_t address,uint16_t index,uint8_t *pdata);             //IIC读一个8位数据
uint8_t VL53L1_read_word(uint8_t address,uint16_t index,uint16_t *pdata);            //IIC读一个16位数据
uint8_t VL53L1_read_dword(uint8_t address,uint16_t index,uint32_t *pdata);           //IIC读一个32位数据
uint8_t VL53L1_read_multi(uint8_t address,uint16_t index,uint8_t *pdata,uint16_t count);  //IIC连续读

void DelayUs(uint16_t n);		//need tested arrcoding to system clock, for 30Mhz a slight slow 
void VL53L1_I2C_Init(void);
#endif 


