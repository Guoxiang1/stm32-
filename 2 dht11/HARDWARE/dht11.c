#include "dht11.h"
#include "delay.h"

u8 DHT11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	DHT11_Rest();
	return DHT11_Check();
}

void DHT11_Rest(void)
{
	DHT11_IO_OUT();
	DHT11_DQ_OUT = 0;
	delay_ms(20);
	DHT11_DQ_OUT = 1;
	delay_us(30);
}

u8 DHT11_Check(void)
{
	u8 r;
	DHT11_IO_IN();
	while(DHT11_DQ_IN && r<100)
	{
		r++;
		delay_us(1);
	}
	if(r>=100)
		return 1;
	else r=0;
	while(!DHT11_DQ_IN && r<100)
	{
		r++;
		delay_us(1);
	}
	if(r>=100)
		return 1;
	return 0;
}

u8 DHT11_Read_Bit(void)
{
	u8 r;
	while(DHT11_DQ_IN && r<100)
	{
		r++;
		delay_us(1);
	}
	r=0;
	while(!DHT11_DQ_IN && r<100)
	{
		r++;
		delay_us(1);
	}
	delay_us(40);
	if(DHT11_DQ_IN)
		return 1;
	else return 0;
	
}

u8 DHT11_Read_Byte(void)
{
	u8 i = 0,dat = 0;
	for(i = 0; i<8; i++)
	{
		dat <<=1;
		dat |=DHT11_Read_Bit();
	}
	return dat;
}

u8 DHT11_Read_Data(u8 *temp,u8 *humi)
{
	u8 buf[5],i;
	DHT11_Rest();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*temp=buf[0];
			*humi=buf[2];
		}
		else return 1;
	}
	else return 1;
	return 0;
}
