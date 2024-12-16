#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // ��׼��ͷ�ļ�
#include <string.h>
#include <dht11.h>
#include <mq.h>
#include <oled.h>


// Function prototypes
void GPIO_Init_LEDs(void); // ��ʼ��GPIO
u8 temp, humi;
    u8 result;
		u8 n = 0;
		char mq_str[10];
int main(void) {
  	delay_init();
    GPIO_Init_LEDs(); // ��ʼ��led����       
		DHT11_init();
		uart_init(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		OLED_Init();
		OLED_Clear();
		MQ_Init();
	
		OLED_ShowChinese(0,0,0,16);			//��
		OLED_ShowChinese(16,0,2,16);		//��
		OLED_ShowChinese(32,0,5,16);		//��
		OLED_ShowChinese(16*5,0,8,16);		//��
		OLED_ShowChinese(0,16,1,16);		//ʪ
		OLED_ShowChinese(16,16,2,16);		//��
		OLED_ShowChinese(32,16,5,16);		//��
		OLED_ShowString(16*5,16,"\%RH",16);	//%RH
		OLED_ShowChinese(0,32,3,16);		//��
		OLED_ShowChinese(16,32,4,16);		//��
		OLED_ShowChinese(32,32,5,16);		//��
		OLED_ShowChinese(0,48,6,16);		//��
		OLED_ShowChinese(16,48,7,16);		//��
		OLED_ShowChinese(32,48,5,16);		//��
	
	
		while(1)
	{		OLED_Refresh();	
			n++;
		if(n == 10){
        // ��ȡDHT11	����
				n = 0;
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

		printf("%d\n",ADC_Data());
		delay_ms(1000);

		sprintf(mq_str, "%d", ADC_Data());
			OLED_ShowNum(48,0,temp,2,16);		//�¶�
			OLED_ShowNum(48,16,humi,2,16);		//ʪ��
			OLED_ShowString(48,32,(unsigned char *)mq_str,16);	//����
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

