/*
 * scpi_parser.h
 *
 * Created: 18.11.2020 19:29:13
 *  Author: V17
 */ 

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#ifndef SCPI_PARSER_H_
#define SCPI_PARSER_H_

// Maximum size of command tree and number of parameters.
#ifndef SCPI_ARRAY_SYZE
#define SCPI_ARRAY_SYZE 6
#endif

// Maximum number of valid tokens.
#ifndef SCPI_MAX_TOKENS
#define SCPI_MAX_TOKENS 15
#endif

// Maximum number of registered commands.
#ifndef SCPI_MAX_COMMANDS
#define SCPI_MAX_COMMANDS 20
#endif

// Maximum length of numeric suffixes (e.g. 3 means maximum of 3 decimal places).
#ifndef SCPI_MAX_SUFFIX_LENGTH
#define SCPI_MAX_SUFFIX_LENGTH 3
#endif

// Length of the message buffer.
#ifndef SCPI_BUFFER_LENGTH
#define SCPI_BUFFER_LENGTH 64
#endif

class SCPI_String_Array
{
	public:
	char* operator[](const uint8_t index);
	void Append(char* value);
	char* Pop();
	char* First();
	char* Last();
	uint8_t Size();
	protected:
	uint8_t size_ = 0;
	char* values_[SCPI_ARRAY_SYZE];
};

class SCPI_Commands : public SCPI_String_Array
{
	public:
	SCPI_Commands(char* message);
	char* not_processed_message;
	char numeric_suffixes[SCPI_MAX_TOKENS][SCPI_MAX_SUFFIX_LENGTH + 1];
};

class SCPI_Parameters : public SCPI_String_Array
{
	public:
	SCPI_Parameters(char *message);
	char* not_processed_message;
};

typedef SCPI_Commands SCPI_C;
typedef SCPI_Parameters SCPI_P;
typedef void (*SCPI_send_str_t)(char*);
typedef void (*SCPI_caller_t)(SCPI_Commands, SCPI_Parameters, SCPI_send_str_t);

class SCPI_Parser
{
	public:
		void SetCommandTreeBase(char* tree_base);
		void SetCommandTreeBase(const char* tree_base);
		void RegisterCommand(char* command, SCPI_caller_t caller);
		void RegisterCommand(const char* command, SCPI_caller_t caller);
		void Execute(char* message);
		void ProcessInput(const char inputByte, const char* term_chars);
		void ProcessInput(const char* inputStr, const char* term_chars);
		void PrintDebugInfo();
	
		SCPI_send_str_t SendStrFunction;
	protected:
		void AddToken(char* token);
		uint32_t GetCommandCode(SCPI_Commands& commands);
		uint8_t tokens_size_ = 0;
		char *tokens_[SCPI_MAX_TOKENS];
		uint8_t codes_size_ = 0;
		uint32_t valid_codes_[SCPI_MAX_COMMANDS];
		SCPI_caller_t callers_[SCPI_MAX_COMMANDS];
		uint32_t tree_code_ = 1;
		char msg_buffer[SCPI_BUFFER_LENGTH];
		uint8_t msg_counter_ = 0;
};

extern SCPI_Parser SCPIparser;
void scpi_parser_init_commands();

#endif /* SCPI_PARSER_H_ */