/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

int monitor(uint8_t r1,uint8_t y1,uint8_t g1,uint8_t r2,uint8_t y2,uint8_t g2,uint8_t a1,uint8_t a2);
extern volatile uint32_t state1history;
typedef struct {
  uint8_t  vals0;
  uint8_t  vals1;
  uint8_t  vals2;
  uint8_t  vals3;
  uint8_t  vals4;
  uint8_t  vals5;
  uint8_t  vals6;
  uint8_t  vals7;
  uint8_t  vals8;
  uint8_t  vals9;
  uint8_t  vals10;
} state1apValuesType;
extern volatile state1apValuesType state1apValues;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Funktionen um die  CS/SK/DO Pins des Schieberegisters anzusprechen
#define WRITE_PIN_DISP_CS(v) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,v);
#define WRITE_PIN_DISP_CK(v) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,v);
#define WRITE_PIN_DISP_DO(v) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,v);

// Constanten für die 7-Segment-Anzeige
const uint8_t segmap [] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80 ,0x90, 
 0x88, 0x83,0xC6, 0xa1, 0x86,0x8e };
const uint8_t segsel [] = {0xf1, 0xf2, 0xf4, 0xf8};
const uint8_t dead [] = {0xa1, 0x86, 0x88, 0xa1 };

// Schiebt ein Byte in das Schieberegister rein
void displayShiftOut(uint8_t data) {
  for (int i=0;i<8;i++) {
    WRITE_PIN_DISP_DO(data & 128?GPIO_PIN_SET:GPIO_PIN_RESET);
    data = data << 1;
    WRITE_PIN_DISP_CK(GPIO_PIN_SET);
    volatile int m = 0;
    for (volatile int k=0;k<10;k++) { m+=k; } // Poor-Man's Delay
    WRITE_PIN_DISP_CK(GPIO_PIN_RESET);
  }
}

// 7-Segment Anzeige Leeren
void empty7SegDisplay() {
  WRITE_PIN_DISP_CS(GPIO_PIN_RESET);
  displayShiftOut(0xFF);
  displayShiftOut(0xFF);
  WRITE_PIN_DISP_CS(GPIO_PIN_SET);
}

// Dezimale Zahl Schreiben
void displayNum(uint16_t num) {
  for (uint8_t i=0;i<4;i++) {
    WRITE_PIN_DISP_CS(GPIO_PIN_RESET);
    displayShiftOut(segmap[num % 10]);
    displayShiftOut(segsel[3-i]);
    num = num / 10;
    WRITE_PIN_DISP_CS(GPIO_PIN_SET);
  }
  WRITE_PIN_DISP_CS(GPIO_PIN_RESET);
  displayShiftOut(0x00);
  WRITE_PIN_DISP_CS(GPIO_PIN_SET);
}

void displayHex(uint16_t num) {
  for (uint8_t i=0;i<4;i++) {
    WRITE_PIN_DISP_CS(GPIO_PIN_RESET);
    displayShiftOut(segmap[num % 16]);
    displayShiftOut(segsel[3-i]);
    num = num / 16;
    WRITE_PIN_DISP_CS(GPIO_PIN_SET);
  }
  WRITE_PIN_DISP_CS(GPIO_PIN_RESET);
  displayShiftOut(0x00);
  WRITE_PIN_DISP_CS(GPIO_PIN_SET);
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/** Monitoring **/

uint32_t currentAPValues = 0; // Atomic propositions for monitoring;
// Bits 0-2: Traffic Light A
// Bits 3-5: Traffic Light B
// Bits 6-7: Buttons A, B

void currentAPsTOUART(uint32_t currentAPs) {
  /*
    void currentAPsTOUART(uint32_t currentAPs);
    currentAPsTOUART(apValues);

  */
  char text[256];
  snprintf(text,256,"Monitor input %d\n",(int)currentAPs);
  HAL_UART_Transmit(&huart2,(uint8_t*)text,strlen(text),10000);
}


void lockEEPROM() {
  // From the reference manual
  while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
  FLASH->PECR |= FLASH_PECR_PELOCK; /* (2) */
}

void unlockEEPROM() {
// Unlock EEPROM. Code taken from the reference manual
  /* (1) Wait till no operation is on going */
  /* (2) Check if the PELOCK is unlocked */
  /* (3) Perform unlock sequence */
  while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (1) */
  {
  /* For robust implementation, add here time-out management */
  }
  if ((FLASH->PECR & FLASH_PECR_PELOCK) != 0) /* (2) */
  {
  FLASH->PEKEYR = FLASH_PEKEY1; /* (3) */
  FLASH->PEKEYR = FLASH_PEKEY2;
  }
}


void setLEDs(uint8_t val) {

  val = ~val;

  if (val & 1) {
    GPIOA->ODR |= GPIO_PIN_5;
  } else {
    GPIOA->ODR &= ~GPIO_PIN_5;
  }

  if (val & 2) {
    GPIOA->ODR |= GPIO_PIN_6;
  } else {
    GPIOA->ODR &= ~GPIO_PIN_6;
  }

  if (val & 4) {
    GPIOA->ODR |= GPIO_PIN_7;
  } else {
    GPIOA->ODR &= ~GPIO_PIN_7;
  }

  if (val & 8) {
    GPIOB->ODR |= GPIO_PIN_6;
  } else {
    GPIOB->ODR &= ~GPIO_PIN_6;
  }

}


void fatal_error() {
  // Flash all LEDs
  while(1) {
    uint32_t ticks = HAL_GetTick();
    setLEDs(15);
    while (HAL_GetTick()-ticks<500) {};
    setLEDs(0);
    while (HAL_GetTick()-ticks<1000) {};
  }
}


/* Traffic Light Display */
const uint8_t trafficSegMap [] = {0xff-80, 64+128, 130, 0x99 ,0xff-64};

void displayTL(uint8_t *modes) {
  for (uint8_t i=0;i<4;i++) {
    WRITE_PIN_DISP_CS(GPIO_PIN_RESET);
    
    // Sanity check, check APs
    //if (modes[i]>=4) fatal_error();
    #ifndef NO_MONITORING
    currentAPValues &= ~(3 << (2*i));
    currentAPValues |= modes[i] << (2*i);
    #endif
    displayShiftOut(trafficSegMap[modes[i]]);
    displayShiftOut(segsel[i]);
    WRITE_PIN_DISP_CS(GPIO_PIN_SET);
  }
}

const char *hexCodes = "0123456789ABCDEF";

void logViolationExplanation() {
  
  uint32_t *storedViolation = (uint32_t*)0x08080000;

  unlockEEPROM();
  *((uint32_t*)0x08080000) = state1history;
  for (unsigned int i=0;i<sizeof(state1apValuesType);i++) {
    *(((uint8_t*)0x08080004)+i) = *(((uint8_t*)&state1apValues)+i);
  }
  lockEEPROM();
  
  char text[256];
  int sizeBuffer = 64;
  snprintf(text,256,"Violation :\n");
  // Hex print
  int pos = strlen(text);
  for (uint32_t index=0;index<sizeBuffer;index++) {
    text[pos] = "0123456789ABCDEF"[((uint8_t*)storedViolation)[index]/16];
    text[pos+1] = "0123456789ABCDEF"[((uint8_t*)storedViolation)[index]%16];
    pos+=2;
    if (pos>250) fatal_error();
  }
  text[pos] = '\n';
  text[pos+1] = 0;
  HAL_UART_Transmit(&huart2,(uint8_t*)text,strlen(text),10000);
 }


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  // Print out current EEPROM content
  {
    const char *info = "\n===================[New Execution]============\nCurrent EEPROM Content:\n";
    HAL_UART_Transmit(&huart2,(uint8_t*)info,strlen(info),10000);
    char d[1027];
    for (unsigned int i=0;i<512;i++) {
      uint8_t data = *((uint8_t*)(0x08080000 + i));
      d[i*2] = hexCodes[data / 16];
      d[i*2+1] = hexCodes[data % 16];
    }
    d[1024] = '\n';
    d[1025] = '\n';
    d[1026] = 0;
    HAL_UART_Transmit(&huart2,(uint8_t*)d,1026,10000);
  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t modes[4] = {0,0,0,0};
  setLEDs(0);

  int currentMode = 0;
  uint32_t nextTick = HAL_GetTick()+1000;
  while (1) {
    
    // Buttons: PA4 PB0
    int buttonA = !(GPIOA->IDR & 2);
    int buttonB = !(GPIOA->IDR & 16);
    int buttonC = !(GPIOB->IDR & 1);

    // Update
    uint32_t currentTick = HAL_GetTick();

    // Faster cycle if button pressed
    if ((buttonA || (buttonB)) && (nextTick>currentTick+750)) {
      nextTick = currentTick+750;
    }

    // Standard update
    if (currentTick>nextTick) {
      // Update
      switch (currentMode) {
      case 0:
        // Red->Yellow(1)
        modes[0] = 1;
        modes[1] = 0;
        modes[2] = 1;
        modes[3] = 0;
        currentMode = 1;
        nextTick = currentTick+1000;
        break;
      case 1:
        // Yellow->Green
        modes[0] = 2;
        modes[1] = 0;
        modes[2] = 2;
        modes[3] = 0;
        if (!buttonA) 
          currentMode = 2;
        nextTick = currentTick+2000;
        break;
      case 2:
        // Green->Yellow
        modes[0] = 3;
        modes[1] = 0;
        modes[2] = 3;
        modes[3] = 0;
        if (buttonA) {
          currentMode = 6;
        } else {
          currentMode = 3;
        }
        nextTick = currentTick+1000;
        break;
      case 3:
        // Switch to Yellow
        modes[0] = 0;
        modes[1] = 1;
        modes[2] = 0;
        modes[3] = 1;
        currentMode = 4;
        nextTick = currentTick+1000;
        break;
      case 4:
        // Yellow->Green
        modes[0] = 0;
        modes[1] = 2;
        modes[2] = 0;
        modes[3] = 2;
        if (!buttonB) 
          currentMode = 5;
        nextTick = currentTick+2000;
        break;
      case 5:
        // Yellow->Green
        modes[0] = 0;
        modes[1] = 3;
        modes[2] = 0;
        modes[3] = 3;
        if (buttonB) {
          currentMode = 6;
        } else {
          currentMode = 0;
        }
        nextTick = currentTick+1000;
        break;
      case 6: 
        // All Red -> Button presses
        modes[0] = 0;
        modes[1] = 0;
        modes[2] = 0;
        modes[3] = 0;
        if (buttonB) {
          currentMode=3;
        } else {
          currentMode=0;
        }
        nextTick = currentTick+1000;
        break;
      }

      // Update APs & Monitor if not failed already!
      if (modes[0]!=4) {
        int monitorValue = monitor(modes[0]<=1,(modes[0]==1) || (modes[0]==3),modes[0]==2,modes[1]<=1,(modes[1]==1) || (modes[1]==3),modes[1]==2,buttonA,buttonB);

        if (monitorValue>0) {
          // Failure
          modes[0]=4;
          modes[1]=4;
          modes[2]=4;
          modes[3]=4;
          currentMode = 7;
          logViolationExplanation();
        }
      }
    }
    
    // Reset button
    if (buttonC) {
      unlockEEPROM();
      *((uint32_t*)0x08080000) = 0xFFFFFFFF;
      for (unsigned int i=0;i<64;i++) {
        *(((uint8_t*)0x08080004)+i) = 0;
      }
      lockEEPROM();
      fatal_error();
    }

    
    // Main loop: show
    displayTL(modes);
  }
  /* USER CODE END WHILE */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 PA8 
                           PA9 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
