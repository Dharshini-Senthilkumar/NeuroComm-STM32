#include "i2c_lcd.h"

#define LCD_ADDR 0x4E   // PCF8574 I2C address (try 0x4E or 0x27<<1)

// Control bits
#define RS 0x01
#define EN 0x04

static void I2C_Write(uint8_t data) {
    while (I2C1->SR2 & I2C_SR2_BUSY);           // Wait if busy

    I2C1->CR1 |= I2C_CR1_START;                 // START
    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = LCD_ADDR;                        // Send address
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;                            // Clear ADDR flag

    I2C1->DR = data;                            // Send data
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->CR1 |= I2C_CR1_STOP;                  // STOP
}

static void LCD_Send(uint8_t data, uint8_t mode) {
    uint8_t high = (data & 0xF0) | mode;
    uint8_t low  = ((data << 4) & 0xF0) | mode;

    I2C_Write(high | EN);
    I2C_Write(high);

    I2C_Write(low | EN);
    I2C_Write(low);
}

void LCD_Command(uint8_t cmd) {
    LCD_Send(cmd, 0x00);                        // RS=0 for command
}

void LCD_Init(void) {
    // Enable GPIOB + I2C1
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB6 = SCL, PB7 = SDA
    GPIOB->CRL &= ~((0xF << (6*4)) | (0xF << (7*4)));
    GPIOB->CRL |= (0xB << (6*4)) | (0xB << (7*4));

    // Configure I2C1 @ 100kHz
    I2C1->CR2 = 36;                             // APB1 = 36 MHz
    I2C1->CCR = 180;                            // 100 kHz
    I2C1->TRISE = 37;
    I2C1->CR1 |= I2C_CR1_PE;

    // LCD init sequence
    LCD_Command(0x33);
    LCD_Command(0x32);
    LCD_Command(0x28);                          // 4-bit, 2 line
    LCD_Command(0x0C);                          // Display on
    LCD_Command(0x06);                          // Entry mode
    LCD_Command(0x01);                          // Clear display
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    uint8_t addr[] = {0x80, 0xC0};
    LCD_Command(addr[row] + col);
}

void LCD_Print(char *str) {
    while (*str) {
        LCD_Send(*str++, RS);                   // RS=1 for data
    }
}
