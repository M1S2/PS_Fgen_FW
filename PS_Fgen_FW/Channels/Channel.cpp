/*
 * Channel.cpp
 *
 * Created: 03.02.2021 20:40:24
 *  Author: V17
 */ 

#include "Channel.h"

const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE", "SAWTOOTH", "DC" };

Channel::Channel(ChannelTypes_t channelType)
{
	_channelType = channelType;
}

ChannelTypes_t Channel::GetChannelType()
{
	return _channelType;
}