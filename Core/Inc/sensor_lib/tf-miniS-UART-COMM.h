
/*
 * DEFINIR VARIÀVEIS PADROES
 *
 *
 */
#include "sensors_logic.h"
//#include "main.h"

/*
 * 				PROTOTYPES
 */

uint16_t laser_read(UART_HandleTypeDef* huartX);				// UART communication
uint16_t laser_read_UART(UART_HandleTypeDef* huartX);
void setup_i2c_laser_fromUART(UART_HandleTypeDef* huartX);

//static uint8_t ALTURA_IDEAL = 120; 						// unidade [cm]

uint16_t laser_read_UART(UART_HandleTypeDef* huartX){

	HAL_UART_StateTypeDef uart_state;
	uint8_t pData[9] = {0,0,0,0,0,0,0,0,0};
	uint16_t val;

	uart_state = HAL_UART_Receive( huartX, pData, 9, 4);

	if (uart_state != 0){
		return (uint16_t)0;
	}
	else{
		val = get_valid_laser_dist(pData);
		val = release_result(val);
		return val;
	}

}

void setup_i2c_laser_fromUART(UART_HandleTypeDef* huartX){
	uint8_t i2c_cmd[5];
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 5;
	i2c_cmd[2] = 5;
	i2c_cmd[3] = 6;
	i2c_cmd[4] = 106;

	if(HAL_UART_Receive(huartX, i2c_cmd, 5, 1000) != HAL_OK){
		// sai somente se ocorrer tudo ok
		HAL_Delay(1);
	}
	HAL_Delay(10);

}
