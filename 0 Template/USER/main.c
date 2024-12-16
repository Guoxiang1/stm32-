#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // ��׼��ͷ�ļ�

// Function prototypes
void GPIO_Init_LEDs(void); // ��ʼ��GPIO

int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // ��ʼ��PA6��PA7

    while (1) {
        // ����LED1����LED2��
	  	  GPIO_SetBits(GPIOA, GPIO_Pin_4); 
			
        GPIO_SetBits(GPIOB, GPIO_Pin_10);    // ����LED1
        GPIO_SetBits(GPIOB, GPIO_Pin_1); // Ϩ��LED2
				delay_ms(300);                // ��ʱ

        // ����LED1��LED2��
		  	GPIO_ResetBits(GPIOA, GPIO_Pin_4);  
        GPIO_ResetBits(GPIOB, GPIO_Pin_10); // Ϩ��LED1
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);   // ����LED2
        delay_ms(500);                   // ��ʱ
    }
}

// GPIO ��ʼ������
void GPIO_Init_LEDs(void) {
    GPIO_InitTypeDef GPIO_InitStructure; // GPIO��ʼ���ṹ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// ����GPIOAʱ��

			

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10; // ����Pb1��Pb10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      // 50MHz�ٶ�
    GPIO_Init(GPIOB, &GPIO_InitStructure); // ��ʼ��GPIO
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_4; // ����PA8��PA4
	  GPIO_Init(GPIOA, &GPIO_InitStructure); // ��ʼ��GPIO
}

