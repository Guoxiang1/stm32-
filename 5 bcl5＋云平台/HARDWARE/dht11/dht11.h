#ifndef	MY_DHT11_H
#define	MY_DHT11_H
#include "sys.h"

#define DHT11_IO_IN()	{GPIOA->CRL&=0xFFFFFFF0;GPIOA->CRL|=8;}
#define DHT11_IO_OUT()	{GPIOA->CRL&=0xFFFFFFF0;GPIOA->CRL|=3;}

#define DHT11_DQ_OUT	PAout(0)
#define DHT11_DQ_IN		PAin(0)

u8 DHT11_init(void);
void DHT11_Rest(void);
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);

#endif
