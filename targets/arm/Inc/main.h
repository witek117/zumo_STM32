/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PG_3V3_Pin GPIO_PIN_13
#define PG_3V3_GPIO_Port GPIOC
#define SENS_IR_LED_Pin GPIO_PIN_14
#define SENS_IR_LED_GPIO_Port GPIOC
#define TRIG_Pin GPIO_PIN_15
#define TRIG_GPIO_Port GPIOC
#define ECHO_Pin GPIO_PIN_0
#define ECHO_GPIO_Port GPIOC
#define ECHO_EXTI_IRQn EXTI0_IRQn
#define SENS8_Pin GPIO_PIN_1
#define SENS8_GPIO_Port GPIOC
#define SENS7_Pin GPIO_PIN_2
#define SENS7_GPIO_Port GPIOC
#define SENS6_Pin GPIO_PIN_3
#define SENS6_GPIO_Port GPIOC
#define SENS5_Pin GPIO_PIN_0
#define SENS5_GPIO_Port GPIOA
#define SENS5A1_Pin GPIO_PIN_1
#define SENS5A1_GPIO_Port GPIOA
#define SENS3_Pin GPIO_PIN_4
#define SENS3_GPIO_Port GPIOA
#define SENS2_Pin GPIO_PIN_5
#define SENS2_GPIO_Port GPIOA
#define SENS1_Pin GPIO_PIN_6
#define SENS1_GPIO_Port GPIOA
#define TEMP_Pin GPIO_PIN_7
#define TEMP_GPIO_Port GPIOA
#define WS2812B_Pin GPIO_PIN_4
#define WS2812B_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOC
#define V_CURRENT_SENS_Pin GPIO_PIN_0
#define V_CURRENT_SENS_GPIO_Port GPIOB
#define V_BAT_Pin GPIO_PIN_1
#define V_BAT_GPIO_Port GPIOB
#define V_5V_Pin GPIO_PIN_2
#define V_5V_GPIO_Port GPIOB
#define AIN1_Pin GPIO_PIN_10
#define AIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_11
#define AIN2_GPIO_Port GPIOB
#define SHDN_MOTOR_TPS_Pin GPIO_PIN_12
#define SHDN_MOTOR_TPS_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_13
#define BIN1_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_14
#define BIN2_GPIO_Port GPIOB
#define FAULT_Pin GPIO_PIN_15
#define FAULT_GPIO_Port GPIOB
#define DIP_SW_Pin GPIO_PIN_6
#define DIP_SW_GPIO_Port GPIOC
#define PG_5V_Pin GPIO_PIN_7
#define PG_5V_GPIO_Port GPIOC
#define INT_CHARGER_Pin GPIO_PIN_8
#define INT_CHARGER_GPIO_Port GPIOC
#define PG_3V3_ESP_Pin GPIO_PIN_9
#define PG_3V3_ESP_GPIO_Port GPIOC
#define EN_PER_ESP_Pin GPIO_PIN_8
#define EN_PER_ESP_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOA
#define PG_6V_Pin GPIO_PIN_12
#define PG_6V_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_15
#define SW1_GPIO_Port GPIOA
#define INT_BHI_Pin GPIO_PIN_12
#define INT_BHI_GPIO_Port GPIOC
#define INT_MPU_9250_Pin GPIO_PIN_2
#define INT_MPU_9250_GPIO_Port GPIOD
#define INT_MPU6050_Pin GPIO_PIN_3
#define INT_MPU6050_GPIO_Port GPIOB
#define MOT_L_A_Pin GPIO_PIN_4
#define MOT_L_A_GPIO_Port GPIOB
#define MOT_L_B_Pin GPIO_PIN_5
#define MOT_L_B_GPIO_Port GPIOB
#define MOT_R_A_Pin GPIO_PIN_6
#define MOT_R_A_GPIO_Port GPIOB
#define MOT_R_B_Pin GPIO_PIN_7
#define MOT_R_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
