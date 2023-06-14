/**
 * @file
 * @author Bajek
 * @brief Header file for DFR0646-G driver.
 * @n At present, it only supports showing two double values from range -999 - 999
 * @n Can also light seperetly 7-segment sector with the numbers 0 to 9, capital letters A, B, C, D, E, F, H, L, O, P, U and dash-,
 * */

/*
 * DFR0646-G Driver is a device driver for DFR0646-G segment display (8bits digital tube version).
 * Copyright (C) 2022  Bajek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see:
 * <https://www.gnu.org/licenses/>.
 * */


#ifndef INC_DFR0646_G_H_
#define INC_DFR0646_G_H_

#include "stm32l4xx_hal.h"

  /**
   * @fn display_begin
   * @brief Turn on the clock and display
   * @param hi2c I2C Handler to which display is connected
   * @device_number Device number in range 0-3 (uint8_t) which coressponds to device addresses 0xE0, 0xE2, 0xE4, 0xE6
   * @return Status of whether the data was successfully sent (HAL_StatusTypeDef)
   */
HAL_StatusTypeDef display_begin(I2C_HandleTypeDef *hi2c, uint8_t device_number);

/**
 * @fn display_brightness
 * @brief Change brightness of whole display
 * @param hi2c I2C Handler to which display is connected
 * @param device_number Device number in range 0-3 (uint8_t) which coressponds to device addresses 0xE0, 0xE2, 0xE4, 0xE6
 * @param brightness Value of brightness in range 0-7 (uint8_t)
 * @return Status of whether the data was successfully sent (HAL_StatusTypeDef)
 */
HAL_StatusTypeDef display_brightness(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t brightness);

/**
 * @fn display_print_seg
 * @brief Display on one of eight poles
 * @param hi2c I2C Handler to which display is connected
 * @param device_number Device number in range 0-3 (uint8_t) which coressponds to device addresses 0xE0, 0xE2, 0xE4, 0xE6
 * @param seg_num Number of selected sector in range 0-7 (uint8_t)
 * @param dot 1 for display dot, 0 for not (uint8_t)
 * @param value Number of symbol to display from "Display Data" in range 0-22 (uint8_t)
 * @return Status of whether the data was successfully sent (HAL_StatusTypeDef)
 */
HAL_StatusTypeDef display_print_seg(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t seg_num, uint8_t dot, uint8_t value);

/**
 * @fn display_print_seg
 * @brief Display all 8 values
 * @param hi2c I2C Handler to which display is connected
 * @param device_number Device number in range 0-3 (uint8_t) which coressponds to device addresses 0xE0, 0xE2, 0xE4, 0xE6
 * @param dots Tab of 8 values 0/1 to display dots (uint8_t*)
 * @param values Tab of 8 values 0-22 with data to display (uint8_t*)
 * @return None
 */
void display_print_seg_all(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t* dots, uint8_t* values);


/**
 * @fn display_print_two
 * @brief Display two double values (in range -999 to 999 to show full number)
 * @param hi2c I2C Handler to which display is connected
 * @param device_number Device number in range 0-3 (uint8_t) which coressponds to device addresses 0xE0, 0xE2, 0xE4, 0xE6
 * @param value1 First number to display on left 4 segments (double)
 * @param value2 Seconf number to display on right 4 segments (double)
 * @return None
 */
void display_print_two(I2C_HandleTypeDef *hi2c, uint8_t device_number, double value1, double value2);


//Display Data
// 0-9, A-F(10-15), H(16), L(17), O(18), P(19), U(20), -(21),  (22)
static const uint8_t TAB1[23]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
                              0x77,0x7C,0x39,0x5E,0x79,0x71,
                              0x76, 0x38, 0x5C, 0x73, 0x3E, 0x40,0x00};
//The same symbols with dot
static const uint8_t TAB2[23]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,
                                    0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,
                                    0xF6,0xB8,0xDC,0xF3,0xBE,0xC0,0x00};

#endif /* INC_DFR0646_G_H_ */
