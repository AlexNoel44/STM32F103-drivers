#include "stm32f1xx.h"
#include "Usart.h"
#include "Gpio.h"

int main(void)
{
    Gpio_Init(g_GpioConf);
    Usart_Init(&g_usart1, &g_usartConf, USART1);
    Usart_Init(&g_usart2, &g_usartConf, USART2);

    while (1)
    {
        if (Usart_RxAvailable(&g_usart2))
        {
            uint8_t data = Usart_ReadByte(&g_usart2);
            Usart_WriteByte(&g_usart1, data);
        }
    }
}
