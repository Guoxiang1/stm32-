#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // 标准库头文件
#include <string.h>
#include <dht11.h>

// Function prototypes
void GPIO_Init_LEDs(void); // 初始化GPIO

int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // 初始化led引脚       
		DHT11_init();
		uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		PBout(1)=1;
		PBout(10)=1;
	
    while (1) {
			if(USART_RX_STA & 0x8000)
				{
					printf("接收：%s\n",USART_RX_BUF);
           		 if (strstr((const char *)USART_RX_BUF, "打开led1")) {
						    GPIO_ResetBits(GPIOB, GPIO_Pin_10); // LED1灭
                GPIO_ResetBits(GPIOB, GPIO_Pin_1);  // LED2灭
                GPIO_SetBits(GPIOA, GPIO_Pin_4);  // 蜂鸣器响
           		 } 
							 
							 
							 else {
                GPIO_SetBits(GPIOB, GPIO_Pin_10); // LED1亮
                GPIO_SetBits(GPIOB, GPIO_Pin_1);  // LED2亮
                GPIO_ResetBits(GPIOA, GPIO_Pin_4);  // 蜂鸣器灭
            }
					memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
					USART_RX_STA = 0 ;
    }
}

}


// GPIO 初始化函数
void GPIO_Init_LEDs(void) {
    GPIO_InitTypeDef GPIO_InitStructure; // GPIO初始化结构体
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 开启GPIOA时钟

			

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10; // 配置Pb1和Pb10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      // 50MHz速度
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIO
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_4; // 配置PA8和PA4
	  GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO
	
}

