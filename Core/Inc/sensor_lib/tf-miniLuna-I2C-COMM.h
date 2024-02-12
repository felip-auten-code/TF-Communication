
/*
 *
 * LIB FOR TF LUNA MINI
 *
 */
#include "main.h"

/*
 * 					PROTOTYPES
 */
uint8_t i2c_scan_devices(I2C_HandleTypeDef * hi2c, uint8_t* addrs);
void i2c_setup_triggerMode(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_reset_device(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_reset_freq_MEM(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
uint16_t i2c_receive_bus_tfLuna(I2C_HandleTypeDef * hi2c, uint16_t ADDR, uint16_t* dist);
uint16_t i2c_receive_bus_tfLuna_(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_change_addr_tfluna(I2C_HandleTypeDef * hi2c, uint16_t ADDR, uint8_t new_ADDR);
void i2c_change_addr_tfluna_MEM(I2C_HandleTypeDef * hi2c, uint16_t ADDR, uint8_t new_ADDR);
void i2c_save_settings_tfluna(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void i2c_save_settings_tfluna_MEM(I2C_HandleTypeDef * hi2c, uint16_t ADDR);
void sensor_take_reading(I2C_HandleTypeDef* i2cX, uint16_t* readings, uint16_t ADDR, uint8_t idx_sensor);
int test_sensor(I2C_HandleTypeDef* i2cX, uint16_t ADDR);


uint8_t i2c_scan_devices(I2C_HandleTypeDef * hi2c, uint8_t* addrs){
	//uint8_t* msg = "Scanning Devices";
	uint8_t ctt=0;
	for(int i = 0 ; i< 127; i++){
		if(HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(i<<1), 3, 300) != HAL_OK){
			//HAL_I2C_Master_Transmit(hi2c, (uint16_t)(i<<1) , msg , sizeof(msg) , 200);
//			HAL_Delay(1);
		}else{
			if(i!=0){
				addrs[ctt] = i;
				ctt++;
			}
		}
	}
	return ctt;
}

void i2c_setup_triggerMode(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	// 5A 06 03 00 00 00
	uint8_t i2c_cmd[6];					// COMMAND TRIGGER DETECTION MODE
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 6;
	i2c_cmd[2] = 3;
	i2c_cmd[3] = 0;
	i2c_cmd[4] = 0;
	i2c_cmd[5] = 0;
	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, i2c_cmd , 6, 200) != HAL_OK){

	} else {
		HAL_Delay(1);
	}
}

void i2c_reset_device(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	// 5A 04 02 00
	if(HAL_I2C_Mem_Write(hi2c, ADDR, 0x29 , 1, (uint8_t*)1, 1, 120) != HAL_OK){
		// I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout

	}else{
		//HAL_Delay(1);

		HAL_Delay(1);
	}
}

void i2c_reset_freq_MEM(I2C_HandleTypeDef * hi2c, uint16_t ADDR){		// Continuous ranging mode
	// 5A 06 03 FA 00 00
	if(HAL_I2C_Mem_Write(hi2c, ADDR, 0x23 , 1, (uint8_t*)0, 1, 120) != HAL_OK){
		// I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout

	}else{
		//HAL_Delay(1);

		HAL_Delay(1);
	}
}

uint16_t i2c_receive_bus_tfLuna(I2C_HandleTypeDef * hi2c, uint16_t ADDR, uint16_t* dist){
	// trigger mode sample --> 5A 04 04 00
	uint8_t i2c_cmd[4];					// COMMAND TRIGGER DETECTION
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 4;
	i2c_cmd[2] = 4;
	i2c_cmd[3] = 0;

	uint8_t DIST_L[1], DIST_H[1];

	if(HAL_I2C_Master_Transmit(hi2c, ADDR, i2c_cmd  , 4 , 120) != HAL_OK){

	}else{
		if(HAL_I2C_Mem_Read(hi2c, ADDR, 0x00, 1, DIST_L, 1, 120) != HAL_OK){
//			HAL_Delay(1);
		}
		else{
			if(HAL_I2C_Mem_Read(hi2c, ADDR, 0x01, 1, DIST_H, 1, 120) != HAL_OK){
//				HAL_Delay(1);
			}else{
				//HAL_Delay(1);
				dist[0] = DIST_L[0] + DIST_H[0] * 256;
//				HAL_Delay(1);
			}
		}
	}
	return dist[0];

}

uint16_t i2c_receive_bus_tfLuna_(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	// trigger mode sample --> 5A 04 04 00
	uint8_t i2c_cmd[4];					// COMMAND TRIGGER DETECTION
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 4;
	i2c_cmd[2] = 4;
	i2c_cmd[3] = 0;

	uint8_t DIST_L[1], DIST_H[1];
	uint16_t dist=0;

	if(HAL_I2C_Master_Transmit(hi2c, ADDR, i2c_cmd  , 4 , 120) != HAL_OK){

	}else{
		if(HAL_I2C_Mem_Read(hi2c, ADDR, 0x00, 1, DIST_L, 1, 120) != HAL_OK){
//			HAL_Delay(1);
		}
		else{
			if(HAL_I2C_Mem_Read(hi2c, ADDR, 0x01, 1, DIST_H, 1, 120) != HAL_OK){
//				HAL_Delay(1);
			}else{
				//HAL_Delay(1);
				dist = DIST_L[0] + DIST_H[0] * 256;
//				HAL_Delay(1);
			}
		}
	}
	return dist;

}

uint16_t i2c_trigger_sample_tfLuna_(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	// trigger mode sample --> 5A 04 04 00
	uint8_t i2c_cmd[4];					// COMMAND TRIGGER DETECTION
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 4;
	i2c_cmd[2] = 4;
	i2c_cmd[3] = 0;

	//uint8_t DIST_L[1], DIST_H[1];
	uint16_t dist=0;

	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, i2c_cmd  , 4 , 120) != HAL_OK){

	}else{

		HAL_Delay(10);
	}
	return dist;

}

void i2c_change_addr_tfluna(I2C_HandleTypeDef * hi2c, uint16_t ADDR, uint8_t new_ADDR){
	uint8_t i2c_cmd[5];
	// 5A 05 0B 20 00
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 5;
	i2c_cmd[2] = 11;
	i2c_cmd[3] = new_ADDR;
	i2c_cmd[4] = 0;

	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, i2c_cmd  , 5 , 200) != HAL_OK){
//		Error_Handler()
	}else{
		HAL_Delay(1);
	}
//	i2c_save_settings_tfluna(hi2c, new_ADDR<<1);
}

void i2c_change_addr_tfluna_MEM(I2C_HandleTypeDef * hi2c, uint16_t ADDR, uint8_t new_ADDR){
	if(HAL_I2C_Mem_Write(hi2c, ADDR, 0x22, 1, &new_ADDR, 1, 120) != HAL_OK){
		// I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout

	}else{
		//HAL_Delay(1);

		HAL_Delay(1);
	}
}

void i2c_save_settings_tfluna(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	uint8_t i2c_cmd[4];
	// 5A 04 11 00
	i2c_cmd[0] = 90;
	i2c_cmd[1] = 4;
	i2c_cmd[2] = 17;
	i2c_cmd[3] = 0;


	if(HAL_I2C_Master_Transmit(hi2c, ADDR<<1, i2c_cmd  , 4 , 200) != HAL_OK){
//		Error_Handler()
	}else{
		HAL_Delay(1);
	}
}

void i2c_save_settings_tfluna_MEM(I2C_HandleTypeDef * hi2c, uint16_t ADDR){
	if(HAL_I2C_Mem_Write(hi2c, ADDR, 0x20, 1, (uint8_t*)1, 1, 120) != HAL_OK){
		// I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout

	}else{
		//HAL_Delay(1);

		HAL_Delay(1);
	}
}

void sensor_take_reading(I2C_HandleTypeDef* i2cX, uint16_t* readings, uint16_t ADDR, uint8_t idx_sensor){
	readings[idx_sensor] = i2c_receive_bus_tfLuna_(i2cX, ADDR<<1);
}

int test_sensor(I2C_HandleTypeDef* i2cX, uint16_t ADDR){
	if(HAL_I2C_IsDeviceReady(i2cX, (uint16_t)(ADDR<<1), 30, 100) != HAL_OK){
		return 0;
	}else{
		return 1;
	}
}

