/**
 * @file	ScreenManager.h
 * @date	05.11.2020 18:45:19
 * @author	Markus Scheich
 * @brief	Containing a class and functions for screen handling.
 */ 

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "ILI9341_Fast.h"

#include "../Encoder/Encoder.h"
#include "../KeyPad/KeyPad.h"
#include "../Configuration.h"
#include "Core/UI_Manager.h"
#include "Core/UI_Elements.h"
#include "Icons.h"
#include <string.h>

#define DISPLAY_WIDTH	320
#define DISPLAY_HEIGHT	240

#define COLOR_BACKGROUND			RGB565(0x00, 0x00, 0x00)
#define COLOR_FOREGROUND			RGB565(0x00, 0xF7, 0x00)
#define COLOR_FOREGROUND_HEADERS	RGB565(0xFF, 0xFF, 0xFF)

/**
 * Available screen types.
 * Enumeration with all different screens of the device, dependent on the enabled subsystems.
 * The enumeration value is used to determine the index of the screen in the tab control.
 */
typedef enum ScreenTypes
{
#ifdef PS_SUBSYSTEM_ENABLED
	SCREEN_PS,					/**< Power supply screen. */
#endif
#ifdef DDS_SUBSYSTEM_ENABLED
	SCREEN_DDS,					/**< DDS screen. */
#endif
#ifdef MEASURE_SUBSYSTEM_ENABLED
	SCREEN_MEAS,				/**< Measurement screen. */
#endif
	SCREEN_CONF,				/**< Configuration screen. */
	NUM_SCREEN_ELEMENTS			/**< The last element is used to determine the number of elements in the enumeration */
}ScreenTypes_t; 


#ifdef SPLASHSCREEN_ENABLED 
	/**
	 * Build the SplashScreen by linking all necessary components together.
	 * @return UIElement representing the top level element of the SplashScreen.
	 */
	UIElement* uiBuildSplashScreen();
#endif

#ifdef PS_SUBSYSTEM_ENABLED
	/**
	 * Build the PowerSupply screen by linking all necessary components together.
	 * @return UIElement representing the top level element of the PowerSupply screen.
	 */
	UIElement* uiBuildScreenPS();
#endif

#ifdef DDS_SUBSYSTEM_ENABLED
	/**
	 * Build the DDS screen by linking all necessary components together.
	 * @return UIElement representing the top level element of the DDS screen.
	 */
	UIElement* uiBuildScreenDDS();
#endif

#ifdef MEASURE_SUBSYSTEM_ENABLED
	/**
	 * Build the Measure screen by linking all necessary components together.
	 * @return UIElement representing the top level element of the Measure screen.
	 */
	UIElement* uiBuildScreenMeasure();
#endif

/**
 * Build the Settings screen by linking all necessary components together.
 * @return UIElement representing the top level element of the Settings screen.
 */
UIElement* uiBuildScreenSettings();

/**
 * Build the Calibration screen by linking all necessary components together.
 * @return UIElement representing the top level element of the Calibration screen.
 */
UIElement* uiBuildScreenCalibration();

/**
 * Start the calibration by initializing all calibration screen components and the calibration state machine.
 * @return UIElement representing the top level element of the Calibration screen.
 */
UIElement* StartCalibration();

#ifdef MEASURE_SUBSYSTEM_ENABLED
	/** 
	 * Update the visibility of the DMM channel negative warning icons.
	 */
	void updateDmmNegativeWarningVisibility();
#endif

#ifdef DDS_SUBSYSTEM_ENABLED
	/**
	 * Update the visibility of all controls on the DDS screen. Do this e.g. after loading all settings. 
	 */
	void ScreenDDSUpdateVisibility();
#endif

#define LCD_CS_PIN_NUMBER	1		// Corresponding digital pin number for PORTB1
#define LCD_A0_PIN_NUMBER	0		// Corresponding digital pin number for PORTB0

/**
 * Class that is used to control the screen.
 * This class can be used for screen handling. It contains all necessary handles and methods.
 */
class ScreenManagerClass
{
	private:
		ILI9341 _tft;							/**< ILI9341 graphics handle that is used with all drawing related methods. */
		
		/**
		 * Build the VisualTree for all screens used for drawing.
		 * This function is internally called by the Init() function of this class.
		 */
		void uiBuildTree();
		
	public:
		bool RedrawScreenRequest;				/**< Flag that indicates that the screen should be redrawn. A redraw should occur periodically (if the screen needs to be redrawn periodically) or on user inputs. */
		bool CurrentScreenNeedsPeriodicRedraw;	/**< Flag that indicates if the current displayed screen needs to be periodically redrawn. This is neccessary if the screen contains measured values that change periodically. This isn't neccessary if the screen only contains static data. The screen is always redrawn on user inputs (keys or USART). */
	
		bool IsSplashScreenShown;				/**< Variable used to keep track if the SplashScreen is shown. */
		uint16_t TimeCounter_SplashScreen_ms;	/**< Timer conter value that is used to measure the time, the SplashScreen is shown. */
		uint16_t TimeCounter_ScreenRedraw_ms;	/**< Variable used for measuring the time to the next screen redraw */
		
		ScreenManagerClass();					/**< Constructor of the ScreenManagerClass */
		void Init();							/**< Initialize the ScreenManager. This method initializes the u8g_lib and UI_Manager handles and builds the VisualTree. */
		void UpdateVisibilities();				/**< Update the visibility of all screens. */
		
		void DoDraw();							/**< Evaluate, if the screen should be redrawn. If yes, the Draw method is called. */
		
		void ShowUiMainPage();					/**< This method can be used to show the main page (no MessageDialog or calibration page). This is a short version for using the ChangeVisualTreeRoot() method of the UI_Manager. */
		void ShowUiCalibrationMenu();			/**< This method can be used to start the calibration and show the calibration page. This is a short version for using the ChangeVisualTreeRoot() method of the UI_Manager. */
		
		/**
		 * Show or hide a message indicating that the device is in RWL state (only controllable via SCPI commands).
		 * @param showMessage If true, a message is displayed that the device is in RWL state; If false, the main page is shown.
		 */
		void ShowHideDeviceRWLMessage(bool showMessage);
		
		/**
		 * This method is called periodically by the device timer ISR and runs in the ISR context.
		 * It is used to show or hide the SplashScreen.
		 */
		void DeviceTimerTickISR();
		
		/**
		 * Update the settings changed indicator (star indicating that there are unstored settings).
		 * @param settingsChanged If true, the star is shown; If false, the star is hidden.
		 */
		void UpdateSettingsChangedIndicator(bool settingsChanged);
		
		/**
		 * Method that is used to input keys (and encoder inputs) into the screen subsystem (and the UI_Lib).
		 * @param key Key that is forwarded to the UI_Lib.
		 */
		void KeyInput(Keys_t key);
};

#endif /* SCREENMANAGER_H_ */