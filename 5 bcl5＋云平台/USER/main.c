#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // 标准库头文件
#include <string.h>
#include <dht11.h>
#include <mq.h>
#include <oled.h>
#include "vl53l1x.h"
#include "bc25.h"

// Function prototypes
void GPIO_Init_LEDs(void); // 初始化GPIO
extern VL53L1_Dev_t VL53L1_dev[2];	//device param, include I2C
u8 temp, humi;
    u8 result;
		u8 n = 0;
		u8 m = 0;
		u8 j = 0;
		u8 buffer[100] = {0};
		char mq_str[10];
/*激光测距模块数据变量*/
		u8 speed[100] = {0};
		u8 mode = 0;		//激光雷达模式 mode: 0:default, 1:high accuracy, 2:long distance
		float data = 0;		//激光雷达数据暂存
		
		
int main(void) {
  	delay_init();   //延时函数初始化
    GPIO_Init_LEDs(); // 初始化led引脚       
		DHT11_init();			
		uart_init(9600);		//串口初始化
		uart_init_2(9600);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		OLED_Init();
		OLED_Clear();
		MQ_Init();
		BC25_GPIO_INIT();
		NB_RST();


	//	printf("AT+CGACT=0\r\n");
	
		OLED_ShowChinese(0,0,0,16);			//温
		OLED_ShowChinese(16,0,2,16);		//度
		OLED_ShowChinese(32,0,5,16);		//：
		OLED_ShowChinese(16*5,0,8,16);		//℃
		OLED_ShowChinese(0,16,1,16);		//湿
		OLED_ShowChinese(16,16,2,16);		//度
		OLED_ShowChinese(32,16,5,16);		//：
		OLED_ShowString(16*5,16,"\%RH",16);	//%RH
		OLED_ShowChinese(0,32,3,16);		//甲
		OLED_ShowChinese(16,32,4,16);		//烷
		OLED_ShowChinese(32,32,5,16);		//：
		OLED_ShowChinese(0,48,6,16);		//速
		OLED_ShowChinese(16,48,7,16);		//度
		OLED_ShowChinese(32,48,5,16);		//：
	
		/*激光雷达模块初始化*/
	while(VL53L1_init(&VL53L1_dev[0]))
	{
		printf("VL53L1_init_Error\n");
	}
	while(VL53L1_set_mode(&VL53L1_dev[0],mode))
	{
		printf("VL53L1_mode_Erro\n");
	}
	
	
	
		while(1)
	{		OLED_Refresh();	
//			printf("AT+CGACT=0\r\n");
        if (USART_RX_STA & 0x8000)
        {
            USART_RX_BUF[USART_RX_STA & 0x3FFF] = '\0'; // 添加字符串结束符
            printf("Received AT command: %s\n", USART_RX_BUF); // 处理接收到的AT指令

            USART_RX_STA = 0; // 清除接收状态
//			n++;
//		printf("AT+CGACT=0\r\n");
//		     delay_ms(500);
//		printf("AT+CIMI\r\n");
//					delay_ms(500);
//		if(n == 10){
//        // 读取DHT11	数据
//				n = 0;
//        result = DHT11_Read_Data(&temp, &humi);
//        if (result == 0)
//        {
//            // 打印温度和湿度
//            USART_SendString("Temperature: ");
//            USART_SendNumber(temp);
//            USART_SendString("C, Humidity: ");
//            USART_SendNumber(humi);
//            USART_SendString("%\r\n");
//        }
//        else
//        {
//            USART_SendString("无法读取数据!\r\n");
//        }

//        // 延时一段时间再读取
//        delay_ms(2000);

//		printf("%d\n",ADC_Data());
//		delay_ms(1000);
//		
//		sprintf(mq_str, "%d", ADC_Data());
//			data = (float)Distance;
//			if(VL53L1_single_test(&VL53L1_dev[0],&VL53L1_data[0])==VL53L1_Error_NONE)
//		{
//			data = (data - (float)Distance);
//			if(data>0)
//			{
//				sprintf((char*)speed,"%.2fkm/h",data*3.6/100);
//			}
//		}
//			OLED_ShowNum(48,0,temp,2,16);		//温度
//			OLED_ShowNum(48,16,humi,2,16);		//湿度
//			OLED_ShowString(48,32,(unsigned char *)mq_str,16);	//甲烷
//			OLED_ShowString(48,48,speed,16);	//速度
//		
//		
//			m++;
//			if(m == 10)
//			{
//				m = 0;
//				
////				j = !j;
////				PBout(0) = j;
////				printf("AT+CGSN=1\r\n");
////				delay_ms(500); 
////				//发送温度
////				sprintf((char *)buffer,"{\"temp\":\"%d\"}",temp);
////				printf("AT+NMGS=%d,",strlen((char *)buffer));
////				for(int i=0; i < strlen((char *)buffer); i++)
////				{
////					printf("%x",buffer[i]);
////					delay_ms(15); 
////				}
////				printf("\r\n");
////				delay_ms(100);
////				sprintf((char *)buffer,"{\"humi\":\"%d\"}",humi);
////				printf("AT+NMGS=%d,",strlen ((char *)(char *)buffer));
////				for(int i=0;i < strlen ((char *)buffer); i++)
////				{
////					printf("%x",buffer[i]);
////					delay_ms(15); 
////				}
////				printf("\r\n");
//			}
//		
//    }

		

	}

}



// GPIO 初始化函数
//void GPIO_Init_LEDs(void) {
//    GPIO_InitTypeDef GPIO_InitStructure; // GPIO初始化结构体
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 开启GPIOA时钟

//			

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10; // 配置Pb1和Pb10
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       // 推挽输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      // 50MHz速度
//    GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIO
//	
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_4; // 配置PA8和PA4
//	  GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO
//	
//}

