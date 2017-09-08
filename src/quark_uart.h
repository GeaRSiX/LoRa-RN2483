/*
 *	quark_uart.c
 *	date: august 2017
 *	author: Alexander Collins
 */
#ifndef QUARK_UART_H_
#define QUARK_UART_H_

/* ========
 *  MACROS
 * ========
 */
//custom includes
#include "CFG.h"
//std includes
#include "clk.h"					//clk_*
#include "stdlib.h"					//EXIT_*
#include "qm_uart.h"				//QM_UART_*
#include "qm_isr.h"					//QM_IRQ_*
#include "qm_pinmux.h"				//QM_PMIX_*, QM_PIN_*
#include "qm_interrupt.h"			//QM_IRQ_REQUEST
#include "qm_interrupt_router.h"	//QM_IR_*

/* ===========
 *  FUNCTIONS
 * ===========
 */
int	QUARK_UART_init();

//synchronous
int	QUARK_UART_write(const void *buff);

#endif /* QUARK_UART_H_ */
