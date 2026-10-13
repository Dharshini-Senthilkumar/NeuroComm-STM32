#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "stm32f10x.h"

void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_Print(char *str);

#endif
