/*
 * DMM_Channel.cpp
 * Created: 12.02.2021 18:39:17
 * Author: Markus Scheich
 */ 

#include "DMM_Channel.h"
#include "../Device.h"

#ifdef MEASURE_SUBSYSTEM_ENABLED

DMM_Channel::DMM_Channel() : Channel(DMM_CHANNEL_TYPE)
{
	
}

#endif /* MEASURE_SUBSYSTEM_ENABLED */