/*
 * hx711Config.h
 *
 *  Created on: Jan 20, 2023
 *      Author: Admin
 */

#ifndef INC_HX711CONFIG_H_
#define INC_HX711CONFIG_H_

#define	_HX711_USE_FREERTOS		0
#define _HX711_DELAY_US_LOOP  	4

#define HX711_CLK_GPIO_Port 	GPIOB
#define HX711_CLK_Pin			GPIO_PIN_14
#define HX711_DATA_GPIO_Port	GPIOB
#define HX711_DATA_Pin			GPIO_PIN_15

#endif /* INC_HX711CONFIG_H_ */
