#include "VL53L1X_i2c.h"
#include "stm32f10x.h"
#include "delay.h"

void VL53L1_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE );	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_3;    //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //50Mhz速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);//PA2,PA3 输出高	
}
void VL_IIC_Start(void)
{
	VL_SDA_OUT();//sda线输出
	VL_IIC_SDA=1;	  	  
	VL_IIC_SCL=1;
	delay_us(4);
 	VL_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	VL_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void VL_IIC_Stop(void)
{
	VL_SDA_OUT();//sda线输出
	VL_IIC_SCL=0;
	VL_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	VL_IIC_SCL=1; 
	VL_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

u8 VL_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	VL_SDA_IN();  //SDA设置为输入  
	VL_IIC_SDA=1;delay_us(1);	   
	VL_IIC_SCL=1;delay_us(1);	 
	while(VL_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			VL_IIC_Stop();
			return 1;
		}
	}
	VL_IIC_SCL=0;//时钟输出0 	   
	return 0;  
}

//产生ACK应答
void VL_IIC_Ack(void)
{
	VL_IIC_SCL=0;
	VL_SDA_OUT();
	VL_IIC_SDA=0;
	delay_us(2);
	VL_IIC_SCL=1;
	delay_us(2);
	VL_IIC_SCL=0;
}

//不产生ACK应答		    
void VL_IIC_NAck(void)
{
	VL_IIC_SCL=0;
	VL_SDA_OUT();
	VL_IIC_SDA=1;
	delay_us(2);
	VL_IIC_SCL=1;
	delay_us(2);
	VL_IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void VL_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	VL_SDA_OUT(); 	    
    VL_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			VL_IIC_SDA=1;
		else
			VL_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);  
		VL_IIC_SCL=1;
		delay_us(2); 
		VL_IIC_SCL=0;	
		delay_us(2);
    }	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 VL_IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	VL_SDA_IN();//SDA设置为输入
	VL_IIC_SDA = 1;
	delay_us(4);
	for(i=0;i<8;i++ )
	{
		receive<<=1;
		VL_IIC_SCL=0; 
		delay_us(4);
	  VL_IIC_SCL=1;
		delay_us(4);
		if(VL_READ_SDA)
			receive |= 0x01;   
	  delay_us(4); //1
	}	
  VL_IIC_SCL = 0;	
	return receive;
}

//IIC写一个字节数据
u8 VL_IIC_Write_1Byte(u8 SlaveAddress, u16 REG_Address,u8 REG_data)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);
	if(VL_IIC_Wait_Ack())
	{
		VL_IIC_Stop();//释放总线
		return 1;//没应答则退出

	}
	VL_IIC_Send_Byte(REG_Address >> 8);
	VL_IIC_Wait_Ack();	
	VL_IIC_Send_Byte(REG_Address & 0x00ff);
	VL_IIC_Wait_Ack();
	delay_ms(1);
	VL_IIC_Send_Byte(REG_data);
	VL_IIC_Wait_Ack();	
	VL_IIC_Stop();

	return 0;
}

//IIC读一个字节数据
u8 VL_IIC_Read_1Byte(u8 SlaveAddress, u16 REG_Address,u8 *REG_data)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL_IIC_Wait_Ack())
	{
		 VL_IIC_Stop();//释放总线
		 return 1;//没应答则退出
	}		
	VL_IIC_Send_Byte(REG_Address >> 8);
	VL_IIC_Wait_Ack();
	VL_IIC_Send_Byte((REG_Address & 0x00ff));
	VL_IIC_Wait_Ack();
	delay_ms(1);
	VL_IIC_Start(); 
	VL_IIC_Send_Byte(SlaveAddress|0x01);//发读命令
	VL_IIC_Wait_Ack();
	*REG_data = VL_IIC_Read_Byte();
	VL_IIC_Stop();

	return 0;
}

//I2C read n bytes
uint8_t VL_I2C_Read_nByte(uint8_t SlaveAddress, uint16_t REG_Address, uint8_t *buf, uint16_t len)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL_IIC_Wait_Ack()) 
	{
		VL_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	VL_IIC_Send_Byte(REG_Address >> 8);
	VL_IIC_Wait_Ack();
	VL_IIC_Send_Byte((REG_Address & 0x00ff));
	VL_IIC_Wait_Ack();
	delay_ms(1);
	VL_IIC_Start(); 
	VL_IIC_Send_Byte(SlaveAddress|0x01);//发读命令
	VL_IIC_Wait_Ack();
	while(len)
	{
		*buf = VL_IIC_Read_Byte();
		if(1 == len)
		{
			VL_IIC_NAck();
		}
		else
		{
			VL_IIC_Ack();
		}
		buf++;
		len--;
	}
	VL_IIC_Stop();
	//if(HAL_I2C_Mem_Read(&hDevI2c, (uint16_t)SlaveAddress, REG_Address, I2C_MEMADD_SIZE_16BIT, buf, len, I2C_RX_TIMEOUT)!=HAL_OK)
		//return STATUS_FAIL;

	return STATUS_OK;
}

//I2C write n bytes
uint8_t VL_I2C_Write_nByte(uint8_t SlaveAddress, uint16_t REG_Address, uint8_t *buf, uint16_t len)
{
	VL_IIC_Start();
	VL_IIC_Send_Byte(SlaveAddress);//发写命令
	if(VL_IIC_Wait_Ack()) 
	{
		VL_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	VL_IIC_Send_Byte(REG_Address >> 8);
	VL_IIC_Wait_Ack();
	VL_IIC_Send_Byte(REG_Address & 0x00ff);
	VL_IIC_Wait_Ack();
	while(len--)
	{
		VL_IIC_Send_Byte(*buf++);
		VL_IIC_Wait_Ack();
	}
	VL_IIC_Stop();
	//if(HAL_I2C_Mem_Write(&hDevI2c, (uint16_t)SlaveAddress, REG_Address, I2C_MEMADD_SIZE_16BIT, buf, len, I2C_TX_TIMEOUT)!=HAL_OK)
		//return STATUS_FAIL;

	return STATUS_OK;
}
//VL53L1X wrtie multi data
//address: device addr
//index: reg addr
//pdata
//count
uint8_t VL53L1_write_multi(uint8_t address, uint16_t index, uint8_t *pdata, uint16_t count)
{
	uint8_t status = STATUS_OK;
	
	if(VL_I2C_Write_nByte(address,index,pdata,count))
	   status  = STATUS_FAIL;

	return status;
}


//VL53L1X read multi data
//address: dev addr
//index: reg addr
//pdata
//count
uint8_t VL53L1_read_multi(uint8_t address,uint16_t index,uint8_t *pdata,uint16_t count)
{
	uint8_t status = STATUS_OK;

	if(VL_I2C_Read_nByte(address,index,pdata,count))
	  status  = STATUS_FAIL;

	return status;
}

//VL53L1X write 1 byte
//address: dev addr
//index: reg addr
//data (8 bits)
uint8_t VL53L1_write_byte(uint8_t address,uint16_t index,uint8_t data)
{
	uint8_t status = STATUS_OK;

	status = VL53L1_write_multi(address,index,&data,1);

	return status;
}

//VL53L1X write 1 word (2 bytes)
//address: dev addr
//index: reg addr
//data (16 bits)
uint8_t VL53L1_write_word(uint8_t address, uint16_t index, uint16_t data)
{
	uint8_t status = STATUS_OK;
	
	uint8_t buf[2];
	
	buf[0] = (uint8_t)(data >> 8);	//H
	buf[1] = (uint8_t)(data & 0xff);	//L
	
	if(index % 2 == 1)
	{
		status = VL53L1_write_multi(address,index,&buf[0],1);
		status = VL53L1_write_multi(address,index,&buf[0],1);
	}
	status = VL53L1_write_multi(address,index,buf,2);
	
	return status;
}

//VL53L1X a dword(4 bytes)
//address: dev addr
//index: reg addr
//data (32 bits)
uint8_t VL53L1_write_dword(uint8_t address, uint16_t index, uint32_t data)
{
  uint8_t status = STATUS_OK;

  uint8_t buf[4];	
	
	buf[0] = (uint8_t)(data >> 24);
	buf[1] = (uint8_t)((data & 0xff0000) >> 16);
	buf[2] = (uint8_t)((data & 0xff00) >> 8);
	buf[3] = (uint8_t)(data & 0xff);
	
	status = VL53L1_write_multi(address,index,buf,4);
	
	return status;
}


//VL53L1X read 1 byte
//address: dev addr
//index: reg addr
//data (8 bits)
uint8_t VL53L1_read_byte(uint8_t address, uint16_t index, uint8_t *pdata)
{
	uint8_t status = STATUS_OK;
	 
	status = VL53L1_read_multi(address, index, pdata, 1);
	return status;
}

//VL53L1X read a word
//address: dev addr
//index: reg addr
//data (16 bits)
uint8_t VL53L1_read_word(uint8_t address, uint16_t index, uint16_t *pdata)
{
	uint8_t status = STATUS_OK;
	uint8_t buf[2];
	
	status = VL53L1_read_multi(address, index, buf, 2);
	*pdata = ((uint16_t)buf[0] << 8)+(uint16_t)buf[1];
	
	return status;
}

//VL53L1X read a dword
//address: dev addr
//index: reg addr
//data (32 bits)
uint8_t VL53L1_read_dword(uint8_t address, uint16_t index, uint32_t *pdata)
{
	uint8_t status = STATUS_OK;
	
	uint8_t buf[4];
	
	status = VL53L1_read_multi(address,index,buf,4);
	
	*pdata = ((uint32_t)buf[0] << 24)+((uint32_t)buf[1] << 16)+((uint32_t)buf[2] << 8)+((uint32_t)buf[3]);
	
	return status;
}

void DelayUs(uint16_t n)		//need tested arrcoding to system clock, for 30Mhz a slight slow 
{
	uint16_t u16;
	
	while(n)
	{
		u16=7;
		while(u16)
			u16--;
		
		n--;
	}
}

