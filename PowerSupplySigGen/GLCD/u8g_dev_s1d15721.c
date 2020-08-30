/*

  u8g_dev_s1d15721.c

  Universal 8bit Graphics Library
  
  Create by Markus Scheich
  28.08.2020
  based on u8g_dev_st7565_dogm132.c
  
*/

#include "u8g.h"

#define WIDTH 240
#define HEIGHT 64
#define PAGE_HEIGHT 4

// ### Commands
#define LCD_CMD_DATA_WRITE          0x1D    // Display Data Write (n-bytes)
#define LCD_CMD_PAGE_ADDR           0xB1    // Page Address Set (2 byte)
#define LCD_CMD_COL_ADDR            0x13    // Column Address Set (2 byte)
#define LCD_CMD_DISPLAY_OFFON       0xAE    // Display ON/OFF: 0=Off/1=On
#define LCD_CMD_DISPLAY_REVERSE     0xA6    // Display/Normal: 0=Normal/1=Reverse
#define LCD_CMD_DISPLAY_ALL_LIGHT   0xA4    // Display All Lighting: 0=Normal/1=All On
#define LCD_CMD_DISPLAY_START_LINE  0x8A    // Display Start Line (2/3 byte)
#define LCD_CMD_DISPLAY_MODE        0x66    // Display Mode (2 byte): 0=4 gray-scale, 1=binary
#define LCD_CMD_GRAY_SCALE_PATTERN  0x39    // Gray Scale Pattern (2 byte)
#define LCD_CMD_COM_OUT_STATUS      0xC4    // Common Output Status Select: 0=Normal/1=Reverse
#define LCD_CMD_DUTY_SET            0x6D    // Duty Set Command (3 byte)
#define LCD_CMD_BUILTIN_OSC         0xAA    // Built-in Oscillator Circuit: [0]:0=Off/1=On
#define LCD_CMD_BUILTIN_OSC_FREQ    0x5F    // Built-in Oscillator Circuit Frequency Select (2 byte)
#define LCD_CMD_PWR_CONTROL         0x25    // Power Control Set (2 byte)
#define LCD_CMD_LC_DRIVE_VOLTAGE    0x2B    // Liquid Crystal Drive Voltage Select (2 byte)
#define LCD_CMD_EL_VOLUME           0x81    // Electronic Volume Set (2 byte)
#define LCD_CMD_RESET               0xE2    // Software Reset

static const uint8_t u8g_dev_s1d15721_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),			/* disable chip */
  U8G_ESC_ADR(0),			/* instruction mode */
  U8G_ESC_CS(1),			/* enable chip */	

  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_RESET | 1,
  
  U8G_ESC_DLY(10),			/* delay 10 ms */
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_BUILTIN_OSC_FREQ,
  U8G_ESC_ADR(1),           /* data mode */
  0x000,

  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_BUILTIN_OSC | 1,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_LC_DRIVE_VOLTAGE,
  U8G_ESC_ADR(1),           /* data mode */
  0x007,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_EL_VOLUME,
  U8G_ESC_ADR(1),           /* data mode */
  0x010,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_PWR_CONTROL,
  U8G_ESC_ADR(1),           /* data mode */
  0b11111,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_DUTY_SET,
  U8G_ESC_ADR(1),           /* data mode */
  0x011,
  0x000,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_DISPLAY_REVERSE | 0,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_DISPLAY_ALL_LIGHT | 0,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_DISPLAY_MODE,
  U8G_ESC_ADR(1),           /* data mode */
  0b00,

  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_GRAY_SCALE_PATTERN,
  U8G_ESC_ADR(1),           /* data mode */
  0x036,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_COM_OUT_STATUS | 1,
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_DISPLAY_START_LINE,
  U8G_ESC_ADR(1),           /* data mode */
  0x000,
 
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_DISPLAY_OFFON | 1,
  
  U8G_ESC_DLY(100),			/* delay 100 ms */
  U8G_ESC_CS(0),			/* disable chip */
  U8G_ESC_END				/* end of sequence */
};

static const uint8_t u8g_dev_s1d15721_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_COL_ADDR,
  U8G_ESC_ADR(1),           /* data mode */
  0x008,					/* 0 */
  
  U8G_ESC_ADR(0),           /* instruction mode */
  LCD_CMD_PAGE_ADDR,
  U8G_ESC_ADR(1),           /* data mode */
  
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_s1d15721_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_400NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_s1d15721_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
         u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
         u8g_WriteEscSeqP(u8g, dev, u8g_dev_s1d15721_data_start); 
		 u8g_WriteByte(u8g, dev, pb->p.page + 2);		/* page number */
		 
         u8g_SetAddress(u8g, dev, 0);				/* instruction mode */
         u8g_WriteByte(u8g, dev, LCD_CMD_DATA_WRITE);
         u8g_SetAddress(u8g, dev, 1);				/* data mode */
         if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
		 {
			return 0;
		 }
         u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
  }
  return u8g_dev_pb8v2_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_s1d15721_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_s1d15721_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_s1d15721_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_s1d15721_fn, U8G_COM_HW_SPI);
