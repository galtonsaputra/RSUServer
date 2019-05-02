#pragma once

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "SSD1306Fonts.h"

#define OLED_I2C_SLAVE_ADDRESS_DEFAULT	0x3C

enum TOledPanel {
	OLED_PANEL_128x64_8ROWS,	///< Default
	OLED_PANEL_128x64_4ROWS,
	OLED_PANEL_128x32_4ROWS
};

enum TCursorMode {
	SET_CURSOR_OFF = 0,
	SET_CURSOR_ON = (1 << 0),
	SET_CURSOR_BLINK_OFF = 0,
	SET_CURSOR_BLINK_ON = (1 << 1)
};

enum OledFontSize {
	FONT_SIZE_SMALL = 0,
	FONT_SIZE_MEDIUM,
	FONT_SIZE_LARGE,
	FONT_SIZE_XLARGE
};


class OledDisplay
{
protected:
	uint8_t m_nCols;
	uint8_t m_nRows;
};

class SSD1306 : OledDisplay
{
public:
	SSD1306();
	~SSD1306();

	bool Start(void);
	void PutString(const char *pString);
	
	void SetFontSize(OledFontSize);

	void Cls(void);
	void InitMembers(void);
	void SendCommand(uint8_t cmd);
	void SetCursorOn(void);
	void SetCursorOff(void);
	void SetCursorBlinkOn(void);
	void PutChar(int c);
	void SetCursor(TCursorMode tCursorMode);
	void SetCursorPos(uint8_t col, uint8_t row);
	void SendData(const uint8_t *pData, uint32_t nLength);

	uint8_t m_nSlaveAddress;
	TOledPanel m_OledPanel;
	char *m_pShadowRam;

	uint32_t m_i2cd;
	uint16_t m_nShadowRamIndex;
	uint8_t m_nCursorOnChar;
	uint8_t m_nCursorOnCol;
	uint8_t m_nCursorOnRow;
	uint8_t m_tCursorMode;
};
