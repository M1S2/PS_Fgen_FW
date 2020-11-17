/*
 * scpi_commands.c
 *
 * Created: 17.11.2020 21:19:11
 *  Author: V17
 */ 

#include "../libs/scpi_parser/scpi.h"

void cmd_OUTP_cb(const SCPI_argval_t *args)
{
	scpi_send_string("OUTP command received");
}

/* Custom commands */
const SCPI_command_t scpi_commands[] = {
	// see the struct definition for more details. Examples:
	{
		.levels = {"OUTPut"},
		.params = {SCPI_DT_BOOL},
		.callback = cmd_OUTP_cb
	},
	{/*END*/} // <-- important! Marks end of the array
};