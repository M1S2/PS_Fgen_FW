/**
 * @file	ScreenManager.h
 * @date	05.11.2020 18:45:19
 * @author	Markus Scheich
 * @brief	Containing a class and functions for screen handling.
 */ 

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include "../libraries/u8glib/u8g.h"
#include "../Encoder/Encoder.h"
#include "../KeyPad/KeyPad.h"
#include "../Configuration.h"
#include "../UI_Lib/Core/UI_Manager.h"
#include "../UI_Lib/Core/UI_Elements.h"
#include "Icons.h"
#include <string.h>


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


/**
 * Class that is used to control the screen.
 * This class can be used for screen handling. It contains all necessary handles and methods.
 */
class ScreenManagerClass
{
	private:
		u8g_t _u8g;						/**< u8g_lib graphics handle that is used with all drawing related methods. */
		
		/**
		 * Build the VisualTree for all screens used for drawing.
		 * This function is internally called by the Init() function of this class.
		 */
		void uiBuildTree();
		
	public:
		UI_Manager UiManager;					/**< Handle for the UI_Manager of the UI_Lib. The UI_Manager is used as entry point into the UI_Lib. */
		
		bool DisplayEnabled;					/**< Variable used to keep track if the display is enabled or not. */
		bool DisplayInverted;					/**< Variable used to keep track if the display is inverted or not. */
	
		bool RedrawScreenRequest;				/**< Flag that indicates that the screen should be redrawn. A redraw should occur periodically (if the screen needs to be redrawn periodically) or on user inputs. */
		bool CurrentScreenNeedsPeriodicRedraw;	/**< Flag that indicates if the current displayed screen needs to be periodically redrawn. This is neccessary if the screen contains measured values that change periodically. This isn't neccessary if the screen only contains static data. The screen is always redrawn on user inputs (keys or USART). */
	
		bool IsSplashScreenShown;				/**< Variable used to keep track if the SplashScreen is shown. */
		uint16_t TimeCounter_SplashScreen_ms;	/**< Timer conter value that is used to measure the time, the SplashScreen is shown. */
		uint16_t TimeCounter_ScreenRedraw_ms;	/**< Variable used for measuring the time to the next screen redraw */
		
		void Init();							/**< Initialize the ScreenManager. This method initializes the u8g_lib and UI_Manager handles and builds the VisualTree. */
		
		void DoDraw();							/**< Evaluate, if the screen should be redrawn. If yes, the DrawAll method is called. */
		void DrawAll();							/**< Call this method to redraw the screen. This contains the picture loop of the u8g_lib. */
		
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
		 * @param currentPeriod_ms Tick period of the device timer at which this method is called.
		 */
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
		
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
		
		/**
		 * Enable or Disable the display depending on the input parameter.
		 * @param displayEnabled If true, the display is enabled; If false, the display is disabled.
		 */
		void SetDisplayEnabled(bool displayEnabled);

		/**
		 * Invert the display depending on the input parameter.
		 * @param displayInverted If true, the display is inverted; If false, the display is non-inverted (normal).
		 */
		void SetDisplayInverted(bool displayInverted);
};

#endif /* SCREENMANAGER_H_ */