/*
 * onewire.c
 *
 *  Created on: 3 maj 2023
 *      Author: Kuba
 */

#include "onewire.h"

static void set_baudrate(uint32_t baudrate){
	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = baudrate;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
	  huart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
	  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
		Error_Handler();
}

HAL_StatusTypeDef wire_reset(void){
	uint8_t data_out = 0xF0;
	uint8_t data_in;
	set_baudrate(9600);
	HAL_UART_Transmit(&huart1, &data_out, 1, HAL_MAX_DELAY);
	HAL_UART_Receive(&huart1, &data_in, 1, HAL_MAX_DELAY);
	set_baudrate(115200);
	return (data_in != 0xF0)? HAL_OK : HAL_ERROR;
}

static int read_bit(void){
  uint8_t data_out = 0xFF;
  uint8_t data_in = 0;
  HAL_UART_Transmit(&huart1, &data_out, 1, HAL_MAX_DELAY);
  HAL_UART_Receive(&huart1, &data_in, 1, HAL_MAX_DELAY);

  return data_in & 0x01;
}

uint8_t wire_read(void){
  uint8_t value = 0;
  for (int i = 0; i < 8; i++) {
	value >>= 1;
	if (read_bit())
	  value |= 0x80;
  }
  return value;
}

static void write_bit(int value){
	if (value) {
	  uint8_t data_out = 0xff;
	  HAL_UART_Transmit(&huart1, &data_out, 1, HAL_MAX_DELAY);
	}
	else {
	  uint8_t data_out = 0x0;
	  HAL_UART_Transmit(&huart1, &data_out, 1, HAL_MAX_DELAY);
	}
}

void wire_write(uint8_t byte){
  for (int i = 0; i < 8; i++) {
	write_bit(byte & 0x01);
	byte >>= 1;
  }
}
