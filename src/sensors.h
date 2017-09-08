/*
 *	sensors.h
 *	date: august 2017
 *	author: Alexander Collins
 *	description:
 *		This is the header file for sensors.c, sensors.* files contain all of the
 *		software for using the sensors connected to the board.
 *		I've tried to abstract any details outside of user scope related to sensors.c
 *		To change the pins being used or any details like that, look in CFG.h, sensors.c
 *		shouldn't need to be changed (i hope).
 */

#ifndef SENSORS_H_
#define SENSORS_H_

/* ========
 *  MACROS
 * ========
 */
//custom includes
#include "CFG.h"
//std includes
#include "clk.h"		//clk_*
#include "string.h"		//strcat
#include "stdlib.h"		//EXIT_*
#include "qm_adc.h"		//QM_ADC_*
#include "qm_pinmux.h"	//QM_PMIX_*, QM_PIN_*

//defines
#define SENSORS_READSIZE		10
#define	SENSORS_NUM_POLLS		(CFG_ADC_CHANNEL_COUNT * CFG_ADC_TRANSFER_SAMPLES)

/* ===========
 *  FUNCTIONS
 * ===========
 */
int SENSORS_init();
int SENSORS_read();

#endif /* SENSORS_H_ */
