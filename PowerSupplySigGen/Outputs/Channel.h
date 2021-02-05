/*
 * Channel.h
 *
 * Created: 03.02.2021 20:39:32
 *  Author: V17
 */ 


#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "Parameter.h"
#include <stddef.h>

typedef enum SignalForms
{
	SINE,
	RECTANGLE,
	TRIANGLE,
	SAWTOOTH
} SignalForms_t;
extern const char* SignalFormsNames[];

typedef enum ChannelTypes
{
	POWER_SUPPLY_CHANNEL_TYPE,
	DDS_CHANNEL_TYPE
} ChannelTypes_t;


class Channel
{
	protected:
		ChannelTypes_t _channelType;

	public:
		Channel(ChannelTypes_t channelType);

		ChannelTypes_t GetChannelType();
};

#endif /* CHANNEL_H_ */