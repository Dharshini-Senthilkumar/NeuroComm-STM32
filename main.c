#include "stm32f10x.h"
#include "i2c_lcd.h"   // custom I2C LCD driver

void ADC1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // Enable ADC1 clock
    GPIOA->CRL &= ~(0xF << (0*4));        // PA0 as analog input

    ADC1->SMPR2 |= ADC_SMPR2_SMP0;        // Sample time for channel 0
    ADC1->SQR3 = 0;                       // 1st conversion on channel 0
    ADC1->CR2 |= ADC_CR2_ADON;            // Enable ADC
    for (volatile int i = 0; i < 10000; i++); // Stabilise
    ADC1->CR2 |= ADC_CR2_CAL;             // Calibrate
    while (ADC1->CR2 & ADC_CR2_CAL);
}

uint16_t ADC_Read(void) {
    ADC1->CR2 |= ADC_CR2_ADON;            // Start conversion
    while (!(ADC1->SR & ADC_SR_EOC));     // Wait until complete
    return ADC1->DR;                      // Read result
}

int main(void) {
    SystemInit();
    ADC1_Init();
    LCD_Init();

    LCD_Print("NeuroComm Ready");

    while (1) {
        uint16_t value = ADC_Read();

        LCD_SetCursor(0, 1);
        if (value < 1000) {
            LCD_Print("Message: YES ");
        } else if (value < 2500) {
            LCD_Print("Message: NO  ");
        } else {
            LCD_Print("Message: HELP");
        }
    }
}
