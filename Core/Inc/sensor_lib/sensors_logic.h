
/*											TF-mini-PLUS -  COMMAND FRAME
 *
 *
 * USEFUL COMMANDS ON ST - (Compatible with UART and I2C)
 *
 * 	-- 	Change I2C Device Address 			: cmd = "5A 05 0B ADDR SU" 			; example: "5A 05 0B 20 8A" --> Address changes to 0x20
 * 	--	Change communication Protocol 		: cmd = "5A 05 0A MODE SU" 			; example: "5A 05 0A 01 6A" --> set comm. to I2C
 *
 *
 * 	--	Obtain DataFrame[cm](only i2c)		: cmd = "5A 05 00 01 60" 			; example: declare the command as follows:
 * 																						"cmd[0] = (90)dec" == (0x5A)hex  --> and so on ...
 *
 * 	--	Restore Factory Settings			: cmd = "5A 04 10 6E"
 * 	--	Save Settings 						: cmd = "5A 04 11 6F"
 *
 *
 */


/*											TF-Luna - COMMAND FRAME
 *
 *
 * 	-- 	Change the sampling
 * 		frequency of the sensor				: cmd = "5A 06 03 FA 00 00" -> 250Hz 				; example "5A 06 03 00 00 00"  -> 0Hz -> trigger mode
 * 	--	Change i2c ADDRESS					: cmd = "5A 05 0B 20 00"  	-> Change to 0x20
 *
 *5A 05 0B 11 00
 *
 * 	--	Trigger a Sample when
 * 		in Trigger Mode						: cmd = "5A 04 04 00"		-> TAKES SAMPLE - UPDATES REGISTERS
 *
 * 	--	Save current settings				: cmd = "5A 04 11 00"		-> "0" - Success
 * 	--	Restore Default settings			: cmd = "5A 04 10 00"		-> "0" - Success
 * 	--	Software Reset 						: cmd = "5A 04 02 00"		-> "0" - Success
 */

/*
 * 	This library works on the data captured by the laser sensor cited above. After the data is collected from the communication protocol
 * 	the functions declared here should resolve the data frame in coherent information and do other mathematical processes.
 */


/*
 * 					PROTOTYPES
 */
uint16_t get_valid_laser_dist(uint8_t* data);
//void somethin_med(LASER_DATA* ls, int* acc, int qtd, int samples);			// OUTDATED
void somethin_med_(uint16_t* reading, uint16_t* med, int* acc, int qtd, int samples, int* med_flag);
char* notify_COMM_STATE(uint16_t dist);
uint16_t* stack_median_reading(uint16_t dist, uint8_t* cont, uint16_t* accumulator, uint8_t sampling);
uint16_t release_median(uint16_t* accumulator);
uint16_t get_median_r(uint8_t dist, uint8_t* cont, uint16_t* acc , uint16_t* median);
uint16_t UPDATE_median_r(uint8_t dist, uint8_t* cont, uint16_t* acc , uint16_t* median);
uint16_t release_result(uint16_t dist);


// retorna distancia se leitura válida  ou (-1) caso leitura invalida
uint16_t get_valid_laser_dist(uint8_t* data){
	if(data[0] == 0x59 && data[1] == 0x59 ){						// somente quando há leituras válidas no IC do sensor laser
		uint8_t check = data[0] + data[1] + data[2] +
				data[3] + data[4] + data[5] + data[6] + data[7];
		if(data[8] == check % 256){
			// reading is valid
			uint16_t dist = data[2] + data[3] * 256;
			return dist;
		}
		else{
			return ((uint16_t)(1));
		}
	}
	else{
		return ((uint16_t)(2));
	}
}

//void somethin_med(LASER_DATA* ls, int* acc, int qtd, int samples){
//	for(int i =0; i< qtd; i++){
//		if(ls[0].VEC_DIST_LS_BAR[i][0] != 0){
//			acc[0]+=1;
//			ls[0].medias[i] += ls[0].VEC_DIST_LS_BAR[i][0];
//		}
//	}
//	//HAL_Delay(2);
//	acc[1]++;
//	if(acc[1] == samples){
//		acc[0]=0;
//		acc[1]=0;
//		for(int i =0; i< qtd; i++){
//			ls[0].medias[i] *= 0.125;
//			//HAL_Delay(2);
//		}
//		ls[0].med_flag = 1;
//	}
//
//
//}

void somethin_med_(uint16_t* reading, uint16_t* med, int* acc, int qtd, int samples, int* med_flag){
	for(int i =0; i< qtd; i++){
		if(reading[i] != 0){
			acc[0]+=1;
			med[i] += reading[i];
		}
	}
	//HAL_Delay(2);
	acc[1]++;
	if(acc[1] == samples){
		acc[0]=0;
		acc[1]=0;
		for(int i =0; i< qtd; i++){
			med[i] *= 0.125;
			//HAL_Delay(2);
		}
		*med_flag = 1;
	}


}

// (internal) -- notification of COMMUNICATION STATE - laser_scan()
char* notify_COMM_STATE(uint16_t dist){
	char *notice;
	if(dist == 0){
		notice = (char*)"Error on COMM STATE";
	}
	else if( dist == 1){
		notice = (char*)"Error on checksum";
	}
	else if( dist == 2){
		notice = (char*)"Error on Header Frame";
	}
	else{
		notice = (char*)"All OK";
	}
	return notice;
}

// stacks the sum of MAX_READINGs on the vector accumulator --> accumulator[0]
// takes samples (dist) read by the laser sensor
uint16_t* stack_median_reading(uint16_t dist, uint8_t* cont, uint16_t* accumulator, uint8_t sampling){
	uint8_t max_reading = sampling;
	if(cont[0] < max_reading){
		accumulator[0] += (uint16_t)(dist);
		accumulator[1] = 0;
		cont[0]+=1;
	}
	else{
    	cont[0] = 0;									// reset counter for next set of samples
		accumulator[1] = 1;								// flag up accumulator[1] --- the samples were taken
	}
	return accumulator;
}

// when there are enough samples in the accumulator --> take median
uint16_t release_median(uint16_t* accumulator){
	if(accumulator[1] == 1){												// flag of accumulator sampling
		accumulator[1] = 0;													// reset accumulator flag
		uint16_t res = (uint16_t)((double) (accumulator[0] * 0.125));
		accumulator[0] = 0;													// reset accumulator
		return res;
	}
	else{
		return 0;
	}

}

// only samples when there is valid data on the laser scan "dist"
uint16_t get_median_r(uint8_t dist, uint8_t* cont, uint16_t* acc , uint16_t* median){
	//uint16_t latest = median[0];
	if(dist != 0){
		median[0] = release_median( (stack_median_reading(dist, cont, acc, 4*2)) );
	}else{
		median[0] = 0;
	}

	return median[0];
}

uint16_t UPDATE_median_r(uint8_t dist, uint8_t* cont, uint16_t* acc , uint16_t* median){

	if(dist != 0){
		median[0] = release_median( (stack_median_reading(dist, cont, acc, 4*2)) );
//		if(median[0]== 0 && latest != 0){
//			median[0] = latest;
//		}
	}else{
		median[0] = 0;
	}

	return median[0];
}

// release result for laser scan distance reading -- (internal) - laser_scan()
uint16_t release_result(uint16_t dist){
	if(notify_COMM_STATE(dist) == (char*)"All OK"){
		return dist;
	}
	else{
		return 0;
	}
}





