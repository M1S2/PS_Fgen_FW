/*
 * SCPI_Parser.h
 *
 * Created: 18.11.2020 19:29:13
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "SCPI_Commands.h"
#include "SCPI_Parameters.h"
#include "SCPI_ErrorQueue.h"

#ifndef SCPI_PARSER_H_
#define SCPI_PARSER_H_

// Maximum number of registered commands.
#ifndef SCPI_MAX_COMMANDS
#define SCPI_MAX_COMMANDS 100
#endif

// Length of the message buffer.
#ifndef SCPI_BUFFER_LENGTH
#define SCPI_BUFFER_LENGTH 64
#endif

typedef void (*SCPI_send_str_t)(const char*);
typedef void (*SCPI_caller_t)(SCPI_Commands, SCPI_Parameters, SCPI_send_str_t);

class SCPI_Parser
{
	public:
		SCPI_Parser(SCPI_send_str_t sendStrFunction, const char* termChars);
		void RegisterCommand(char* command, SCPI_caller_t caller);
		void RegisterCommand(const char* command, SCPI_caller_t caller);
		void Execute(char* message);
		void ProcessInput(const char inputByte);
		void ProcessInput(const char* inputStr);
	
		SCPI_Error_Queue ErrorQueue;
	
	private:
		bool addToken(char* token);
		uint32_t getCommandCode(SCPI_Commands& commands);

		uint8_t tokens_size_;
		char *tokens_[SCPI_MAX_TOKENS];
		uint8_t codes_size_;
		uint32_t valid_codes_[SCPI_MAX_COMMANDS];
		SCPI_caller_t callers_[SCPI_MAX_COMMANDS];
		char msg_buffer[SCPI_BUFFER_LENGTH];
		uint8_t msg_counter_;
		SCPI_send_str_t sendStrFunction_;
		const char* termChars_;
};

void scpi_parser_init_buildin_commands();
void scpi_parser_init_user_commands();
extern SCPI_Parser SCPIparser;

#endif /* SCPI_PARSER_H_ */