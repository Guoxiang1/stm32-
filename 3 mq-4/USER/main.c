#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // 标准库头文件
#include <string.h>
#include <dht11.h>
#include <mq.h>
#include <oled.h>


// Function prototypes
void GPIO_Init_LEDs(void); // 初始化GPIO
u8 temp, humi;
    u8 result;

int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // 初始化led引脚       

		uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

		MQ_Init();
		while(1)
	{
		printf("%d\n",ADC_Data());
		delay_ms(1000);
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

