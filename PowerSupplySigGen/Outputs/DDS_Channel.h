/*
 * DDS_Channel.h
 *
 * Created: 09.01.2021 16:58:41
 *  Author: V17
 */ 


#ifndef DDS_CHANNEL_H_
#define DDS_CHANNEL_H_

#include "DDS.h"
#include "Channel.h"

#define DDSCHANNEL(MINFREQ, MAXFREQ, MINAMPL, MAXAMPL, MINOFFSET, MAXOFFSET) DDS_Channel(MINFREQ, MAXFREQ, MINAMPL, MAXAMPL, MINOFFSET, MAXOFFSET)

class DDS_Channel : public Channel
{
	public:
		const uint16_t* OriginalWaveTable;
		uint16_t WaveTable[(1 << DDS_QUANTIZER_BITS)];	// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
		uint16_t Accumulator;
		uint16_t Increment = 1024;

		DDS_Channel(float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset);
		void UpdateIncrement();
		void UpdateWaveTable();
		void UpdateOriginalWaveTable();

		static void DDSFrequencyChanged(void* channel);
		static void DDSAmplitudeChanged(void* channel);
		static void DDSOffsetChanged(void* channel);
		static void DDSSignalFormChanged(void* channel);
		static void DDSEnabledChanged(void* channel);
};

#endif /* DDS_CHANNEL_H_ */