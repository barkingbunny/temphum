/*
 * global.h
 *
 *  Created on: Dec 29, 2016
 *      Author: jakub
 */

#ifndef MODULE_GLOBAL_H_
#define MODULE_GLOBAL_H_

#include "stm32l0xx_hal.h"

typedef enum {FALSE = 0u, TRUE = 1u} Bool;

typedef enum {	MEASURING,
				HEATING,
				TEST,
				MENUn,
				IDLE,
				TE_ERROR,
				SLEEP
} States_loop;

typedef enum  	{
	menu,
	blind,
	temp,
	def,
	debug,
	time
} Screen;

typedef struct {
	uint8_t new_data_to_show:1;
	uint8_t new_data:1;
	uint8_t measure_activate:1;
	uint8_t measure_running:1;
	uint8_t menu_activate:1;
	uint8_t menu_running:1;
}Flags_main;



#define BUT_DELAY 100		// in milisecond
#define MAIN_LOOP 25		// in milisecond
#define MEASURE_PERIODE 5000 // every 1 secondn
#define LED_PERIODE 500 //

extern Flags_main flags;


#endif /* MODULE_GLOBAL_H_ */
