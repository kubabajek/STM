/*
 * ds18b20.c
 *
 *  Created on: May 1, 2023
 *      Author: Kuba
 */

#include "ds18b20.h"


static uint8_t byte_crc(uint8_t crc, uint8_t byte){
  for (int i = 0; i < 8; i++) {
	uint8_t b = crc ^ byte;
	crc >>= 1;
	if (b & 0x01)
	  crc ^= 0x8c;
	byte >>= 1;
  }
  return crc;
}


static uint8_t wire_crc(const uint8_t* data, int len){
  int i;
	uint8_t crc = 0;
	for (i = 0; i < len; i++)
	  crc = byte_crc(crc, data[i]);
	return crc;
}


static HAL_StatusTypeDef send_cmd(const uint8_t* rom_code, uint8_t cmd){
  int i;

  if (wire_reset() != HAL_OK)
	return HAL_ERROR;

  if (!rom_code) {
	wire_write(DS18B20_SKIP_ROM);
  } else {
	wire_write(DS18B20_MATCH_ROM);
	for (i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
	  wire_write(rom_code[i]);
  }
  wire_write(cmd);
  return HAL_OK;
}


HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code){
  uint8_t crc;

  if (wire_reset() != HAL_OK)
	return HAL_ERROR;

  wire_write(DS18B20_READ_ROM);

  for (int i = 0; i < DS18B20_ROM_CODE_SIZE; i++){
	rom_code[i] = wire_read();
  }

  crc = wire_crc(rom_code, DS18B20_ROM_CODE_SIZE - 1);
  if (rom_code[DS18B20_ROM_CODE_SIZE - 1] == crc)
	return HAL_OK;
  else
	return HAL_ERROR;
}


HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code){
  return send_cmd(rom_code, DS18B20_CONVERT_T);
}


HAL_StatusTypeDef ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad){
  int i;
  uint8_t crc;

  if (send_cmd(rom_code, DS18B20_READ_SCRATCHPAD) != HAL_OK)
	return HAL_ERROR;

  for (i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++)
	scratchpad[i] = wire_read();

  crc = wire_crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1);
  if (scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc)
	return HAL_OK;
  else
	return HAL_ERROR;
}

float ds18b20_get_temp(const uint8_t* rom_code){
  uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
  int16_t temp;

  if (ds18b20_read_scratchpad(rom_code, scratchpad) != HAL_OK)
	return 85.0f;

  memcpy(&temp, &scratchpad[0], sizeof(temp));

  return temp / 16.0f;
}
