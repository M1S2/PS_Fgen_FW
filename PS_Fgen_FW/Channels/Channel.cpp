/*
 * Channel.cpp
 * Created: 03.02.2021 20:40:24
 * Author: Markus Scheich
 */ 

#include "Channel.h"

Channel::Channel(ChannelTypes_t channelType)
{
	_channelType = channelType;
}

ChannelTypes_t Channel::GetChannelType()
{
	return _channelType;
}