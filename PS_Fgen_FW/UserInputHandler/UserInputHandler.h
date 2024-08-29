/**
 * @file	UserInputHandler.h
 * @date	08.09.2020 19:31:51
 * @author	Markus Scheich
 * @brief	Containing a classes for user input handling and the corresponding data types.
 */ 

#ifndef USERINPUTHANDLER_H_
#define USERINPUTHANDLER_H_

#include "CircularBuffer.h"
#include "../KeyPad/KeyPad.h"
#include "../OnOffControls/OnOffControls.h"
#include <stdbool.h>

#include "../Configuration.h"

/**
 * Enum used to differentiate the user input types
 */
typedef enum UserInputDataTypes
{
	USERDATA_KEY,				/**< User data type for Keys */
	USERDATA_USART,				/**< User data type for USART input */
	USERDATA_ON_OFF_BUTTONS		/**< User data type for on/off buttons */
} UserInputDataTypes_t;

/**
 * Class that holds one user input made by keys (or encoder) or USART
 */
class UserInputData
{
	public:
		UserInputDataTypes_t DataType;		/**< Type of the user input data */
		Keys_t Key;							/**< Key pressed by the user if the DataType is USERDATA_KEY */
		uint8_t UsartChr;					/**< Character received via Usart if the DataType is USERDATA_USART */
		OnOffButtons_t OnOffButton;			/**< on/off button pressed by the user if the DataType is USERDATA_ON_OFF_BUTTONS */

		/** Empty Constructor. */	
		UserInputData() 
		{}
	
		/**
		 * Constructor for the UserInputData class.
		 * Use this constructor to initialize for a key.
		 * @param key Key that should be stored with this class.
		 */
		UserInputData(Keys_t key) : Key(key)
		{
			DataType = USERDATA_KEY;
		}
		
		/**
		 * Constructor for the UserInputData class.
		 * Use this constructor to initialize for a Usart character.
		 * @param usartChr Character that should be stored with this class.
		 */	
		UserInputData(uint8_t usartChr) : UsartChr(usartChr)
		{
			DataType = USERDATA_USART;
		}
		
		/**
		 * Constructor for the UserInputData class.
		 * Use this constructor to initialize for a physical on/off button.
		 * @param button on/off button that should be stored with this class.
		 */	
		UserInputData(OnOffButtons_t button) : OnOffButton(button)
		{
			DataType = USERDATA_ON_OFF_BUTTONS;
		}
};


/**
 * Class used to enqueue and process user inputs.
 */
class UserInputHandlerClass 
{
	private:
		CircularBuffer<UserInputData, USERINPUT_QUEUE_LENGTH> _userInputRingBuffer;		/**< Circular buffer holding all user inputs. */
		
	public:		
		/**
		 * Enqueue the given key into the circular buffer for later processing.
		 * The key is only enqueued and no further actions are taken.
		 * @param userKeyInput Key that should be enqueued.
		 */
		inline void EnqueueKeyInput(Keys_t userKeyInput)
		{
			if(!_userInputRingBuffer.full())
			{
				UserInputData keyInput(userKeyInput);
				_userInputRingBuffer.enqueue(&keyInput);
			}
		}
		
		/**
		 * Enqueue the given Usart character into the circular buffer for later processing.
		 * The character is only enqueued and no further actions are taken.
		 * @param userDataInput Usart character that should be enqueued.
		 */
		inline void EnqueueUsartInput(uint8_t userDataInput)
		{
			if(!_userInputRingBuffer.full())
			{
				UserInputData dataInput(userDataInput);
				_userInputRingBuffer.enqueue(&dataInput);
			}
		}
		
		/**
		 * Enqueue the given on/off button into the circular buffer for later processing.
		 * The button is only enqueued and no further actions are taken.
		 * @param userButtonInput Button that should be enqueued.
		 */
		inline void EnqueueOnOffButtonInput(OnOffButtons_t userButtonInput)
		{
			if(!_userInputRingBuffer.full())
			{
				UserInputData buttonInput(userButtonInput);
				_userInputRingBuffer.enqueue(&buttonInput);
			}
		}
		
		/**
		 * Process all user inputs in the circular buffer until it is empty.
		 * Keys are send to the ScreenManager, Usart characters to the SCPI parser.
		 */
		void ProcessInputs();
};

#endif /* USERINPUTHANDLER_H_ */