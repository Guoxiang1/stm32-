#include "mq.h"


void MQ_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOAʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);				//ADCʱ��6��Ƶ
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//GPIOA.1
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct );

	ADC_DeInit(ADC1);
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;		//����ת��
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		//�����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;			//����ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;				//��ɨ��
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1,ENABLE);					//ʹ��ADC1
	
	ADC_ResetCalibration(ADC1);				//��λУ��
	
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);					//��ʼУ��
	
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		//��ʼת��
}

u16 ADC_Data(void)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		//��ʼת��
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	
	return ADC_GetConversionValue(ADC1);
}

