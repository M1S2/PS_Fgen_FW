/**
 * @file	UIElementType.h
 * @date	12.03.2021 18:16:45
 * @author	Markus Scheich
 * @brief	Containing the definition for all user interface element types (controls, indicators, containers).
 */ 

#ifndef UIELEMENTTYPE_H_
#define UIELEMENTTYPE_H_

/**
 * Available user interface element types.
 * Enumeration with all different user interface element types.
 */
enum UIElementType
{
	UI_INDICATOR,		/**< Indicator elements are only used to show information to the user. */
	UI_CONTROL,			/**< Control elements are used to show informations and offer the possibility to change some values. */
	UI_CONTAINER		/**< Container elements are used to bundle other elements and add some paging or scrolling logic. */
};

#endif /* UIELEMENTTYPE_H_ */