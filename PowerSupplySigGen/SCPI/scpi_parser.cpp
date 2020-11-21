/*
 * SCPI_Parser.cpp
 *
 * Created: 18.11.2020 19:30:06
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include "SCPI_Parser.h"
#include "../USART/USART.h"

SCPI_Parser SCPIparser (&Usart0TransmitStr, "\r\n");

SCPI_Parser::SCPI_Parser(SCPI_send_str_t sendStrFunction, const char* termChars)
{
	tokens_size_ = 0;
	codes_size_ = 0;
	tree_code_ = 1;
	msg_counter_ = 0;
	sendStrFunction_ = sendStrFunction;
	termChars_ = termChars;
	scpi_parser_init_buildin_commands();
}

void SCPI_Parser::addToken(char *token)
{
	size_t token_size = strlen(token);
	bool isQuery = (token[token_size - 1] == '?');
	if (isQuery) token_size--;

	bool allready_added = false;
	for (uint8_t i = 0; i < tokens_size_; i++)
	allready_added ^= (strncmp(token, tokens_[i], token_size) == 0);
	if (!allready_added)
	{
		if (tokens_size_ < SCPI_MAX_TOKENS)
		{
			//char *stored_token = new char [token_size + 1];
			char* stored_token = (char*)malloc((token_size + 1) * sizeof(char));
			strncpy(stored_token, token, token_size);
			stored_token[token_size] = '\0';
			tokens_[tokens_size_] = stored_token;
			tokens_size_++;
		}
	}
}

uint32_t SCPI_Parser::getCommandCode(SCPI_Commands& commands)
{		
	//TODO Use a hash function instead of "base_SCPI_MAX_TOKENS numbers".
	uint32_t code = tree_code_ - 1; // tree_code = 1 when execute
	bool isQuery = false;
	for (uint8_t i = 0; i < commands.Size(); i++)
	{
		code *= SCPI_MAX_TOKENS;
		
		size_t header_length = strlen(commands[i]);  //header's length
		if (i == commands.Size() - 1) //Last header
		{
			isQuery = (commands[i][header_length - 1] == '?');
			if (isQuery) header_length--;
		}
		
		bool isToken;
		for (uint8_t j = 0; j < tokens_size_; j++)
		{
			size_t short_length = 0; //short token's length
			while (isupper(tokens_[j][short_length])) short_length++;
			size_t long_length = strlen(tokens_[j]); //long token's length

			if ( (tokens_[j][long_length - 1] == '#') //Numeric suffix capable token
			&& (commands[i][header_length - 1] != '#') )
			{
				long_length--;
				while (isdigit(commands[i][header_length - 1])) header_length--;
			}

			isToken = true;
			if (header_length == short_length) //match with short token
			{
				for (uint8_t k  = 0; k < short_length; k++)
				isToken &= (toupper(commands[i][k]) == tokens_[j][k]);
			}
			else if (header_length == long_length) //match with long token
			{
				for (uint8_t k  = 0; k < long_length; k++)
				isToken &= (toupper(commands[i][k]) == toupper(tokens_[j][k]));
			}
			else
			{
				isToken = false;
			}
			if (isToken)
			{
				code += j;
				
				uint8_t suffixLength = strlen(commands[i]) - header_length;
				if(suffixLength == 0)
				{
					commands.numeric_suffixes[i][0] = '1';
					suffixLength = 1;
				}
				else
				{
					for(uint8_t suffixCharIdx = 0; suffixCharIdx < suffixLength; suffixCharIdx++)
					{
						commands.numeric_suffixes[i][suffixCharIdx] = commands[i][header_length + suffixCharIdx];
					}
				}
				commands.numeric_suffixes[i][suffixLength] = '\0';
				break;
			}
		}
		if (!isToken) return 0;
	}
	if (isQuery) code ^= 0x80000000;
	return code+1;
}

void SCPI_Parser::SetCommandTreeBase(const char* tree_base)
{
	strcpy(msg_buffer, tree_base);
	this->SetCommandTreeBase(msg_buffer);
}

void SCPI_Parser::SetCommandTreeBase(char* tree_base)
{
	if (strlen(tree_base) > 0)
	{
		SCPI_Commands tree_tokens(tree_base);
		for (uint8_t i = 0; i < tree_tokens.Size(); i++)
		this->addToken(tree_tokens[i]);
		tree_code_ = 1;
		tree_code_ = this->getCommandCode(tree_tokens);
	}
	else
	{
		tree_code_ = 1;
	}
}

void SCPI_Parser::RegisterCommand(const char* command, SCPI_caller_t caller)
{
	strcpy(msg_buffer, command);
	this->RegisterCommand(msg_buffer, caller);
}

void SCPI_Parser::RegisterCommand(char* command, SCPI_caller_t caller)
{	
	SCPI_Commands command_tokens(command);	
	for (uint8_t i = 0; i < command_tokens.Size(); i++)
	{
		this->addToken(command_tokens[i]);
	}
	uint32_t code = this->getCommandCode(command_tokens);
	valid_codes_[codes_size_] = code;
	callers_[codes_size_] = caller;
	codes_size_++;
}

void SCPI_Parser::Execute(char* message)
{
	tree_code_ = 1;
	SCPI_Commands commands(message);	
	SCPI_Parameters parameters(commands.not_processed_message);	
	uint32_t code = this->getCommandCode(commands);	
	for (uint8_t i = 0; i < codes_size_; i++)
	if (valid_codes_[i] == code && code != 0)
	(*callers_[i])(commands, parameters, errorQueue_, sendStrFunction_);
}

void SCPI_Parser::ProcessInput(const char* inputStr)
{
	while (*inputStr != 0)
	{
		ProcessInput(*inputStr);
		inputStr++;
	}
}

void SCPI_Parser::ProcessInput(const char inputByte)
{
	this->msg_buffer[msg_counter_] = inputByte;

	++msg_counter_;
	if (msg_counter_ >= SCPI_BUFFER_LENGTH)
	{
		msg_counter_ = 0;
		/* Buffer overflow */
	}
	msg_buffer[msg_counter_] = '\0';
	if (strstr(msg_buffer, termChars_) != NULL)
	{
		this->msg_buffer[msg_counter_ - strlen(termChars_)] = '\0';
		
		/* Process received command */
		this->Execute(this->msg_buffer);
		msg_counter_ = 0;
	}
}
