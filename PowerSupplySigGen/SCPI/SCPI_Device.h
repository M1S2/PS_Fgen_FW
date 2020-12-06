/*
 * SCPI_Device.h
 *
 * Created: 01.12.2020 18:08:03
 *  Author: V17
 */ 


#ifndef SCPI_DEVICE_H_
#define SCPI_DEVICE_H_

#include "../libraries/scpi-parser/inc/scpi/parser.h"
#include "../libraries/scpi-parser/inc/scpi/ieee488.h"
#include "../libraries/scpi-parser/inc/scpi/error.h"
#include "../libraries/scpi-parser/inc/scpi/constants.h"
#include "../libraries/scpi-parser/inc/scpi/minimal.h"
#include "../libraries/scpi-parser/inc/scpi/units.h"
#include "../libraries/scpi-parser/inc/scpi/utils.h"
#include "../libraries/scpi-parser/inc/scpi/expression.h"

#define SCPI_INPUT_BUFFER_LENGTH 256
#define SCPI_ERROR_QUEUE_SIZE 17
#define SCPI_IDN1 "Markus Scheich"
#define SCPI_IDN2 "PowerSupplySigGen"
#define SCPI_IDN3 NULL
#define SCPI_IDN4 "01-02"

extern scpi_t scpi_context;

size_t SCPI_Write(scpi_t * context, const char * data, size_t len);
scpi_result_t SCPI_Flush(scpi_t * context);
int SCPI_Error(scpi_t * context, int_fast16_t err);
scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val);
scpi_result_t SCPI_Reset(scpi_t * context);

void SCPI_Init_Device();
bool SCPI_GetVoltageFromParam(scpi_t* context, const scpi_number_t& param, float& value, float minVoltage, float maxVoltage, float defVoltage, float stepVoltage);

/******* SCPI_Cmds_Output.cpp ****************************/
scpi_result_t scpi_cmd_outputState(scpi_t * context);
scpi_result_t scpi_cmd_outputStateQ(scpi_t * context);

/******* SCPI_Cmds_Source.cpp ****************************/
scpi_result_t scpi_cmd_sourceVoltage(scpi_t * context);
scpi_result_t scpi_cmd_sourceVoltageQ(scpi_t * context);

/******* SCPI_Cmds_System.cpp ****************************/
scpi_result_t scpi_cmd_systemLocal(scpi_t * context);
scpi_result_t scpi_cmd_systemRemote(scpi_t * context);
scpi_result_t scpi_cmd_systemRWLock(scpi_t * context);

/******* SCPI_Cmds_Memory.cpp ****************************/
scpi_result_t scpi_cmd_memoryRcl(scpi_t * context);
scpi_result_t scpi_cmd_memorySav(scpi_t * context);
scpi_result_t scpi_cmd_memoryNStatesQ(scpi_t * context);

/******* SCPI_Cmds_Display.cpp ****************************/
scpi_result_t scpi_cmd_displayEnable(scpi_t * context);
scpi_result_t scpi_cmd_displayEnableQ(scpi_t * context);

#endif /* SCPI_DEVICE_H_ */