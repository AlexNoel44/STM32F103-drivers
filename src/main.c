#include "stm32f1xx.h"
#include "Usart.h"
#include "Spi.h"
#include "Gpio.h"

volatile uint32_t ms = 0;
uint32_t last = 0;

/* 1ms tick */
void SysTick_Handler(void)
{
    ms++;
}

uint32_t millis(void)
{
    return ms;
}

int main(void)
{

    SysTick_Config(SystemCoreClock / 1000); // 1ms tick
 
    uint8_t payload[] = {0x45, 0x44, 0x44, 0x44, 0x44};
    uint8_t ret[5];

    Gpio_Init(g_GpioConf);
    Spi_Init(&g_SpiConf, SPI1);
    Usart_Init(&g_usart1, &g_usartConf, USART1);

    while(1)
    {
        if(millis() - last >= 10) // every 100 ms
        {
            last = millis();
            Spi_Transfer(payload, ret, 5);
            Usart_WriteBytes(&g_usart1, payload, 5);
        }
    }

}