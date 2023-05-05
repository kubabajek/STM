/*
 * ds18b20.h
 *
 *  Created on: May 1, 2023
 *      Author: Kuba
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stm32l4xx_hal.h"
#include "onewire.h"

#define DS18B20_ROM_CODE_SIZE		8
#define DS18B20_SCRATCHPAD_SIZE		9
#define DS18B20_READ_ROM			0x33 //only when one device on bus
#define DS18B20_MATCH_ROM			0x55 //after this send address of device
#define DS18B20_SKIP_ROM			0xCC //skip address, only when one device on bus
#define DS18B20_CONVERT_T			0x44 //start measurement
#define DS18B20_READ_SCRATCHPAD		0xBE //read measurement


HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code);
HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code);
HAL_StatusTypeDef ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad);
float ds18b20_get_temp(const uint8_t* rom_code);



#endif /* INC_DS18B20_H_ */
