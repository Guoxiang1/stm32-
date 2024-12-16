#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // 标准库头文件

// Function prototypes
void GPIO_Init_LEDs(void); // 初始化GPIO

int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // 初始化PA6和PA7

    while (1) {
        // 控制LED1亮，LED2灭
	  	  GPIO_SetBits(GPIOA, GPIO_Pin_4); 
			
        GPIO_SetBits(GPIOB, GPIO_Pin_10);    // 点亮LED1
        GPIO_SetBits(GPIOB, GPIO_Pin_1); // 熄灭LED2
				delay_ms(300);                // 延时

        // 控制LED1灭，LED2亮
		  	GPIO_ResetBits(GPIOA, GPIO_Pin_4);  
        GPIO_ResetBits(GPIOB, GPIO_Pin_10); // 熄灭LED1
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);   // 点亮LED2
        delay_ms(500);                   // 延时
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

