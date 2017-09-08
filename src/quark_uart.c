/*
 *	quark_uart.c
 *	date: august 2017
 *	author: Alexander Collins
 */
/* ========
 *  MACROS
 * ========
 */
//includes
#include "quark_uart.h"

/* =========
 *  GLOBALS
 * =========
 */
static uint8_t dummy_data;
static qm_uart_config_t cfg;

/* ===========
 *  FUNCTIONS
 * ===========
 */
/* INTERNAL */
static void init_pmux()
{
	qm_pmux_select(CFG_UART_TXD, CFG_UART_PMUX);
	qm_pmux_select(CFG_UART_RXD, CFG_UART_PMUX);
	qm_pmux_input_en(CFG_UART_RXD, true);

	qm_uart_read_non_block(CFG_UART_INTERFACE, &dummy_data);
}
static void init_clk()
{
	#ifdef	QUARK_UART_A
		clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);
	#elif	QUARK_UART_B
		clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTB_REGISTER);
	#endif
}
static void init_cfg()
{
	//set cfg struct
	cfg.hw_fc = CFG_UART_HWFC;
	cfg.line_control = CFG_UART_LINE;
	switch (CFG_UART_BAUD)
	{
		case 115200:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 17, 6);
			break;
		case 57600:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 34, 12);
			break;
		case 38400:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 52, 1);
			break;
		case 19200:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 104, 3);
			break;
		case 9600:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);
			break;
		case 2400:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(3, 65, 5);
			break;
		case 1200:
			cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(6, 130, 11);
			break;
	}

	//set new cfg
	qm_uart_set_config(CFG_UART_INTERFACE, &cfg);
}

/* EXTERNAL */
int	QUARK_UART_init()
{
	init_pmux();	//init uart physical
	init_clk();
	init_cfg();		//init uart interface

	return EXIT_SUCCESS;
}

