/*
 * DFR0646-G.h
 *
 *  Created on: Apr 16, 2023
 *      Author: Kuba
 */

#ifndef INC_DFR0646_G_H_
#define INC_DFR0646_G_H_

#include "stm32l4xx_hal.h"

//device_number range 0-3 (address 0xE0, 0xE2, 0xE4, 0xE6)
//brightness range 0-7
//seg range 0-7 - select one of 8 7-segment element to print on
//dot range 0-1 - decide if light dot on printing element

HAL_StatusTypeDef display_begin(I2C_HandleTypeDef *hi2c, uint8_t device_number);
void display_brightness(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t brightness);
HAL_StatusTypeDef display_print_seg(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t seg_num, uint8_t dot, uint8_t value);
void display_print_seg_all(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t* dots, uint8_t* values);
void display_print_two(I2C_HandleTypeDef *hi2c, uint8_t device_number, double value1, double value2);

#endif /* INC_DFR0646_G_H_ */
