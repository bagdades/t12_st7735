/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
// #include "board.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
// #include "rotary_encoder.h"
#include <malloc.h>
#include "gui.h"
#include "fonts.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

// This is left here just to have it handy for copying when debugging a specific function
// Don't uncomment!!
//               __attribute__((optimize("O0")))

#define DEBUG_ERROR

#ifdef DEBUG
extern struct mallinfo mi;
#define DEBUG_ALLOC                   // Enable alloc debugging in debug builds
#endif

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Program_Handler(void);
/*
 * Macro to enable debugging of the allocated memory
 * max_allocated will hold the max used memory at any time
 *
 */
#ifdef DEBUG_ALLOC
extern uint32_t max_allocated;
#define dbg_mem() mi=mallinfo();                  \
                  if(mi.uordblks>max_allocated){  \
                    max_allocated=mi.uordblks;     \
                  }                               \

#define _malloc(x)    malloc(x); dbg_mem()
#define _calloc(x,y)  calloc(x,y); dbg_mem()
#define _free(x)      free(x); dbg_mem()

#else
#define _malloc     malloc
#define _calloc     calloc
#define _free       free
#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_SCL_Pin GPIO_PIN_13
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_14
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_15
#define LCD_SDA_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_8
#define LCD_RST_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_9
#define LCD_DC_GPIO_Port GPIOA
#define ENC_SW_Pin GPIO_PIN_15
#define ENC_SW_GPIO_Port GPIOA
#define ENC_L_Pin GPIO_PIN_3
#define ENC_L_GPIO_Port GPIOB
#define ENC_R_Pin GPIO_PIN_4
#define ENC_R_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
