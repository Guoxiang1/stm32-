#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // ��׼��ͷ�ļ�
#include <string.h>
#include <dht11.h>

// Function prototypes
void GPIO_Init_LEDs(void); // ��ʼ��GPIO
u8 temp, humi;
    u8 result;

int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // ��ʼ��led����       
		DHT11_init();
		uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


	
    result = DHT11_init();
    if (result != 0)
    {
        USART_SendString("DHT11��ʼ��ʧ��!\r\n");
        while (1);
    }

		
    while (1)
    {
        // ��ȡDHT11����
        result = DHT11_Read_Data(&temp, &humi);
        if (result == 0)
        {
            // ��ӡ�¶Ⱥ�ʪ��
            USART_SendString("Temperature: ");
            USART_SendNumber(temp);
            USART_SendString("C, Humidity: ");
            USART_SendNumber(humi);
            USART_SendString("%\r\n");
        }
        else
        {
            USART_SendString("�޷���ȡ����!\r\n");
        }

        // ��ʱһ��ʱ���ٶ�ȡ
        delay_ms(2000);
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

