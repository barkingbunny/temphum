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
// the priority is selected by place in the list.
typedef enum {	MEASURING,
				HEATING,
				TEST,
				MENUn,
				TE_ERROR,
				SLEEP,
				TIME,
				IDLE,
				VOLTAGE,
				LOG
} States_loop;

typedef enum  	{
	menu,
	blind,
	desktop,
	debug,
	idle
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
#define MEASURE_PERIODE 5000 // every 5 secondn
#define LED_PERIODE 500 //
#define TIME_PERIODE 400

extern Flags_main flags;


#endif /* MODULE_GLOBAL_H_ */
