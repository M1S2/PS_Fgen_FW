/**
 * @file	MCP23S08.h
 * @date	29.08.2024 18:54:39
 * @author	Markus Scheich
 * @brief	Containing methods to control the MCP23S08 SPI IO expander
 * Based on https://github.com/julianschuler/MCP23S08/blob/master/MCP23S08.h
 */ 

#ifndef MCP23S08_H_
#define MCP23S08_H_

#include <stdint.h>

#define SELECT_MCP23S08		SPI_SELECT_IO_EXP				/**< Select the MCP23S08 IO expander by pulling the IO_EXP_CS pin low */
#define DESELECT_MCP23S08	SPI_DESELECT_IO_EXP				/**< Deselect the MCP23S08 IO expander by pulling the IO_EXP_CS pin high */

// register addresses
#define MCP23S08_IODIR		0x00
#define MCP23S08_IPOL		0x01
#define MCP23S08_GPINTEN	0x02
#define MCP23S08_DEFVAL		0x03
#define MCP23S08_INTCON		0x04
#define MCP23S08_IOCON		0x05
#define MCP23S08_GPPU		0x06
#define MCP23S08_INTF		0x07
#define MCP23S08_INTCAP		0x08
#define MCP23S08_GPIO		0x09
#define MCP23S08_OLAT		0x0A

/**
 * Enumeration with all MCP23S08 pin modes
 */
typedef enum MCP23S08_PinModes
{
	MCP23S08_PINMODE_OUTPUT,		/**< MCP23S08 pin is output */
	MCP23S08_PINMODE_INPUT,			/**< MCP23S08 pin is input */
	MCP23S08_PINMODE_INPUT_PULLUP,	/**< MCP23S08 pin is input with enabled pullup */
}MCP23S08_PinModes_t;

class MCP23S08
{
	public:
		// constructors
		MCP23S08();
		MCP23S08(uint8_t deviceAddr);
	
		// call in setup
		void begin();
	
		// usage equivalent to the default IDE functions, see examples
		bool digitalReadIO(uint8_t pin);
		void digitalWriteIO(uint8_t pin, bool state);
		void pinModeIO(uint8_t pin, MCP23S08_PinModes_t mode);
	
		// advanced, write settings for all pins at once
		void setOutputStates(uint8_t states);
		void setPinModes(uint8_t modes);
		void enablePullups(uint8_t enables);
		void setInterruptOnChange(uint8_t mask);
		void setInterruptOnChangeDefaultCompare(uint8_t mask);
		void setInterruptControl(uint8_t mask);
		void setINTPinPushPullActiveState(bool activeLow);
	
		// advanced, get settings for all pin at once
		uint8_t getInputStates();
		uint8_t getOutputStates();
		uint8_t getPinModes();
		uint8_t getEnabledPullups();
		/**
		 * The GPINTEN register controls the interrupt-onchange feature for each pin.
		 * If a bit is set, the corresponding pin is enabled for interrupt-on-change. The DEFVAL and INTCON registers must also be configured if any pins are enabled for interrupt-on-change.
		 */
		uint8_t getInterruptOnChange();
		/**
		 * The default comparison value is configured in the DEFVAL register.
		 * If enabled (via GPINTEN and INTCON) to compare against the DEFVAL register, an opposite value on the associated pin will cause an interrupt to occur.
		 */
		uint8_t getInterruptOnChangeDefaultCompare();
		/**
		 * The INTCON register controls how the associated pin value is compared for the interrupt-on-change feature.
		 * If a bit is set, the corresponding I/O pin is compared against the associated bit in the DEFVAL register.
		 * If a bit value is clear, the corresponding I/O pin is compared against the previous value.
		 */
		uint8_t getInterruptControl();
		/**
		 * The INTF register reflects the interrupt condition on the port pins of any pin that is enabled for interrupts via the GPINTEN register.
		 * A �set� bit indicates that the associated pin caused the interrupt
		 */
		uint8_t getInterruptFlags();
		/**
		 * The INTCAP register captures the GPIO port value at the time the interrupt occurred.
		 * The register is �readonly� and is updated only when an interrupt occurs.
		 * The register will remain unchanged until the interrupt is cleared via a read of INTCAP or GPIO.
		 */	
		uint8_t getInterruptCaptures();
		
	private:
		// private variables
		uint8_t deviceOpcode = 0x40;
	
		// low level SPI communication
		void writeRegister(uint8_t address, uint8_t data);
		uint8_t readRegister(uint8_t address);
};

#endif /* MCP23S08_H_ */