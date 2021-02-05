/*
 * PS_Channel.h
 *
 * Created: 23.09.2020 19:47:56
 *  Author: V17
 */ 


#ifndef POWERSUPPLY_H_
#define POWERSUPPLY_H_

#include "DAC_MCP492x.h"
#include "../Configuration.h"
#include "Channel.h"

#define PSCHANNEL(MINAMPL, MAXAMPL, MINLOAD, MAXLOAD) PS_Channel(MINAMPL, MAXAMPL, MINLOAD, MAXLOAD)

class PS_Channel : public Channel
{
	public:
		Parameter<bool> Enabled;
		Parameter<float> Amplitude;
		Parameter<float> LoadImpedance;
		
		PS_Channel(float minAmpl, float maxAmpl, float minLoad, float maxLoad);
		void UpdateOutput();
	
		bool SetEnabled(bool enabled);
		bool GetEnabled();

		bool SetAmplitude(float amplitude);
		float GetAmplitude();

		bool SetLoadImpedance(float loadImpedance);
		float GetLoadImpedance();
	
		static void PSEnabledChanged(void* channel);
		static void PSAmplitudeChanged(void* channel);
		static void PSLoadImpedanceChanged(void* channel);
};

#endif /* POWERSUPPLY_H_ */