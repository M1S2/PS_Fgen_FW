/**
 * @file	EnumIndicator.h
 * @date	15.03.2021 19:42:12
 * @author	Markus Scheich
 * @brief	Containing a class for an enum indicator that is only showing a enumeration variable value.
 */ 

#ifndef ENUMINDICATOR_H_
#define ENUMINDICATOR_H_

#include "../Core/UIElement.h"

/**
 * Class for an enum indicator that is only showing a enumeration variable value.
 * @tparam T Type of enumeration handled by this indicator
 */
template <class T>
class EnumIndicator : public UIElement
{
	protected:
		const char** _enumNames;	/**< Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values. */
		uint8_t _numEnumValues;		/**< Number of enumeration values. This number is set by the constructor of the indicator and must match the real number of enum values in T. */
		T* _valuePointer;			/**< Pointer to the enumeration variable that is shown by this indicator. */
		T _valueDraw;				/**< This variable is updated from the _valuePointer on each draw of the first page. */
	
	public:
	
		/**
		 * Constructor of the EnumIndicator.
		 * @param locX X Location of the upper left corner of the EnumIndicator 
		 * @param locY Y Location of the upper left corner of the EnumIndicator
		 * @param valuePointer Pointer to the enumeration variable that is shown by this indicator.
		 * @param enumNames Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values.
		 * @param numEnumValues Number of enumeration values. This number must match the real number of enum values in T.
		 */
		EnumIndicator(unsigned char locX, unsigned char locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues);
		
		/**
		 * Method used for drawing of the EnumIndicator.
		 * @param u8g Pointer to the u8g_lib object used for GLCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(u8g_t *u8g, bool isFirstPage) override;
};

#endif /* ENUMINDICATOR_H_ */