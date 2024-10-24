/**
 * @file	UI_Elements.h
 * @date	12.03.2021 18:54:08
 * @author	Markus Scheich
 * @brief	Containing all include files of the UI_Lib.
 */ 

#ifndef UI_ELEMENTS_H_
#define UI_ELEMENTS_H_

#include "UIElement.h"
#include "../Indicators/Label.h"
#include "../Indicators/Icon.h"
#include "../Indicators/BoolIndicator.h"
#include "../Controls/BoolControl.h"
#include "../Indicators/EnumIndicator.h"
#include "../Controls/EnumControl.h"
#include "../Indicators/NumericIndicator.h"
#include "../Controls/NumericControl.h"
#include "../Indicators/ProgressBar.h"
#include "../Containers/Container.h"
#include "../Containers/ContainerPage.h"
#include "../Containers/ContainerList.h"
#include "../Controls/TabControl.h"
#include "../Controls/ButtonControl.h"
#include "../Controls/MessageDialog.h"

#include "../Indicators/EnumIndicator.cpp"
#include "../Controls/EnumControl.cpp"
#include "../Indicators/NumericIndicator.cpp"
#include "../Controls/NumericControl.cpp"
#include "../Indicators/ProgressBar.cpp"
#include "../Controls/MessageDialog.cpp"
//#include "../Indicators/Label.cpp"			// Label.cpp is included by MessageDialog.cpp
//#include "../Controls/ButtonControl.cpp"		// ButtonControl.cpp is included by MessageDialog.cpp

#endif /* UI_ELEMENTS_H_ */