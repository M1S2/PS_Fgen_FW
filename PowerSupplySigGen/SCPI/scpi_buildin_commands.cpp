/*
 * scpi_buildin_commands.cpp
 *
 * Created: 21.11.2020 18:21:24
 *  Author: V17
 */ 

#include "SCPI_Parser.h"
#include "../Global/DevSettings.h"

static char sbuf[MAX_ERROR_LEN + 1];	// must be long enough to contain an error message

void SCPI_builtin_CLS(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	SCPIparser.ErrorQueue.ClearErrors();
}

void SCPI_builtin_ESE(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_ESEq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_ESRq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_buildin_IDNq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Markus Scheich,PowerSupplySigGen,0,v1.0\r\n"); }
}

void SCPI_builtin_OPC(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_OPCq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("1\r\n"); }
}

void SCPI_builtin_RST(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	ResetDevice();
}

void SCPI_builtin_SRE(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_SREq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_STBq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_WAI(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

void SCPI_builtin_TSTq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("Not implemented.\r\n"); }
}

/********************************** ERROR Subsystem **************************************************************************************************************/

void SCPI_builtin_SYST_ERR_NEXTq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	SCPIparser.ErrorQueue.ReadError(sbuf);
	if(sendFunction != NULL) { sendFunction(sbuf); sendFunction("\r\n"); }		
}

void SCPI_builtin_SYST_ERR_ALLq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if (SCPIparser.ErrorQueue.ErrorCount() > 0) 
	{
		int cnt = 0;
		while (SCPIparser.ErrorQueue.ErrorCount() > 0) 
		{
			SCPIparser.ErrorQueue.ReadError(sbuf);
			if(sendFunction != NULL)
			{
				if (cnt++ > 0) { sendFunction(","); }
				sendFunction(sbuf);
				sendFunction("\r\n");
			}
		}
	} 
	else 
	{
		SCPIparser.ErrorQueue.ReadError(sbuf);
		if(sendFunction != NULL) { sendFunction(sbuf); sendFunction("\r\n"); }
	}
}

void SCPI_builtin_SYST_ERR_CLEAR(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	SCPIparser.ErrorQueue.ClearErrors();
}

void SCPI_builtin_SYST_ERR_COUNq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) 
	{
		char buf[10];
		itoa(SCPIparser.ErrorQueue.ErrorCount(), buf, 10);
		sendFunction(buf);
		sendFunction("\r\n"); 
	}
}

void SCPI_builtin_SYST_ERR_CODE_NEXTq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	SCPIparser.ErrorQueue.ReadError(sbuf);

	// end at comma
	for (int i = 0; i < MAX_ERROR_LEN + 1; i++) 
	{
		if (sbuf[i] == ',') 
		{
			sbuf[i] = 0;
			break;
		}
	}

	if(sendFunction != NULL) { sendFunction(sbuf); sendFunction("\r\n"); }
}

void SCPI_builtin_SYST_ERR_CODE_ALLq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if (SCPIparser.ErrorQueue.ErrorCount() > 0)
	{
		int cnt = 0;
		while (SCPIparser.ErrorQueue.ErrorCount() > 0) 
		{
			SCPIparser.ErrorQueue.ReadError(sbuf);
			if(sendFunction != NULL) 
			{
				if (cnt++ > 0) { sendFunction(","); }

				// end at comma
				for (int i = 0; i < MAX_ERROR_LEN + 1; i++) 
				{
					if (sbuf[i] == ',') 
					{
						sbuf[i] = 0;
						break;
					}
				}
				sendFunction(sbuf);
			}
		}
		sendFunction("\r\n");
	}
	else
	{
		sendFunction("0\r\n");
	}	
}

void SCPI_builtin_SYST_VERSq(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) { sendFunction("1999.0\r\n"); }		// implemented SCPI version
}

void scpi_parser_init_buildin_commands()
{
	SCPIparser.RegisterCommand("*CLS", &SCPI_builtin_CLS);		// Clear Status Command
	SCPIparser.RegisterCommand("*ESE", &SCPI_builtin_ESE);		// Standard Event Status Enable Command
	SCPIparser.RegisterCommand("*ESE?", &SCPI_builtin_ESEq);	// Standard Event Status Enable Query
	SCPIparser.RegisterCommand("*ESR?", &SCPI_builtin_ESRq);	// Standard Event Status Register Query
	SCPIparser.RegisterCommand("*IDN?", &SCPI_buildin_IDNq);	// Identification Query
	SCPIparser.RegisterCommand("*OPC", &SCPI_builtin_OPC);		// Operation Complete Command
	SCPIparser.RegisterCommand("*OPC?", &SCPI_builtin_OPCq);	// Operation Complete Query
	SCPIparser.RegisterCommand("*RST", &SCPI_builtin_RST);		// Reset Command
	SCPIparser.RegisterCommand("*SRE", &SCPI_builtin_SRE);		// Service Request Enable Command
	SCPIparser.RegisterCommand("*SRE?", &SCPI_builtin_SREq);	// Service Request Enable Query
	SCPIparser.RegisterCommand("*STB?", &SCPI_builtin_STBq);	// Read Status Byte Query
	SCPIparser.RegisterCommand("*WAI", &SCPI_builtin_WAI);		// Self-Test Query
	SCPIparser.RegisterCommand("*TST?", &SCPI_builtin_TSTq);	// Wait-to-Continue Command
	
	/* ERROR Subsystem */
	
	SCPIparser.RegisterCommand("SYSTem:ERRor?", &SCPI_builtin_SYST_ERR_NEXTq);
	SCPIparser.RegisterCommand("SYSTem:ERRor:NEXT?", &SCPI_builtin_SYST_ERR_NEXTq);
	SCPIparser.RegisterCommand("SYSTem:ERRor:ALL?", &SCPI_builtin_SYST_ERR_ALLq);
	SCPIparser.RegisterCommand("SYSTem:ERRor:CLEAR", &SCPI_builtin_SYST_ERR_CLEAR);
	SCPIparser.RegisterCommand("SYSTem:ERRor:COUNt?", &SCPI_builtin_SYST_ERR_COUNq);
	SCPIparser.RegisterCommand("SYSTem:ERRor:CODE?", &SCPI_builtin_SYST_ERR_CODE_NEXTq);
	SCPIparser.RegisterCommand("SYSTem:ERRor:CODE:NEXT?", &SCPI_builtin_SYST_ERR_CODE_NEXTq);
	SCPIparser.RegisterCommand("SYSTem:ERRor:CODE:ALL?", &SCPI_builtin_SYST_ERR_CODE_ALLq);
	SCPIparser.RegisterCommand("SYSTem:VERSion?", &SCPI_builtin_SYST_VERSq);
}