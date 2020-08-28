/*
 * VLGEM1277.cpp
 *
 * Created: 25.08.2020 20:05:24
 *  Author: V17
 * see: https://www.mikrocontroller.net/attachment/409640/main_lcd_vlgem1277-01.cpp
 */ 


// #######################################################################
// ###                                                                 ###
// ###          Test VL-FS-COG-VLGEM1277-01 (240x264 4-gray LCD)       ###
// ###                                                                 ###
// #######################################################################

#include "VLGEM1277.h"
#include "../Pins/Pins.h"
#include "../Spi/spi.h"

// === LCD VLGEM1277-01 (S1D15721, 240x64 2bit Gray Color Display) =======

// --- private -----------------------------------------------------------

/** 
 * Sends Data (1 Byte).
 *
 * @param data (I) The data byte.
 */
static void _lcd_data(uint8_t data);

/** 
 * Sends Commands (1 Byte).
 *
 * @param cmd (I) The command byte.
 */
static void _lcd_cmd1(uint8_t cmd);

/** 
 * Sends Commands (2 Byte).
 *
 * @param cmd (I) The command byte.
 * @param arg (I) The arguments byte.
 */
static void _lcd_cmd2(uint8_t cmd, uint8_t arg);

/** 
 * Sends Commands (3 Byte).
 *
 * @param cmd (I) The command byte.
 * @param arg1 (I) The argument byte #1.
 * @param arg2 (I) The argument byte #1.
 */
static void _lcd_cmd3(uint8_t cmd, uint8_t arg1, uint8_t arg2);

/** 
 * Sends Byte to Controller (Bit banging for testing purposes only).
 *
 * @param b (I) The byte to send to controller.
 */
static void _lcd_send(uint8_t b);

// === LCD VLGEM1277-01 (S1D15721, 240x64 4-gray display) ================

// Inits Module.
void lcd_init() 
{
    // Software Reset
    _lcd_cmd1(LCD_CMD_RESET);
    _delay_ms(10);
	
    // Init Sequence
    _lcd_cmd2(LCD_CMD_BUILTIN_OSC_FREQ,0x00);
	_lcd_cmd1(LCD_CMD_BUILTIN_OSC|1);
    _lcd_cmd2(LCD_CMD_LC_DRIVE_VOLTAGE,0x07);
    _lcd_cmd2(LCD_CMD_EL_VOLUME,0x10);
    _lcd_cmd2(LCD_CMD_PWR_CONTROL,0b11111);
    _lcd_cmd3(LCD_CMD_DUTY_SET,0x11,0x00);

    _lcd_cmd1(LCD_CMD_DISPLAY_REVERSE|0);
    _lcd_cmd1(LCD_CMD_DISPLAY_ALL_LIGHT|0);
    _lcd_cmd2(LCD_CMD_DISPLAY_MODE,0b00);
    _lcd_cmd2(LCD_CMD_GRAY_SCALE_PATTERN,0x36);
    _lcd_cmd1(LCD_CMD_COM_OUT_STATUS|1);
    _lcd_cmd2(LCD_CMD_DISPLAY_START_LINE,0);
    _lcd_cmd1(LCD_CMD_DISPLAY_OFFON|1);
}

// Clears Buffer.
void lcd_clear() 
{
    for (uint8_t y=0;y<(LCD_HEIGHT/4);y++) 
	{
        _lcd_cmd2(LCD_CMD_PAGE_ADDR,2+y);
        _lcd_cmd2(LCD_CMD_COL_ADDR,8);
        _lcd_cmd1(LCD_CMD_DATA_WRITE);
        for (uint8_t x=0;x<LCD_WIDTH;x++) 
		{
            _lcd_data(0x00);
        }
    }
}

// Draws ASCII Character.
void lcd_char(uint8_t gray, uint8_t col, uint8_t row, char chr)
 {
    if ((col>=LCD_COLS)||(row>=LCD_ROWS)) 
	{
        return;        
    }
    if ((chr<0x20)||(chr>=0x60)) 
	{
        chr=0x20;        
    }
    chr-=0x20;
    
    // upper half
    _lcd_cmd2(LCD_CMD_PAGE_ADDR,2+(row<<1));
    _lcd_cmd2(LCD_CMD_COL_ADDR,8+(6*col));
    _lcd_cmd1(LCD_CMD_DATA_WRITE);
    for (uint8_t x=0;x<6;x++) 
	{
        uint8_t v=pgm_read_byte(&FONT5X7[chr][x]);
        uint8_t b=0;
        for (uint8_t y=0;y<(LCD_FONT_HEIGHT/2);y++) 
		{
            b>>=2;
            if (v&0x01) 
			{
                b|=gray<<6;
            }
            v>>=1;
        }
        _lcd_data(b);
    }

    // lower half
    _lcd_cmd2(LCD_CMD_PAGE_ADDR,3+(row<<1));
    _lcd_cmd2(LCD_CMD_COL_ADDR,8+(6*col));
    _lcd_cmd1(LCD_CMD_DATA_WRITE);
    for (uint8_t x=0;x<6;x++) 
	{
        uint8_t v=pgm_read_byte(&FONT5X7[chr][x]);
        uint8_t b=0;
        for (uint8_t y=0;y<(LCD_FONT_HEIGHT/2);y++) 
		{
            b>>=2;
            if (v&0x10) 
			{
                b|=gray<<6;
            }
            v>>=1;
        }
        _lcd_data(b);
    }
}

// --- private -----------------------------------------------------------

// Sends Data (1 Byte).
static void _lcd_data(uint8_t data) 
{
	CLEAR_BIT(PORTB, LCD_CS);
	SET_BIT(PORTB, LCD_A0);
    _lcd_send(data);
	SET_BIT(PORTB, LCD_CS);
}

// Sends Commands (1 Byte).
static void _lcd_cmd1(uint8_t cmd) 
{
    CLEAR_BIT(PORTB, LCD_CS);
	CLEAR_BIT(PORTB, LCD_A0);
    _lcd_send(cmd);
	SET_BIT(PORTB, LCD_CS);
}

// Sends Commands (2 Byte).
static void _lcd_cmd2(uint8_t cmd, uint8_t arg)
{
	CLEAR_BIT(PORTB, LCD_CS);
	CLEAR_BIT(PORTB, LCD_A0);
    _lcd_send(cmd);
	SET_BIT(PORTB, LCD_A0);
    _lcd_send(arg);
	SET_BIT(PORTB, LCD_CS);
}

// Sends Commands (3 Byte).
static void _lcd_cmd3(uint8_t cmd, uint8_t arg1, uint8_t arg2) 
{
	CLEAR_BIT(PORTB, LCD_CS);
	CLEAR_BIT(PORTB, LCD_A0);
    _lcd_send(cmd);
	SET_BIT(PORTB, LCD_A0);
    _lcd_send(arg1);
    _lcd_send(arg2);
	SET_BIT(PORTB, LCD_CS);
}

// Sends Byte to Controller (Bit banging for testing purposes only).
// (TODO implement HW spi here...)
static void _lcd_send(uint8_t b) 
{
	SPI_SendByte(b);
	
    /*for (uint8_t i=0;i<8;i++) {
        if (b&0x80) {
            PORTB|=PINB_MOSI;
        } else {
            PORTB&=~PINB_MOSI;
        }
        PORTB|=PINB_SCK;
        PORTB&=~PINB_SCK;
        b<<=1;
    }*/
}
