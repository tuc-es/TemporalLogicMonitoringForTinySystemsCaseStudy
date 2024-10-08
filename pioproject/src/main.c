#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

// Monitoring declaration & macros
int monitor(uint8_t r1,uint8_t y1,uint8_t g1,uint8_t r2,uint8_t y2,uint8_t g2,uint8_t a1,uint8_t a2);
#define CHECKMON(STEP,VAL) if (failed!=VAL) { __NOP(); }

// Main function
int main(void) {
    
  // Init computation cycles counting component.
  *((volatile uint32_t*)0xE000EDFC) |= (1 << 24);
  *((volatile uint32_t*)0xE0001000) |= 1;
  *((volatile uint32_t*)0xE0001004) = 0;
  
  // Run the monitor with an example trace
  int failed = 0;
  failed = monitor(1,0,0,1,0,0,0,0);
  CHECKMON(1,0)
  failed = monitor(1,1,0,1,0,0,0,0);
  CHECKMON(2,0)
  failed = monitor(0,0,1,1,0,0,0,0);
  CHECKMON(3,0)
  failed = monitor(0,1,0,1,0,0,0,0);
  CHECKMON(4,0)
  failed = monitor(1,0,0,1,1,0,0,0);
  CHECKMON(5,0)
  failed = monitor(1,0,0,0,0,1,0,0);
  CHECKMON(5,0)
  failed = monitor(0,0,1,0,0,1,0,0);
  CHECKMON(6,1)
  
  // Ok, now stop counting the clock cycles and subtract a constant
  // representing the computation time needed in case of an empty monitor
  // to get the real monitoring cost.
  uint32_t nofCycles = *((volatile uint32_t*)0xE0001004) - 3; // Subtract previously measured base cost.
   
  // Ok, now we counted the clock cycles -- time to communicate them to the host.
  // 1. Init GPIO ports -- Code generated by STM32CubeMX
  HAL_Init();
  UART_HandleTypeDef huart2;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_USART2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  // 2. Init USART2, which is connected to the PC-interface on the ST Nucleo board -- Code generated by STM32CubeMX
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    // In case of failure, just stop here -- can be found with a debugger.
    while(1) {};
  }
  
  // Build the message for the host PC, but delay by a second to make sure
  // that the host PC has started the tool to read the number of cycles.
  HAL_Delay(1000);
  char msg[256];
  snprintf(msg,256,"#Cycles: %ld\n",nofCycles);
  HAL_UART_Transmit(&huart2, (void*)msg, strlen(msg), 1000);
  
  // End of execution. Do nothing until a reset.  
  while (1) {
  }
  

}




void SysTick_Handler(void) {
  HAL_IncTick();
}

