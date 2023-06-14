#include <dfr0646-g.h>

static const uint8_t address[4] = {0xE0, 0xE2, 0xE4, 0xE6}; //all possible device addresses
static const uint8_t cmd[3] = {0x21, 0x81, 0xEE}; //initialization command, clock enable, display enable
static const uint8_t brightness_tab[8] = {0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8}; //brightness value
static const uint8_t seg[8] = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e}; //segments addresses

HAL_StatusTypeDef display_begin(I2C_HandleTypeDef *hi2c, uint8_t device_number){
	uint8_t flag = 0;
	if (!(device_number >= 0 && device_number < 4))
		return HAL_ERROR;
	if (HAL_I2C_IsDeviceReady(hi2c, address[device_number], 5, 1000) == HAL_ERROR)
		return HAL_ERROR;
	else{
		  if (HAL_I2C_Master_Transmit(hi2c, address[device_number], &cmd[0], 1, HAL_MAX_DELAY) != HAL_OK)
			  flag = 1;
		  if (HAL_I2C_Master_Transmit(hi2c, address[device_number], &cmd[1], 1, HAL_MAX_DELAY) != HAL_OK)
			  flag = 1;
		  if (HAL_I2C_Master_Transmit(hi2c, address[device_number], &cmd[2], 1, HAL_MAX_DELAY) != HAL_OK)
			  flag = 1;
		  if (HAL_I2C_Master_Transmit(hi2c, address[device_number], &brightness_tab[7], 1, HAL_MAX_DELAY) != HAL_OK)
			  flag = 1;
	if (flag==0)
		return HAL_OK;
	else
		return HAL_ERROR;
	}
}

HAL_StatusTypeDef display_brightness(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t brightness){
	if (brightness > 0 && brightness < 8)
		return HAL_I2C_Master_Transmit(hi2c, address[device_number], &brightness_tab[brightness], 1, HAL_MAX_DELAY);
	else
		return HAL_I2C_Master_Transmit(hi2c, address[device_number], &brightness_tab[7], 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef display_print_seg(I2C_HandleTypeDef *hi2c, uint8_t device_number, uint8_t seg_num, uint8_t dot, uint8_t value){
	if (!(device_number >= 0 && device_number < 4))
		return HAL_ERROR;
	if (!(seg_num >= 0 && seg_num < 8))
		return HAL_ERROR;
	if (!(value >= 0 && value < 23))
		return HAL_ERROR;
	if (dot == 0)
		return HAL_I2C_Mem_Write(hi2c, address[device_number], seg[seg_num], 1, &TAB1[value], 1, HAL_MAX_DELAY);
	else
		return HAL_I2C_Mem_Write(hi2c, address[device_number], seg[seg_num], 1, &TAB2[value], 1, HAL_MAX_DELAY);
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
		for (int i=3; i>0; i--){
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
