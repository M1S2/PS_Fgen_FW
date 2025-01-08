/**
 * @file	DDS_Channel.h
 * @date	09.01.2021 16:58:41
 * @author	Markus Scheich
 * @brief	Containing an implementation for a direct digital synthesis channel.
 */ 

#ifndef DDS_CHANNEL_H_
#define DDS_CHANNEL_H_

#include "DDS.h"
#include "Channel.h"
#include "../Configuration.h"

/**
 * Available DDS signal forms.
 * Enumeration with all signal form types.
 */
typedef enum SignalForms
{
	SIGNALFORM_SINE,						/**< Sine wave type */
	SIGNALFORM_RECTANGLE,					/**< Rectange wave type */
	SIGNALFORM_TRIANGLE,					/**< Triangle wave type */
	SIGNALFORM_SAWTOOTH,					/**< Sawtooth wave type */
	SIGNALFORM_DC,							/**< Direct current wave type */
	SIGNALFORM_PWM,							/**< PWM wave type */
	#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
		SIGNALFORM_USER_SIGNAL,				/**< User defined wave type (signal is held in the UserWaveTable of the DDS_Channel) */
	#endif
	NUM_SIGNALFORM_ELEMENTS					/**< The last element is used to determine the number of elements in the enumeration */
} SignalForms_t;


#ifdef DDS_SUBSYSTEM_ENABLED

extern const char* SignalFormsNames[NUM_SIGNALFORM_ELEMENTS];		/**< Array holding the names for all available DDS channel signal forms. */

/**
 * Direct digital synthesis channel.
 * This is a channel holding all parameters of a DDS implementation.
 */
class DDS_Channel : public Channel
{
	public:
		uint8_t DdsChannelNumber;						/**< 1-based channel number (DDS1 => 1, DDS2 => 2) */
	
		Parameter<bool> Enabled;						/**< Is the channel enabled or not. If enabled, the waveform is available at the output. */
		Parameter<float> Frequency;						/**< Frequency of the DDS channel */
		Parameter<float> Amplitude;						/**< Amplitude of the DDS channel */
		Parameter<float> Offset;						/**< Offset of the DDS channel */
		Parameter<SignalForms_t> SignalForm;			/**< Signalform of the DDS channel */
	
		Parameter<float> PWM_Value;						/**< PWM value of the DDS channel. This is only used if the SignalForm is set to PWM */
	
		volatile const uint16_t* OriginalWaveTable;		/**< Pointer to the constant wave table holding the unmodified points of the signal determined by the SignalForm parameter */
		volatile uint16_t* p_WaveTable;					/**< Pointer to the wave table array holding the modified waveform points (adapted to waveform and offset) */
		volatile uint32_t* p_Increment;					/**< Pointer to the increment variable (calculated from the Frequency setting) */
		
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			uint16_t UserWaveTable[(1 << DDS_QUANTIZER_BITS)];	/**< Array holding the points representing a user defined signal form */
			int UserWaveTableReceiveIndex = 0;			/**< Index for filling the UserWaveTable via SCPI command. The received data is inserted into the UserWaveTable array from this index. */
		#endif

		/**
		 * Constructor of the DDS_Channel.
		 * @param ddsChannelNumber 1-based channel number (DDS1 => 1, DDS2 => 2)
		 * @param minFreq Minimum allowed frequency for the DDS channel
		 * @param maxFreq Maximum allowed frequency for the DDS channel
		 * @param minAmpl Minimum allowed amplitude for the DDS channel
		 * @param maxAmpl Maximum allowed amplitude for the DDS channel
		 * @param minOffset Minimum allowed offset for the DDS channel
		 * @param maxOffset Maximum allowed offset for the DDS channel
		 */
		DDS_Channel(uint8_t ddsChannelNumber, float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset);
		
		/** Update the increment value based on the Frequency. */
		void UpdateIncrement();
		
		/** Update the WaveTable based on the SignalForm, Amplitude and Offset. */
		void UpdateWaveTable();
		
		/** Update the pointer to the original WaveTable based on the SignalForm. */
		void UpdateOriginalWaveTable();

		/**
		 * Set the Enabled property of the DDS channel.
		 * @param enabled New value for the Enabled property
		 * @return true->set successful; false->value not set
		 */
		bool SetEnabled(bool enabled);
		/**
		 * Get the Enabled property of the DDS channel.
		 * @return Value of the Enabled property
		 */
		inline bool GetEnabled() { return Enabled.Val; }

		/**
		 * Set the Frequency property of the DDS channel.
		 * @param frequency New value for the Frequency property
		 * @return true->set successful; false->value not set
		 */
		bool SetFrequency(float frequency);
		/**
		 * Get the Frequency property of the DDS channel.
		 * @return Value of the Frequency property
		 */
		inline float GetFrequency() { return Frequency.Val; }

		/**
		 * Set the Amplitude property of the DDS channel.
		 * @param amplitude New value for the Amplitude property
		 * @return true->set successful; false->value not set
		 */
		bool SetAmplitude(float amplitude);
		/**
		 * Get the Amplitude property of the DDS channel.
		 * @return Value of the Amplitude property
		 */
		inline float GetAmplitude() { return Amplitude.Val; }

		/**
		 * Set the Offset property of the DDS channel.
		 * @param offset New value for the Offset property
		 * @return true->set successful; false->value not set
		 */
		bool SetOffset(float offset);
		/**
		 * Get the Offset property of the DDS channel.
		 * @return Value of the Offset property
		 */
		inline float GetOffset() { return Offset.Val; }

		/**
		 * Set the SignalForm property of the DDS channel.
		 * @param signalForm New value for the SignalForm property
		 * @return true->set successful; false->value not set
		 */
		bool SetSignalForm(SignalForms_t signalForm);
		/**
		 * Get the SignalForm property of the DDS channel.
		 * @return Value of the SignalForm property
		 */
		inline SignalForms_t GetSignalForm() { return SignalForm.Val; }

		/**
		 * Set the PWM_Value property of the DDS channel.
		 * @param pwmValue New value for the PWM_Value property
		 * @return true->set successful; false->value not set
		 */
		bool SetPWMValue(float pwmValue);
		/**
		 * Get the PWM_Value property of the DDS channel.
		 * @return Value of the PWM_Value property
		 */
		inline float GetPWMValue() { return PWM_Value.Val; }

		/**
		 * Callback function that can be used for user interface controls modifying the Frequency property.
		 * @param channel Pointer to a DDS_Channel
		 */
		static void DDSFrequencyChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the Amplitude property.
		 * @param channel Pointer to a DDS_Channel
		 */
		static void DDSAmplitudeChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the Offset property.
		 * @param channel Pointer to a DDS_Channel
		 */
		static void DDSOffsetChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the SignalForm property.
		 * @param channel Pointer to a DDS_Channel
		 */
		static void DDSSignalFormChanged(void* channel);

		/**
		 * Callback function that can be used for user interface controls modifying the PWM_Value property.
		 * @param channel Pointer to a DDS_Channel
		 */
		static void DDSPWMValueChanged(void* channel);
		
		/**
		 * Callback function that can be used for user interface controls modifying the Enabled property.
		 * @param channel Pointer to a DDS_Channel
		 */
		static void DDSEnabledChanged(void* channel);
};

#endif /* DDS_CHANNEL_H_ */

#endif /* DDS_SUBSYSTEM_ENABLED */