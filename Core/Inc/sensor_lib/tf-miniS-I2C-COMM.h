
/*
 *
 * LIB FOR TF LUNA MINI
 *
 */
#include "main.h"

/*
 * 				PROTOTYPES
 */

void i2c_save_settings_tfluna(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
uint16_t i2c_receive_bus_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
uint8_t* set_UART_PTL_from_i2c(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_send_cmd(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_change_addr_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_save_settings_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_factory_reset_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR);


uint16_t i2c_receive_bus_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
//	HAL_StatusTypeDef 		status;
//	uint8_t* notice;
	uint16_t dist=0;
	uint8_t i2c_cmd[5];					// COMMAND OBTAIN DATAFRAME
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 5;
	i2c_cmd[2] = 0;
	i2c_cmd[3] = 1;
	i2c_cmd[4] = 96;
	uint8_t data[9];
	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, i2c_cmd  , 5 , 90) != HAL_OK){
//		Error_Handler();
	}else{
		if(HAL_I2C_Master_Receive(hi2c, ADDR<<1, data, sizeof(data), 90) != HAL_OK){
			HAL_Delay(1);
		}
		else{
			HAL_Delay(1);
			dist = data[2] + data[3]*256;
		}
	}
	return dist;
}

uint8_t* set_UART_PTL_from_i2c(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	uint8_t*    notification;
	uint8_t 	uart_cmd[5], save_cmd[4];				//  5A 05 0A MODE SU    // MODE = 0 UART // MODE = 1  I2C
														// 5A 05 0A 01 69
	uart_cmd[0] = 90;
	save_cmd[0] = 90;

	uart_cmd[1] = 5;
	save_cmd[1] = 4;

	uart_cmd[2] = 10;
	save_cmd[2] = 17;

	uart_cmd[3] = 0;
	save_cmd[3] = 111;

	uart_cmd[4] = 105;
	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, uart_cmd, sizeof(uart_cmd), 90) != HAL_OK){
		notification = (uint8_t*)"NO COMMAND HAS BEEN SENT";
	}else{
		notification = (uint8_t*)"COMMAND SET TO UART HAS BEEN SENT";
		if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, save_cmd, sizeof(save_cmd), 60) != HAL_OK){
			notification = (uint8_t*)"THE COMMAND WAS NOT SAVED";
		}else{
			notification = (uint8_t*)"THE COMMAND WAS SENT AND SAVED";
		}
	}
	return notification;
}

void i2c_send_cmd(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	uint8_t i2c_cmd[6];
	// 5A 06 03 0A 00 00   5A 06 03 00 00 SU
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 6;
	i2c_cmd[2] = 3;
	i2c_cmd[3] = 0;
	i2c_cmd[4] = 0;
	i2c_cmd[5] = 0;
	if(HAL_I2C_Master_Transmit(hi2c, ADDR, i2c_cmd  , 6 , 200) != HAL_OK){
//		Error_Handler()
	}else{
		HAL_Delay(1);
	}
}

// 5A 06 03 00 64 C7  // 323

void i2c_change_addr_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	uint8_t i2c_cmd[5];
	// 5A 05 0B ADDR SU
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 5;
	i2c_cmd[2] = 11;
	i2c_cmd[3] = (uint8_t) ADDR;
	i2c_cmd[4] = i2c_cmd[0] + i2c_cmd[1] + i2c_cmd[2] + i2c_cmd[3];

	if(HAL_I2C_Master_Transmit(hi2c, ADDR, i2c_cmd  , 5 , 200) != HAL_OK){
//		Error_Handler()
	}else{
		HAL_Delay(1);
	}
}

void i2c_save_settings_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	uint8_t i2c_cmd[4];
	// 5A 04 11 6F
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 4;
	i2c_cmd[2] = 11;
	i2c_cmd[3] = 111;


	if(HAL_I2C_Master_Transmit(hi2c, ADDR, i2c_cmd  , 4 , 200) != HAL_OK){
//		Error_Handler()
	}else{
		HAL_Delay(1);
	}
}

void i2c_factory_reset_tfminiS(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	uint8_t i2c_cmd[4];
	// 5A 04 10 6E
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 4;
	i2c_cmd[2] = 10;
	i2c_cmd[3] = 110;

	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, i2c_cmd  , 4 , 200) != HAL_OK){
//		Error_Handler()
	}else{
		HAL_Delay(1);
	}
}

