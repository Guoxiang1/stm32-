#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // ��׼��ͷ�ļ�
#include <string.h>
#include <dht11.h>

// Function prototypes
void GPIO_Init_LEDs(void); // ��ʼ��GPIO

int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // ��ʼ��led����       
		DHT11_init();
		uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		PBout(1)=1;
		PBout(10)=1;
	
    while (1) {
			if(USART_RX_STA & 0x8000)
				{
					printf("���գ�%s\n",USART_RX_BUF);
           		 if (strstr((const char *)USART_RX_BUF, "��led1")) {
						    GPIO_ResetBits(GPIOB, GPIO_Pin_10); // LED1��
                GPIO_ResetBits(GPIOB, GPIO_Pin_1);  // LED2��
                GPIO_SetBits(GPIOA, GPIO_Pin_4);  // ��������
           		 } 
							 
							 
							 else {
                GPIO_SetBits(GPIOB, GPIO_Pin_10); // LED1��
                GPIO_SetBits(GPIOB, GPIO_Pin_1);  // LED2��
                GPIO_ResetBits(GPIOA, GPIO_Pin_4);  // ��������
            }
					memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
					USART_RX_STA = 0 ;
    }
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

