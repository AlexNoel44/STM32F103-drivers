#include "UsartConf.h"

const UsartConf g_usartConf =
{
    .USART_InitStruct =
    {
        .USART_BaudRate = BAUDRATE,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_Mode = USART_Mode_Rx_Tx,
        .USART_HardwareFlowControl = 0,
    },
    .USART_ClockInitStruct =
    {
        .USART_Clock = 0,
        .USART_CPOL = 0,
        .USART_CPHA = 0,
        .USART_LastBit = 0,
    },
    .UseIt = true,
    .McuClock = MCU_CLOCK,
};
