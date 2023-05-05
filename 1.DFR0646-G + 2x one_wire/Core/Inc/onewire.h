/*
 * onewire.h
 *
 *  Created on: 3 maj 2023
 *      Author: Kuba
 */

#ifndef INC_ONEWIRE_H_
#define INC_ONEWIRE_H_

#include "stm32l4xx_hal.h"

extern UART_HandleTypeDef huart1;

HAL_StatusTypeDef wire_reset(void);
uint8_t wire_read(void);
void wire_write(uint8_t byte);

#endif /* INC_ONEWIRE_H_ */
