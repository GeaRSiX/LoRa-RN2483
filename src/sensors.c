/*
 *	sensors.c
 *	date: august 2017
 *	author: Mitchelle Sangvhi
 */
/* ========
 *  MACROS
 * ========
 */
//includes
#include "sensors.h"

/* =========
 *  GLOBALS
 * =========
 */
int distance;										//holds the distance value converted from the samples
uint16_t samples_polled[SENSORS_NUM_POLLS] = {0};	//the sample data read from the channels
qm_adc_config_t cfg;								//config for ADC options
qm_adc_xfer_t transfer;								//struct holding sample data (used by qm_adc_convert())

/* ===========
 *  FUNCTIONS
 * ===========
 */
/*	EXTERNAL	*/
//converts analog readings to distance(cm)
static int analog_to_distance (unsigned int voltage)
{
	float volts = (float)voltage * 0.001210068; //0.001210068 = 5volts / 1024bits
	float scaled_volts = volts * 1.1;
	float distance = 45 * (float)(1 / scaled_volts);

	return distance;
}

//returns a mode of an array passed to it
static int get_mode(int data[])
{
	int count;
	int highest = 0;
	int mode = 0;

	for(int i = 0; i < CFG_ADC_TRANSFER_SAMPLES; i++)
	{
		count = 0;

		for(int j = 0; j < CFG_ADC_TRANSFER_SAMPLES; j++)
		{
			if(data[i] == data[j])
				count += 1;

			if(count > highest)
			{
				highest = count;
				mode = data[i];
			}
		}
	}

	return mode;
}

//returns the mean of an array passed to it
static int get_mean(int data[])
{
	int avg = 0;
	int mean = 0;
	int total = CFG_ADC_TRANSFER_SAMPLES - 2;

	for(int i = 0; i < CFG_ADC_TRANSFER_SAMPLES; i++)
		avg += data[i];

	mean = avg / total;

	return mean;
}

/*	EXTERNAL	*/
int SENSORS_init()
{
	//map pins to channels
	qm_adc_channel_t channels[CFG_ADC_CHANNEL_COUNT] =
	{
		CFG_ADC_CHANNEL_1,
		CFG_ADC_CHANNEL_2,
		CFG_ADC_CHANNEL_3,
		CFG_ADC_CHANNEL_4,
		CFG_ADC_CHANNEL_5
	};

	//Enable the ADC and set the clock divisor
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_ADC | CLK_PERIPH_ADC_REGISTER);
	clk_adc_set_div(CFG_ADC_TRANSFER_CLOCK_DIV);

	//set mode on pins to ADC
	qm_pmux_select (CFG_ADC_CHANNEL_1, CFG_ADC_PMUX);
	qm_pmux_select (CFG_ADC_CHANNEL_2, CFG_ADC_PMUX);
	qm_pmux_select (CFG_ADC_CHANNEL_3, CFG_ADC_PMUX);
	qm_pmux_select (CFG_ADC_CHANNEL_4, CFG_ADC_PMUX);
	qm_pmux_select (CFG_ADC_CHANNEL_5, CFG_ADC_PMUX);
	//enable i/o on the channel pins
	qm_pmux_input_en (CFG_ADC_CHANNEL_1, true);
	qm_pmux_input_en (CFG_ADC_CHANNEL_2, true);
	qm_pmux_input_en (CFG_ADC_CHANNEL_3, true);
	qm_pmux_input_en (CFG_ADC_CHANNEL_4, true);
	qm_pmux_input_en (CFG_ADC_CHANNEL_5, true);

	//set the mode and calibrate
	qm_adc_set_mode (CFG_ADC_TRANSFER_CONTROLLER, QM_ADC_MODE_NORM_CAL);
	qm_adc_calibrate (CFG_ADC_TRANSFER_CONTROLLER);

	//setup transfer config
	cfg.window = CFG_ADC_TRANSFER_WINDOW;
	cfg.resolution = CFG_ADC_TRANSFER_RESOLUTION;
	qm_adc_set_config (CFG_ADC_TRANSFER_CONTROLLER, &cfg);

	//setup transfer
	transfer.ch = channels;
	transfer.ch_len = CFG_ADC_CHANNEL_COUNT;
	transfer.samples = samples_polled;
	transfer.samples_len = SENSORS_NUM_POLLS;

	return EXIT_SUCCESS;
}
int SENSORS_read(char *ret)
{
	int readings[CFG_ADC_TRANSFER_SAMPLES] = {0};
	int i, j, k = 0;
	int result = 0;
	int meanCount;
	char *output = malloc(SENSORS_READSIZE);

	//run the conversion and wait for it to complete
	if (qm_adc_convert(CFG_ADC_TRANSFER_CONTROLLER, &transfer, NULL))
		return 1;

	//loop through channels
	for (i = 0; i < CFG_ADC_CHANNEL_COUNT; i++) {
		meanCount = 0;

		//collect readings for channel i (from readings[])
		for (j = i; j < CFG_ADC_TRANSFER_SAMPLES; j += CFG_ADC_CHANNEL_COUNT)
		{
			distance = analog_to_distance((unsigned int)samples_polled[j]);
			if (distance >= 50)
				meanCount++;
			readings[k] = distance; //store the readings in an array
			k++;
		}
		k = 0;

		//if the readings in above 50 at least 6 times, then get a mean else get a mode
		if (meanCount >= 6)
			result = get_mean(readings);
		else
			result = get_mode(readings);

		//write the readings to a string (<sensor_id>:<reading>)
		sprintf(output, "%d:%d!", i, result);
		strcat(ret, output);
	}

	free(output);
	return EXIT_SUCCESS;
}
