



#include "DFR0646-G.h"

static const uint8_t address[4] = {0xE0, 0xE2, 0xE4, 0xE6};
static const uint8_t cmd[3] = {0x21, 0x81, 0xEE};
static const uint8_t brightness_tab[8] = {0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8};
static const uint8_t seg[8] = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e};

/// 0-9, A-F(10-15), H(16), L(17), O(18), P(19), U(20), -(21),  (22)
static const uint8_t TAB1[23]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
                              0x77,0x7C,0x39,0x5E,0x79,0x71,
                              0x76, 0x38, 0x5C, 0x73, 0x3E, 0x40,0x00};
//symbols with dot
static const uint8_t TAB2[23]={0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,
                                    0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,
                                    0xF6,0xB8,0xDC,0xF3,0xBE,0xC0,0x00};


HAL_StatusTypeDef display_begin(I2C_HandleTypeDef *hi2c, uint8_t device_number){
	if (!(device_number >= 0 && device_number < 4))
		return HAL_ERROR;
	if (HAL_I2C_IsDeviceReady(hi2c, address[device_number], 5, 1000) == HAL_ERROR)
		return HAL_ERROR;
	else{
		  HAL_I2C_Master_Transmit(hi2c, address[device_number], &cmd[0], 1, HAL_MAX_DELAY);
		  HAL_I2C_Master_Transmit(hi2c, address[device_number], &cmd[1], 1, HAL_MAX_DELAY);
		  HAL_I2C_Master_Transmit(hi2c, address[device_number], &cmd[2], 1, HAL_MAX_DELAY);
		  HAL_I2C_Master_Transmit(hi2c, address[device_number], &brightness_tab[7], 1, HAL_MAX_DELAY);
		return HAL_OK;
	}
}


void display_brightness(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t brightness){
	if (brightness > 0 && brightness < 8)
		HAL_I2C_Master_Transmit(hi2c, address[device_number], &brightness_tab[brightness], 1, HAL_MAX_DELAY);
	else
		HAL_I2C_Master_Transmit(hi2c, address[device_number], &brightness_tab[7], 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef display_print_seg(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t seg_num, uint8_t dot, uint8_t value){
	if (!(device_number >= 0 && device_number < 4))
		return HAL_ERROR;
	if (!(seg_num >= 0 && seg_num < 8))
		return HAL_ERROR;
	if (!(value >= 0 && value < 23))
		return HAL_ERROR;
	if (dot == 0)
		HAL_I2C_Mem_Write(hi2c, address[device_number], seg[seg_num], 1, &TAB1[value], 1, HAL_MAX_DELAY);
	else
		HAL_I2C_Mem_Write(hi2c, address[device_number], seg[seg_num], 1, &TAB2[value], 1, HAL_MAX_DELAY);
}


void display_print_seg_all(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t* dots, uint8_t* values){
	for(int i=0; i<8; i++){
		display_print_seg(hi2c, device_number, i, dots[i], values[i]);
	}
}


void display_print_two(I2C_HandleTypeDef *hi2c, uint8_t device_number, double value1, double value2){
	if(value1 < 0){
		display_print_seg(hi2c, device_number, 0, 0, 21);
		value1 = -value1;
	}
	else
		display_print_seg(hi2c, device_number, 0, 0, 22);
	if (value1 > 999){
		for (int i = 1; i<4; i++)
			display_print_seg(hi2c, device_number, i, 0, 9);
	}
	else if (value1 >= 100){
		int value = (int)value1;
		for (int i=2; i>=0; i--){
			display_print_seg(hi2c, device_number, i, 0, value%10);
			value = value/10;
		}
	}
	else if (value1 >= 10){
		int value = (int)value1;
		value = (int)(value1*10);
		display_print_seg(hi2c, device_number, 3, 0, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 2, 1, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 1, 0, value%10);
		value = value/10;
	}
	else if (value1 > 0){
		int value = (int)value1;
		value = (int)(value1*100);
		display_print_seg(hi2c, device_number, 3, 0, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 2, 0, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 1, 1, value%10);
		value = value/10;
	}
	else
		for (int i = 1; i<4; i++)
			display_print_seg(hi2c, device_number, i, 0, 0);
	if(value2 < 0){
		display_print_seg(hi2c, device_number, 4, 0, 21);
		value2 = -value2;
	}
	else
		display_print_seg(hi2c, device_number, 4, 0, 22);
	if (value2 > 999){
		for (int i = 5; i<8; i++)
			display_print_seg(hi2c, device_number, i, 0, 9);
	}
	else if (value2 >= 100){
		int value = (int)value2;
		for (int i=7; i>4; i--){
			display_print_seg(hi2c, device_number, i, 0, value%10);
			value = value/10;
		}
	}
	else if (value2 >= 10){
		int value = (int)value2;
		value = (int)(value2*10);
		display_print_seg(hi2c, device_number, 7, 0, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 6, 1, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 5, 0, value%10);
		value = value/10;
	}
	else if (value2 > 0){
		int value = (int)value2;
		value = (int)(value2*100);
		display_print_seg(hi2c, device_number, 7, 0, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 6, 0, value%10);
		value = value/10;
		display_print_seg(hi2c, device_number, 5, 1, value%10);
		value = value/10;
	}
	else
		for (int i = 5; i<8; i++)
			display_print_seg(hi2c, device_number, i, 0, 0);
}












