/*
 *	main.c
 *	date: august 2017
 *	author: Alexander Collins, Mitchelle Sangvhi
 *	notes:
 *		This is an example main.c file that demonstrates the usage of the different files I wrote.
 *		All it does is join a LoRa network, read from the sensors, and then send that data over the
 *		joined LoRa network.
 */
/* ========
 *  MACROS
 * ========
 */
//std includes
#include "stdlib.h"		//EXIT_*
//custom includes
#include "rn2483.h"		//RN2483_*
#include "sensors.h"	//SENSORS_*

/* =========
 *  GLOBALS
 * =========
 */
int tx_count;
char *sensor_readings = NULL;

/* ===========
 *  FUNCTIONS
 * ===========
 */
int init()
{
	sensor_readings = malloc(SENSORS_READSIZE);

	//init RN2483
	if (RN2483_init() != EXIT_SUCCESS)
		return EXIT_FAILURE;

	//init sensors
	if (SENSORS_init() != EXIT_SUCCESS)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int main(void)
{
/*=======*
 * START *
 *=======*/
	//init
	if (init() != EXIT_SUCCESS)
		goto ERROR;

	//join lora network
	REJOIN:
	tx_count = 0;
	if (RN2483_join() == EXIT_FAILURE)
		goto REJOIN;	//failed to join

	//MAIN LOOP
	while (true)
	{
		//check tx_count for rejoin
		if (tx_count == CFG_LoRa_ReJoinDelay)
			goto REJOIN;

		//get sensor readings
		memset(sensor_readings, 0, SENSORS_READSIZE);
		SENSORS_read(sensor_readings);

		//tx sensor readings over lora
		RN2483_tx(sensor_readings, strlen(sensor_readings));

		tx_count++;
	}
/*=====*
 * END *
 *=====*/

//successful exit
	return 0;

//error exit
ERROR:
	return EXIT_FAILURE;
}
