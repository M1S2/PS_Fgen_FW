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

typedef enum PsStates
{
	PS_STATE_CV,
	PS_STATE_CC,
	PS_STATE_OVP,
	PS_STATE_OCP,
	PS_STATE_OPP
} PsStates_t;


class PS_Channel : public Channel
{
	private:
		PsStates_t _psState;
	
	public:		
		Parameter<bool> Enabled;
		Parameter<float> Amplitude;
		Parameter<float> LoadImpedance;
		
		Parameter<bool> OvpState;
		Parameter<uint8_t> OvpLevel;		// OVP trip level in percentage of the Amplitude
		Parameter<float> OvpDelay;
		
		float MeasuredAmplitude;
		float MeasuredCurrent;
		float MeasuredPower;
		
		uint16_t TimeCounter_OvpDelay_ms;
		
		PS_Channel(float minAmpl, float maxAmpl, float minLoad, float maxLoad, uint8_t minOvpLevel, uint8_t maxOvpLevel, float minOvpDelay, float maxOvpDelay);
		void SwitchOffOutput();
		void UpdateOutput();
	
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
	
		PsStates_t GetPsState();
	
		bool SetEnabled(bool enabled);
		bool GetEnabled();

		bool SetAmplitude(float amplitude);
		float GetAmplitude();

		bool SetLoadImpedance(float loadImpedance);
		float GetLoadImpedance();
		
		bool SetOvpLevel(uint8_t ovpLevel);
		uint8_t GetOvpLevel();

		bool SetOvpState(bool ovpState);
		bool GetOvpState();
		
		bool SetOvpDelay(float ovpDelay);
		float GetOvpDelay();
	
		void ClearProtections();
	
		static void PSEnabledChanged(void* channel);
		static void PSAmplitudeChanged(void* channel);
		static void PSLoadImpedanceChanged(void* channel);
		static void PSOvpLevelChanged(void* channel);
		static void PSOvpStateChanged(void* channel);
		static void PSOvpDelayChanged(void* channel);
};

#endif /* POWERSUPPLY_H_ */