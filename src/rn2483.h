/*
 *	rn2483.h
 *	date: august 2017
 *	author: Alexander Collins
 *	description:
 *		This is the header file for rn2483.c.
 *		The rn2483.* files should contain all the logic related to using the RN2483 chip.
 *		Currently this file only has the functions I needed for the project I was writing it for. If it ends
 *		up needing more general usage here's a todo list:
 *			1. RN2483_read()	Currently there's no way of reading from the device to check the success/failure
 *								of commands sent. Obviously for any production enviroment this is very necessary.
 *			2. RN2483_rx()		One of LoRa's coolest features is the ability for the end-device to receive data
 *								over LoRa. For a lot of applications this could be useful.
 *			3. ..everything else
 *	notes:
 *		If you're not using a quark device, you'll need to change the UART library being used.
 *		This file currently uses "quark_uart.h", the only function being used in is the QUARK_UART_init()
 *		function in RN2483_init().
 *		The QUARK_UART_init() function in RN2483_init(), sets the stdout stream to the specified
 *		UART interface, using another uart library means you'll have to make sure that it also does this.
 *		This is done because RN2483_write() uses printf() to write to the RN2483 device.
 *
 *		I'm also using clk_sys_udelay() for sleep in rn2483.c, that will need to be changed for other boards.
 *		Most ARM boards should just let you use sleep(), so do a refactor in rn2483.c for that.
 */
#ifndef RN2483_H_
#define RN2483_H_

/* ========
 *  MACROS
 * ========
 */
//custom includes
#include "CFG.h"		//LORA_*
#include "quark_uart.h"	//QUARK_UART_*
//std includes
#include "clk.h"		//clk_*
#include "stdlib.h"		//EXIT_*
#include "string.h"

/* ===========
 *  FUNCTIONS
 * ===========
 */
/* RN2483_init()
 *
 * 	description: runs any internal initialization functions needed for this module to work.
 * 	params:
 * 		n/a
 * 	returns:
 * 		EXIT_FAILURE;	//if one of the init functions it runs fail
 * 		EXIT_SUCCESS;	//default return
 */
int		RN2483_init();
/* RN2483_write(const char *command)
 *
 * 	description: writes a command to the device over the uart interface
 * 	params:
 * 		const char *command;	//a string of the command you want to send to the device, see /docs/rn2483 command ref.pdf
 * 	returns:
 * 		EXIT_SUCCESS;	//default return
 * 	notes:
 * 	- if the command you sent was invalid, the function won't tell you (no current implementation of receiving over the uart interface)
 * 	- @TODO (after uart rx implemented) check device response for invalid command
 */
int		RN2483_write(const char *command);
//LoRa functions
/* RN2483_join()
 *
 * 	description: tries to a join a lora network using the join mode specified in CFG.h (CFG_LoRa_JoinMode)
 * 	params: n/a
 * 	returns:
 * 		EXIT_SUCCESS;	//default return
 * 	notes:
 * 	- the function currently doesn't tell you if it fails to join
 * 	- @TODO (after uart rx implemented) return failure if device reports join failure
 */
int		RN2483_join();
/* RN2483_tx(void *buff, uint buff_len)
 *
 * 	description: transmits a message over lora
 * 	params:
 * 		void *buff;		//the data buffer to be transmitted, note: void = any datatype
 * 		uint buff_len;	//the length of the data buffer to be transmitted
 * 	returns:
 * 		EXIT_SUCCESS;	//default return
 * 	notes:
 * 	- the function currently doesn't tell you if it's transmission fails or not, but it DOES send a confirmed transmission
 * 	- @TODO (after uart rx implemented) return failure if device reports tx failure
 */
int		RN2483_tx(void *buff, uint buff_len);
#endif /* RN2483_H_ */
