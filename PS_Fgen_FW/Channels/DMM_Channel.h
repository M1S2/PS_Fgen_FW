/**
 * @file	DMM_Channel.h
 * @date	12.02.2021 18:39:08
 * @author	V17
 * @brief	Containing an implementation for a digital multimeter channel.
 */ 

#ifndef DMM_CHANNEL_H_
#define DMM_CHANNEL_H_

#include "../Configuration.h"
#include "Channel.h"

/**
 * Digital multimeter channel.
 * This is a channel holding all parameters of a DMM implementation.
 */
class DMM_Channel : public Channel
{
	public:
		float MeasuredVoltage;				/**< Voltage measured for this DMM channel. */
	
		/**
		 * Constructor of the DMM_Channel.
		 */
		DMM_Channel();
};

#endif /* DMM_CHANNEL_H_ */