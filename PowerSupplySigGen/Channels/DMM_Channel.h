/*
 * DMM_Channel.h
 *
 * Created: 12.02.2021 18:39:08
 *  Author: V17
 */ 


#ifndef DMM_CHANNEL_H_
#define DMM_CHANNEL_H_

#include "../Configuration.h"
#include "Channel.h"

class DMM_Channel : public Channel
{
	public:
		float MeasuredVoltage;
	
		DMM_Channel();
};

#endif /* DMM_CHANNEL_H_ */