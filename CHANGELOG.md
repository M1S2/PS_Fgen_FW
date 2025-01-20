# PS_Fgen_FW

## [v2.0.0]

- Adapted the documentation files and images
- Reduced allowed minimum OVP level to 50% (#38)
- Removed DDS RECT_WAVE_TABLE and DC_WAVE_TABLE, restructured DDS init and start slightly
- Removed some SCPI commands (to save RAM and Flash size)
- Added define to configuration to change the Baud rate staticly (#39)
- Removed feature to change the serial baud rate and to disable the serial echo (to reduce RAM and Flash sizes)
- Removed protection cleared message from ScreenPS (fixing bug with OnOffControls LED indicating wrong state sometimes)
- Added feature to reset the calibration factors
- Removed USART.h and using Arduino Serial instead
- Removed ATX +12V and -12V device voltages
- Changed pin handling to use Arduino functions
- Changed SPI handling to Arduino SPI functions, improved SPI chip select handling
- Added touch screen support
- Updated doxygen documentation to work with doxygen v1.12.0
- Decreasing DDS frequency (otherwise the device hangs if DDS is enabled)
- Added UI_Lib library and adapted the screens accordingly
- Updated project to build with platformio now
- Added handling for OnOffControls via IO Expander (#37)
- Fixed encoder problems with enabled DDS channels (INT0 ISR was called twice sometimes) (#36)
- Removed constant current feature (#28)

## [v1.0.0]

- Initial version
