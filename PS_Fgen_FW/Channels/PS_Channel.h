/**
 * @file	PS_Channel.h
 * @date	23.09.2020 19:47:56
 * @author	Markus Scheich
 * @brief	Containing an implementation for a power supply channel.
 */ 

#ifndef POWERSUPPLY_H_
#define POWERSUPPLY_H_

#include "DAC_MCP492x.h"
#include "../Configuration.h"
#include "Channel.h"

#ifdef PS_SUBSYSTEM_ENABLED

/**
 * Available power supply states.
 * Enumeration with all power supply states.
 */
typedef enum PsStates
{
	PS_STATE_CV,			/**< Power supply state constant voltage */
	PS_STATE_CC,			/**< Power supply state constant current */
	PS_STATE_OVL,			/**< Power supply state overload. The channel isn't able to regulate the voltage (or current) to the desired value, because it is at it's limits and running in open-loop. */
	PS_STATE_OVP,			/**< Power supply state over voltage protection */
	PS_STATE_OCP,			/**< Power supply state over current protection */
	PS_STATE_OPP,			/**< Power supply state over power protection */
	NUM_PS_STATE_ELEMENTS	/**< The last element is used to determine the number of elements in the enumeration */
} PsStates_t;
extern const char* PSStatesNames[];

/**
 * Power supply channel Class.
 * This is a channel holding all parameters of a power supply implementation.
 *
 * The voltage is regulated by a PID regulator. 
 *
 * An anti-windup provision is implemented on the PID integrator to prevent deep saturation (aka integrator windup) when the output is saturated:
 * - The new control output with the latest integrator value is computed.
 * - If the control output exceeds either output limit, <i>and</i> the latest change in the integrator is in the same direction, then the new integrator value is not saved for the next call.
 * - Otherwise, the integrator is saved for the next call.
 *
 * @see https://rn-wissen.de/wiki/index.php/Regelungstechnik (for general PID regulator code)
 * @see https://www.embeddedrelated.com/showcode/346.php (for PID anti-windup scheme)
 */
class PS_Channel : public Channel
{	
	private:
		float _PIDVoltErrorSum;				/**< PID voltage regulator error sum */
		float _PIDVoltErrorLast;			/**< PID voltage regulator last error */
		float _setVoltage;					/**< Voltage to which the output should be set. This value is calculated by the PID voltage regulator. */
	
	public:
		PsStates_t PsState;					/**< Current state of the power supply channel. */
	
		Parameter<bool> Enabled;			/**< Is the channel enabled or not. If enabled, the voltage is available at the output. */
		Parameter<float> Voltage;			/**< Voltage of the power supply channel. This is the voltage that the PID regulator tries to produce on the output in CV state. */
		Parameter<float> Current;			/**< Current of the power supply channel. This is the current that the PID regulator tries to produce on the output in CC state. */
		
		Parameter<bool> OvpState;			/**< Is the over voltage protection for the channel enabled or not. If disabled, the OvpLevel and OvpDelay parameters have not effect. */
		Parameter<uint8_t> OvpLevel;		/**< OVP trip level in percentage of the Voltage. */
		Parameter<float> OvpDelay;			/**< Time after which the over voltage protection kicks in. */
			
		Parameter<bool> OcpState;			/**< Is the over current protection for the channel enabled or not. If disabled, the OcpLevel and OcpDelay parameters have not effect. */
		Parameter<uint8_t> OcpLevel;		/**< OCP trip level in percentage of the Current. */
		Parameter<float> OcpDelay;			/**< Time after which the over current protection kicks in. */
		
		Parameter<bool> OppState;			/**< Is the over power protection for the channel enabled or not. If disabled, the OppLevel and OppDelay parameters have not effect. */
		Parameter<float> OppLevel;			/**< OPP trip level in Watt. */
		Parameter<float> OppDelay;			/**< Time after which the over power protection kicks in. */
		
		float MeasuredVoltage;				/**< Measured Voltage for this channel. This value is used for PID regulation of the output voltage in CV state. */
		float MeasuredCurrent;				/**< Measured Current for this channel. This value is used for PID regulation of the output current in CC state. */
		float MeasuredPower;				/**< Measured Power for this channel. This value is calculated from the MeasuredVoltage and MeasuredCurrent. */
		
		uint16_t TimeCounter_OvpDelay_ms;	/**< Time counter used to measure how long the channel has an over voltage. If this value exceeds the OvpDelay, the over voltage protection kicks in. */
		uint16_t TimeCounter_OcpDelay_ms;	/**< Time counter used to measure how long the channel has an over current. If this value exceeds the OcpDelay, the over current protection kicks in. */
		uint16_t TimeCounter_OppDelay_ms;	/**< Time counter used to measure how long the channel has an over power. If this value exceeds the OppDelay, the over power protection kicks in. */
		
		/**
		 * Constructor of the PS_Channel.
		 * @param minVolt Minimum allowed voltage for the PS channel
		 * @param maxVolt Maximum allowed voltage for the PS channel
		 * @param minCurrent Minimum allowed current for the PS channel
		 * @param maxCurrent Maximum allowed current for the PS channel
 		 * @param minOvpLevel Minimum allowed Ovp level for the PS channel
 		 * @param maxOvpLevel Maximum allowed Ovp level for the PS channel
		 * @param minOvpDelay Minimum allowed Ovp delay for the PS channel
		 * @param maxOvpDelay Maximum allowed Ovp delay for the PS channel
		 * @param minOcpLevel Minimum allowed Ocp level for the PS channel
		 * @param maxOcpLevel Maximum allowed Ocp level for the PS channel
		 * @param minOcpDelay Minimum allowed Ocp delay for the PS channel
		 * @param maxOcpDelay Maximum allowed Ocp delay for the PS channel
		 * @param minOppLevel Minimum allowed Opp level for the PS channel
		 * @param maxOppLevel Maximum allowed Opp level for the PS channel
		 * @param minOppDelay Minimum allowed Opp delay for the PS channel
		 * @param maxOppDelay Maximum allowed Opp delay for the PS channel
		 */
		PS_Channel(float minVolt, float maxVolt, float minCurrent, float maxCurrent, uint8_t minOvpLevel, uint8_t maxOvpLevel, float minOvpDelay, float maxOvpDelay, uint8_t minOcpLevel, uint8_t maxOcpLevel, float minOcpDelay, float maxOcpDelay, float minOppLevel, float maxOppLevel, float minOppDelay, float maxOppDelay);
		
		/** Set the voltage at the output of this channel to zero. */
		void SwitchOffOutput();
		
		/** Set the voltage at the output of this channel to the value calculated by the PID regulator if the output is enabled and in CV state. Otherwise switch off the output. */
		void UpdateOutput();
	
		/**
		 * This method is called periodically by the device timer ISR and runs in the ISR context.
		 * It is used to increment the protection timers if neccessary, to change the channel state if an protection kicks in and to calculate a new PID value.
		 * @param currentPeriod_ms Tick period of the device timer at which this method is called.
		 */
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
	
		/**
		 * Return the current state of this channel.
		 * @return Current power supply channel state.
		 */
		PsStates_t GetPsState();
	
		/**
		 * Set the Enabled property of the PS channel.
		 * @param enabled New value for the Enabled property
		 * @return true->set successful; false->value not set
		 */
		bool SetEnabled(bool enabled);
		/**
		 * Get the Enabled property of the PS channel.
		 * @return Value of the Enabled property
		 */
		bool GetEnabled();

		/**
		 * Set the Voltage property of the PS channel.
		 * @param voltage New value for the Voltage property
		 * @return true->set successful; false->value not set
		 */
		bool SetVoltage(float voltage);
		/**
		 * Get the Voltage property of the PS channel.
		 * @return Value of the Voltage property
		 */
		float GetVoltage();

		/**
		 * Set the Current property of the PS channel.
		 * @param current New value for the Current property
		 * @return true->set successful; false->value not set
		 */
		bool SetCurrent(float current);
		/**
		 * Get the Current property of the PS channel.
		 * @return Value of the Current property
		 */
		float GetCurrent();
		
		/**
		 * Set the OvpLevel property of the PS channel.
		 * @param ovpLevel New value for the OvpLevel property
		 * @return true->set successful; false->value not set
		 */
		bool SetOvpLevel(uint8_t ovpLevel);
		/**
		 * Get the OvpLevel property of the PS channel.
		 * @return Value of the OvpLevel property
		 */
		uint8_t GetOvpLevel();

		/**
		 * Set the OvpState property of the PS channel.
		 * @param ovpState New value for the OvpState property
		 * @return true->set successful; false->value not set
		 */
		bool SetOvpState(bool ovpState);
		/**
		 * Get the OvpState property of the PS channel.
		 * @return Value of the OvpState property
		 */
		bool GetOvpState();
		
		/**
		 * Set the OvpDelay property of the PS channel.
		 * @param ovpDelay New value for the OvpDelay property
		 * @return true->set successful; false->value not set
		 */
		bool SetOvpDelay(float ovpDelay);
		/**
		 * Get the OvpDelay property of the PS channel.
		 * @return Value of the OvpDelay property
		 */
		float GetOvpDelay();

		/**
		 * Set the OcpLevel property of the PS channel.
		 * @param ocpLevel New value for the OcpLevel property
		 * @return true->set successful; false->value not set
		 */
		bool SetOcpLevel(uint8_t ocpLevel);
		/**
		 * Get the OcpLevel property of the PS channel.
		 * @return Value of the OcpLevel property
		 */
		uint8_t GetOcpLevel();

		/**
		 * Set the OcpState property of the PS channel.
		 * @param ocpState New value for the OcpState property
		 * @return true->set successful; false->value not set
		 */
		bool SetOcpState(bool ocpState);
		/**
		 * Get the OcpState property of the PS channel.
		 * @return Value of the OcpState property
		 */
		bool GetOcpState();

		/**
		 * Set the OcpDelay property of the PS channel.
		 * @param ocpDelay New value for the OcpDelay property
		 * @return true->set successful; false->value not set
		 */		
		bool SetOcpDelay(float ocpDelay);
		/**
		 * Get the OcpDelay property of the PS channel.
		 * @return Value of the OcpDelay property
		 */
		float GetOcpDelay();

		/**
		 * Set the OppLevel property of the PS channel.
		 * @param oppLevel New value for the OppLevel property
		 * @return true->set successful; false->value not set
		 */
		bool SetOppLevel(float oppLevel);
		/**
		 * Get the OppLevel property of the PS channel.
		 * @return Value of the OppLevel property
		 */
		float GetOppLevel();

		/**
		 * Set the OppState property of the PS channel.
		 * @param oppState New value for the OppState property
		 * @return true->set successful; false->value not set
		 */
		bool SetOppState(bool oppState);
		/**
		 * Get the OppState property of the PS channel.
		 * @return Value of the OppState property
		 */
		bool GetOppState();
		
		/**
		 * Set the OppDelay property of the PS channel.
		 * @param oppDelay New value for the OppDelay property
		 * @return true->set successful; false->value not set
		 */
		bool SetOppDelay(float oppDelay);
		/**
		 * Get the OppDelay property of the PS channel.
		 * @return Value of the OppDelay property
		 */
		float GetOppDelay();
	
	
		/**
		 * Clear the protections for the power supply channel and return to the CV state.
		 */
		void ClearProtections();
	
	
		/**
		 * Callback function that can be used for user interface controls modifying the Enabled property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSEnabledChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the Voltage property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSVoltageChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the Current property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSCurrentChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the OvpLevel property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOvpLevelChanged(void* channel);
		/**
		 * Callback function that can be used for user interface controls modifying the OvpState property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOvpStateChanged(void* channel);
		/**
		 * Callback function that can be used for user interface controls modifying the OvpDelay property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOvpDelayChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the OcpLevel property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOcpLevelChanged(void* channel);
		/**
		 * Callback function that can be used for user interface controls modifying the OcpState property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOcpStateChanged(void* channel);
		/**
		 * Callback function that can be used for user interface controls modifying the OcpDelay property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOcpDelayChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the OppLevel property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOppLevelChanged(void* channel);
		/**
		 * Callback function that can be used for user interface controls modifying the OppState property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOppStateChanged(void* channel);
		/**
		 * Callback function that can be used for user interface controls modifying the OppDelay property.
		 * @param channel Pointer to a PS_Channel
		 */
		static void PSOppDelayChanged(void* channel);
};

#endif /* POWERSUPPLY_H_ */

#endif /* PS_SUBSYSTEM_ENABLED */