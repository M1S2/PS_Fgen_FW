/*
 * scpi_device_implementation2.c
 *
 * Created: 17.11.2020 21:17:00
 *  Author: V17
 */ 

#include "../libs/scpi_parser/scpi.h"
#include "../USART/USART.h"

// Newline sequence for responses.
// Device accepts botn \r\n and \n in incomming messages.
const char *scpi_eol = "\r\n";

const SCPI_error_desc scpi_user_errors[] =
{
	//{10, "Custom error"}, // add your custom errors here (positive numbers)
	{/*END*/} // <-- end marker
};

void scpi_send_byte_impl(uint8_t b)
{
	// send the byte to master over UART
	Usart0Transmit(b);
}

const char *scpi_user_IDN(void)
{
	// possible to eg. read a serial # from EEPROM
	//return "<manufacturer>,<product>,<serial#>,<version>";
	return "Markus Scheich,PowerSupplySigGen,0,v1.0";
	
}

// ---- OPTIONAL CALLBACKS ----

/** Error callback */
void scpi_user_error(int16_t errno, const char * msg)
{
	//printf("### ERROR ADDED: %d, %s ###\n", errno, msg);
}

/** Service request impl */
void scpi_user_SRQ(void)
{
	// Called when the SRQ flag in Status Byte is set.
	// Device should somehow send the request to master.
}

// Device specific implementation of common commands
// (status registers etc are handled internally)
void scpi_user_CLS(void) { /*...*/ }
void scpi_user_RST(void) { /*...*/ }
void scpi_user_TSTq(void) { /*...*/ }