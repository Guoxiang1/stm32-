#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//技术支持：

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

void uart_init(u32 bound);

void USART_SendString(char *str);
void USART_SendNumber(u8 num);
#endif


