/**
  ******************************************************************************
  * @file           : bldc_def.h
  ******************************************************************************/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LASER_DEF_H__
#define __LASER_DEF_H__

//#include "stm32f0xx_hal_def.h"
//#include "wc_def.h"
#include "stm32f10x.h"
/* USER CODE BEGIN Private defines */
#define XShut2_Pin GPIO_Pin_3
#define XShut2_GPIO_Port GPIOB
#define XShut_Pin GPIO_Pin_15
#define XShut_GPIO_Port GPIOA
/*#define LED_Pin GPIO_Pin_6
#define LED_GPIO_Port GPIOA
#define LED2_Pin GPIO_Pin_1
#define LED2_GPIO_Port GPIOB*/

#define XShut_On		    GPIO_SetBits(XShut_GPIO_Port,XShut_Pin)
#define XShut_Off			GPIO_ResetBits(XShut_GPIO_Port,XShut_Pin)
#define XShut2_On			GPIO_SetBits(XShut2_GPIO_Port, XShut2_Pin)
#define XShut2_Off		    GPIO_ResetBits(XShut2_GPIO_Port, XShut2_Pin)
/*define LED_On				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define LED_Off				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LED2_On				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2_Off			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)*/

/* USER CODE END Private defines */

#endif /* __LASER_DEF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
