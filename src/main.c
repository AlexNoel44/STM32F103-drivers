#include "stm32f1xx.h"
#include "Usart.h"
#include "Gpio.h"
#include "Neo6m.h"

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
            //Usart_WriteByte(&g_usart1, data);
            Neo6m_Process(data);
        }

        Usart_WriteByte(&g_usart1, Neo6m_Minute());
        Usart_WriteByte(&g_usart1, Neo6m_GetLatitude());
        Usart_WriteByte(&g_usart1, Neo6m_GetLongitude());

    }
}
