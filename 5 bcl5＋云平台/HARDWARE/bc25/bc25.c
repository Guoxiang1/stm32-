#include "bc25.h"
#include "usart.h"	
#include "delay.h"
#include "string.h"
#include "stm32f10x.h"

#define BC25_ON_GPIO GPIO_Pin_14  //供电引脚，PB14
#define BC25_ON PBout(14)

#define BC25_REST_GPIO GPIO_Pin_15 //复位引脚，PB15
#define BC25_REST PBout(15)

#define BC25_REST PBout(15)

#define NB_receive_buf USART_RX_BUF_2

#define NBSTATUS(n) LED3=n

#define HAL_Delay delay_ms

#define NBSTATUS_TOGGLE LED3=!LED3

void BC25_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = BC25_ON_GPIO;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,BC25_ON_GPIO);	//供电引脚
	
	GPIO_InitStructure.GPIO_Pin = BC25_REST_GPIO;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,BC25_REST_GPIO);		//供电引脚

}

void NB_RST(void)
{
    BC25_REST=1;
	delay_ms(1200);	//1s
	BC25_REST=0;
}



void BC25_QSCLK(void)
{
	u8 i = 0;	
	//NB禁用休眠
	printf("AT+QSCLK=0\r\n"); //禁止NB进入休眠
	delay_ms(100);
	char *strx = strstr((const char *)USART_RX_BUF_2, (const char *)"OK");//不同返回NULL

	while (strx == NULL)//不是OK
    {
		i++;
		USART_RX_STA_2 = 0;
		printf("AT+QSCLK=0\r\n"); //禁止NB进入休眠
        delay_ms(200);
        strx = strstr((const char *)USART_RX_BUF_2, (const char *)"OK");

		if(i%100 == 0)
			NB_RST();		
	}
	
}

void BC25_CPSMS(void)
{
	u8 i = 0;	
	//禁止进入PSM
	printf("AT+CPSMS=0\r\n"); 
	delay_ms(300);
	char *strx = strstr((const char *)USART_RX_BUF_2, (const char *)"OK");//不同返回NULL

	while (strx == NULL)//不是OK
    {
		i++;
		USART_RX_STA_2 = 0;
		printf("AT+CPSMS=0\r\n"); 
        delay_ms(300);
        strx = strstr((const char *)USART_RX_BUF_2, (const char *)"OK");

		if(i%100 == 0)
			NB_RST();		
	}
	
}

void BC25_NNMI(void)//以只吐模式登录电信平台
{
	u8 i = 0;	
	USART_RX_STA_2 = 0;
	printf("AT+NNMI=1\r\n");  //以直吐模式注册到电信平台 
	HAL_Delay(500);
	char *strx = strstr((const char *)NB_receive_buf, (const char *)"OK");
    while (strx == NULL)
    {
		i++;
        USART_RX_STA_2 = 0;
		printf("AT+NNMI=1\r\n");  //以直吐模式注册到电信平台
        HAL_Delay(500);

        strx = strstr((const char *)NB_receive_buf, (const char *)"OK");
		if(i%100 == 0)
			NB_RST();
    }		
	
}
void BC25_CIMI(void) //获得NB卡号
{
	
	u8 i = 0;	
	USART_RX_STA_2 = 0;
	printf("ATE1\r\n");  //关闭回显
	HAL_Delay(500);HAL_Delay(500);HAL_Delay(500);
    printf("AT+CIMI\r\n"); //获取卡号，是否存在卡
    HAL_Delay(700);HAL_Delay(500);
    char *strx = strstr((const char *)NB_receive_buf, (const char *)"460"); //返回OK，说明卡是存在的
    while (strx == NULL)//未获取卡号
    {
		i++;
       // USART_RX_STA_2 = 0;
		printf("AT+CIMI\r\n");  //以直吐模式注册到电信平台
        HAL_Delay(500);

        strx = strstr((const char *)NB_receive_buf, (const char *)"460");
		if(i%100 == 0)
			NB_RST();	
    }
	
}
void BC25_CGATT(void) //附着
{
	u8 i = 0;	
	USART_RX_STA_2 = 0;
    printf("AT+CGATT?\r\n");  //发送附着命令
    HAL_Delay(900);
    char *strx = strstr((const char *)NB_receive_buf, (const char *)"+CGATT: 1");
    while (strx == NULL)//未附着成功
    {
        i++;
        USART_RX_STA_2 = 0;
		printf("AT+CGATT?\r\n");  
        HAL_Delay(600);

        strx = strstr((const char *)NB_receive_buf, (const char *)"+CGATT: 1");
		if(i%100 == 0)
			NB_RST();
		
    }
		
}
void BC25_CGPADDR(void)//查询是否注网
{
	u8 i = 0;	
	USART_RX_STA_2 = 0;
	printf("AT+QCCID\r\n"); //取得设备号
    HAL_Delay(500);
	printf("AT+CGPADDR?\r\n");        //查询模块是否激活cid
	char *strx = strstr((const char *)NB_receive_buf, (const char *)"+CGPADDR: 1");
    while (strx == NULL)
    {
		i++;
        USART_RX_STA_2 = 0;
		printf("AT+CGPADDR?\r\n");  //以直吐模式注册到电信平台
        HAL_Delay(500);

        strx = strstr((const char *)NB_receive_buf, (const char *)"+CGPADDR: 1");
		if(i%100 == 0)
			NB_RST();		
		//查询是否注网
	}
}
uint8_t BC25_INIT(void)
{
	u8 t = 0;
	
	BC25_QSCLK();//禁止NB进入休眠
		
	BC25_CPSMS();//禁止进入PSM
	
	BC25_CIMI();//获得NB卡号	
	
	BC25_NNMI();//以只吐模式登录电信平台
		
	BC25_CGATT();//激活网络状态	
	
	BC25_CGPADDR();	//查询是否注网
	
	USART_RX_STA_2 = 0;
	//连接电信云平台
	printf("AT+NCFG=0,86400\r\n");//设置lifetime时间
	HAL_Delay(500);  
	printf("AT+NCDPOPEN=\"221.229.214.202\",5683\r\n"); // 连接电信云AT+NCDPOPEN="221.229.214.202",5683
	HAL_Delay(500);
	//    mqtt_overtime_flag = 0;
	//    mqtt_tim_start = 1; //开启超时计数
	//    HAL_Delay(100);
	char *strx = strstr((const char *)NB_receive_buf, (const char *)"+QLWEVTIND: 3"); //模块连接电信云成功	
	while(strx == NULL)
	{  
		t++;
		USART_RX_STA_2 = 0;
		printf("AT+NCDPOPEN=\"221.229.214.202\",5683\r\n"); // 连接电信云AT+NCDPOPEN="221.229.214.202",5683
		HAL_Delay(1000);

		strx = strstr((const char *)NB_receive_buf, (const char *)"+QLWEVTIND: 3"); //模块连接电信云成功			
		if(t%100 == 0)
			NB_RST();			
	}
	//连接电信云平台	
	return 0;
}







