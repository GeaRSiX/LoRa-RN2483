/*
 *	rn2483.c
 *	date: august 2017
 *	author: Alexander Collins
 *	notes:
 *		Just to explain what the get_hex_string() function is for:
 *		The tx command on the RN2483 requires that data being sent to be in a hex format, however
 *		the commands need to be a string ending in \r\n. This is covered in the documentation, but
 *		regardless it makes life really difficult.
 *		Luckily I wrote of a cool function that converts a databuffer to a string representation of it's
 *		hex value, i.e: 12 = 0x0C = "0C".
 */
/* ========
 *  MACROS
 * ========
 */
//includes
#include "rn2483.h"

//defines
#define U_SECOND	1000000				//1 second (second in microseconds)
#define DELAY_WRITE (1.5 * U_SECOND)	//1.5secs
#define DELAY_SAVE	(3 * U_SECOND)		//3secs

/* =========
 *  GLOBALS
 * =========
 */
int PORT = 1;	//port field seems to be arbitrary in RN2483
const char hex_map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/* ===========.
 *  FUNCTIONS
 * ===========
 */
/*	INTERNAL	*/
/* init_mac()
 *
 * 	description: sends all the mac commands to the RN2483 needed to initialise it for joining a lora network
 * 	params:
 * 		n/a
 * 	returns:
 * 		EXIT_SUCCESS;	//default return
 */
static int init_mac()
{
	//reset rn2483
	RN2483_write("mac reset " CFG_LoRa_Frequency);

	//set DevAddr
	RN2483_write("mac set devaddr " CFG_LoRa_DevAddr);
	//set DevEui
	RN2483_write("mac set deveui " CFG_LoRa_DevEUI);
	//set AppEui
	RN2483_write("mac set appeui " CFG_LoRa_AppEUI);
	//set AppKey
	RN2483_write("mac set appkey " CFG_LoRa_AppKey);
	//set data rate
	RN2483_write("mac set dr " CFG_LoRa_DataRate);

	//save settings
	RN2483_write("mac save");
	//clk_sys_udelay while module saves (any commands send during save get ignored)
	clk_sys_udelay(DELAY_SAVE);

	return EXIT_SUCCESS;
}

/* get_hex_string(uint8_t *buff, char *ret, int buff_len)
 *
 * 	description: coverts a hex value to a string representation of that hex value using hex_map
 * 	params:
 * 		uint8_t *buff;	//pointer to the uint8_t value to be converted, e.g: 0xA1
 * 		char *ret;		//string to be filled with string representation of hex value
 * 		int buff_len;	//length of the buff value
 * 	returns:
 * 		EXIT_SUCCESS;	//default return
 */
static int get_hex_string(uint8_t *buff, char *ret, int buff_len)
{
	int i;	//index for ret
	int j;	//index for buff
	uint8_t nibble;

	for (i = 0, j = 0; i < buff_len*2; i++)
	{
		nibble = 20;	//out of bounds of hex_map

		if (i % 2 == 0)
			nibble = (buff[j] >> 4) & 0x0F;
		else
		{
			nibble = buff[j] & 0x0F;
			j++;
		}

		ret[i] = hex_map[nibble];
	}

	return EXIT_SUCCESS;
}

/*	EXTERNAL */
//for function details, see rn2483.h
int		RN2483_init(uint8_t *dev_addr)
{
	//init QUARK_UART
	if (QUARK_UART_init() != EXIT_SUCCESS)	// NEEDS CHANGING FOR IF QUARK_UART CHANGES
		return EXIT_FAILURE;

	//set keys & ids
	init_mac();

	return EXIT_SUCCESS;
}
//for function details, see rn2483.h
int 	RN2483_write(const char *command)
{
	//stdout stream prints to uart interface
	printf("%s\r\n", command);	//rn2483 commands "\r\n"

	//clk_sys_udelay so next write isn't ignored while this one is being read
	clk_sys_udelay(DELAY_WRITE);

	return EXIT_SUCCESS;
}
//for function details, see rn2483.h
int		RN2483_join()
{
	//run join command
	RN2483_write("mac join " CFG_LoRa_JoinMode);

	//wait for success/fail response
	clk_sys_udelay(10000000);

	return EXIT_SUCCESS;
}
//for function details, see rn2483.h
int		RN2483_tx(void *buff, uint buff_len)
{
	//set max payload length
	int len = 0;
	if(strcmp(CFG_LoRa_DataRate, "0") == 0 || strcmp(CFG_LoRa_DataRate, "1") == 0 || strcmp(CFG_LoRa_DataRate, "2") == 0)
		len = 59;
	else if(strcmp(CFG_LoRa_DataRate, "3") == 0)
		len = 123;
	else if(strcmp(CFG_LoRa_DataRate, "4") == 0 || strcmp(CFG_LoRa_DataRate, "5") == 0 || strcmp(CFG_LoRa_DataRate, "6") == 0 || strcmp(CFG_LoRa_DataRate, "7") == 0)
		len = 230;
	else
		len = 230;

	//get payload
	int payload_len = buff_len*2;	//1 byte = 2 nibbles; buff = bytes payload = nibbles
	char *payload = malloc(payload_len);
	get_hex_string((uint8_t *)buff, payload, buff_len);

	//create tx command
	char *msg = malloc(len);
	sprintf(msg, "mac tx cnf %d %s", PORT, payload);
	RN2483_write(msg);

	//free allocated memory
	free(msg);
	free(payload);

	clk_sys_udelay(CFG_LoRa_ReTxDelay);

	return EXIT_SUCCESS;
}
