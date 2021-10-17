/**
 * @file	Channel.h
 * @date	03.02.2021 20:39:32
 * @author	Markus Scheich
 * @brief	Containing the base class for all channels.
 */ 

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "Parameter.h"
#include <stddef.h>

/**
 * Available channel types.
 * Enumeration with all different channel types.
 */
typedef enum ChannelTypes
{
	#ifdef PS_SUBSYSTEM_ENABLED
		POWER_SUPPLY_CHANNEL_TYPE,	/**< Power supply channel type. */ 
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		DDS_CHANNEL_TYPE,			/**< Direct digital synthesis channel type. */
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		DMM_CHANNEL_TYPE			/**< Digital multimeter channel type. */ 
	#endif
} ChannelTypes_t;

/**
 * Channel base class.
 * This is the base class that should be used for all specific channel implementations.
 */
class Channel
{
	protected:
		/**
		 * Type of the channel.
		 */
		ChannelTypes_t _channelType;

	public:
		/**
		 * Constructor of the Channel class
		 * @param channelType Type of the channel.
		 */
		Channel(ChannelTypes_t channelType)
		{
			_channelType = channelType;
		}

		/**
		 * Function that can be used to return the _channelType property.
		 * @return Type of the channel
		 */
		inline ChannelTypes_t GetChannelType() { return _channelType; }
};

#endif /* CHANNEL_H_ */