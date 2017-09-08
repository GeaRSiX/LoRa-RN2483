/*
 *	CFG.h
 *	date: august 2017
 *	author: Alexander Collins
 *	notes:
 *		It might be possible more settings need to be added in the future.
 *		I've tried to document & explain all these settings as much as possible.
 *		The UART CONFIG and ADC CONFIG settings are very tailored towards the
 *		Quark D2000 device I was writing this for, they'll probably need to be
 *		changed for any other board.
 */
#ifndef _CFG_LORA_H_
#define _CFG_LORA_H_

/*	===========
 *	LoRa CONFIG
 *	===========
 */
/*	------------------
 	KEYS & IDs (KIds? :D)
  	------------------
	These are the LoRa Keys & ID's used for joining a LoRa network.
	For more details on the separate keys & identifiers, see:
		LoRa Alliance, LoRaWAN Specification V1.0, 6 End-Device Activation, pg.30
	Notes:
		DevAddr = 8 bytes
		DevEUI = 16 bytes
		AppEUI = 16 bytes
		AppKey = 32 bytes
 */
#define	CFG_LoRa_DevAddr		"4449434B"
#define	CFG_LoRa_DevEUI			"5457494E5045414B"
#define CFG_LoRa_AppEUI			"4621434B54484953"
#define	CFG_LoRa_AppKey			"4D49544348454C4C54202620414C4558"
/*  ---------
    JOIN MODE
    ---------
    The different LoRa join mode used.
	For more details on the two join modes, see:
		LoRa Alliance, LoRaWAN Specification V1.0, 6 End-Device Activation, pg.30
    Options:
    	"otaa"		//over the air activation
    	"abp"		//activation by personalisation
*/
#define CFG_LoRa_JoinMode		"otaa"
/* ------------
 * REJOIN DELAY
 * ------------
 * How many transmissions get sent before sending another join request.
 * This is done in a lazy way at the moment, but it should ensure a rejoin if the connection faults.
 */
#define CFG_LoRa_ReJoinDelay	1000
/* ------------
 * RE-TRANSMISSION DELAY
 * ------------
 * The sleep time after transmitting data.
 * Don't set this value too low or you'll fill up all the LoRa channels and won't be able to send
 * anything else for another minute or two. For more details read up on how LoRa works.
 * For the LoRaBee (which this software was written for I found something between 3 - 10 seconds was best.
 */
#define CFG_LoRa_ReTxDelay		5000000		//u secs
/*  ---------
    FREQUENCY
    ---------
    The radio frequency you're using.
	Note: LoRa operatings on the ISM band, the options listed below only cover EU frequencies because
	the RN2483 model I was using only supported EU frequencies.
	For more details on LoRa frequencies, see:
		LoRa Alliance, LoRaWAN Specification V1.0, 7 Physical Layer, pg.34
    Options:
		"868"
		"434"
*/
#define CFG_LoRa_Frequency		"868"
/*	---------
	DATA RATE
	---------
	The 'data rate' setting of the LoRa device.
	In LoRa, the data rate affects the maximum length of the data being sent (see below)
	For more details, see:
		LoRa Alliance, LoRaWAN Specification V1.0, 7 Physical Layer, pg.34
    Options:
		"0" - "2"	//data len 59
		"3"			//data len 123
		"4" - "7"	//data len 230
*/
#define	CFG_LoRa_DataRate		"5"


/*	===========
 *	UART CONFIG
 *	===========
 */
/*	------------------
	PHYSICAL INTERFACE
	------------------
	The physical pins that are being used for the UART interface.
	These settings will probably need to be changed if you're not
	on a Quark D2000.
*/
//physical uart interface
#define CFG_UART_INTERFACE		QM_UART_0			//UART_A
//uart interface pins
#if (CFG_UART_INTERFACE == QM_UART_0)
	#define CFG_UART_TXD			QM_PIN_ID_12	//UART_A TX (D1)
	#define CFG_UART_RXD			QM_PIN_ID_13	//UART_A RX (D0)
#elif (CFG_UART_INTERFACE == QM_UART_1)
	#define CFG_UART_TXD			QM_PIN_ID_20	//UART_B TXD
	#define CFG_UART_RXD			QM_PIN_ID_21	//UART_B RXD
#endif
//pin's mode for uart
#define CFG_UART_PMUX			QM_PMUX_FN_2		//UART

/*	-----------------
	LOGICAL INTERFACE
	-----------------
	The logical interface settings for UART.
	The CFG_UART_LINE setting will need changing for non-quark devices.
 */
#define CFG_UART_BAUD			57600			//baud rate
#define CFG_UART_LINE			QM_UART_LC_8N1	//8 data bits, No parity, 1 stop bit
#define CFG_UART_HWFC			0				//hardware flow control (0 / 1)


/*	==========
 *	ADC CONFIG
 *	==========
 */
/*	----
	PINS
	----
 */
//number of sensors
#define CFG_ADC_CHANNEL_COUNT	5					//if this increases/decreases you'll need to change SENSORS_init()
//pin ids
#define CFG_ADC_CHANNEL_1		QM_PIN_ID_3			//A0
#define	CFG_ADC_CHANNEL_2		QM_PIN_ID_4			//A1
#define	CFG_ADC_CHANNEL_3		QM_PIN_ID_11		//D2
#define	CFG_ADC_CHANNEL_4		QM_PIN_ID_10		//D3
#define	CFG_ADC_CHANNEL_5		QM_PIN_ID_5			//D4
//pin's mode
#define	CFG_ADC_PMUX			QM_PMUX_FN_1		//1 = ADC
/*	---------------
	TRANSFER CONFIG
	---------------
 */
//config
#define	CFG_ADC_TRANSFER_CONTROLLER		QM_ADC_0			//ADC controller
#define CFG_ADC_TRANSFER_CLOCK_DIV		320					//ADC clock divisor
//transfer
#define	CFG_ADC_TRANSFER_WINDOW			20					//clock cycles between the start of each sample
#define CFG_ADC_TRANSFER_RESOLUTION		QM_ADC_RES_12_BITS;	//12-bit steps for ADC
#define	CFG_ADC_TRANSFER_SAMPLES		5					//number of samples to collect from each sensor (per read call)

#endif /* _CFG_LORA_H_ */
