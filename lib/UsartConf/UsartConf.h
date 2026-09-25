#ifndef USART_CONF_H
#define USART_CONF_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx.h"

#define MCU_CLOCK 8000000
#define BAUDRATE  9600

typedef struct
{
    uint32_t USART_BaudRate;
    uint16_t USART_WordLength;
    uint16_t USART_StopBits;
    uint16_t USART_Parity;
    uint16_t USART_Mode;
    uint16_t USART_HardwareFlowControl;
} USART_InitTypeDef;

typedef struct
{
    uint16_t USART_Clock;
    uint16_t USART_CPOL;
    uint16_t USART_CPHA;
    uint16_t USART_LastBit;
} USART_ClockInitTypeDef;

typedef enum
{
    USART_WordLength_8b = 0x0000,
    USART_WordLength_9b = 0x1000,
} USART_WordLength;

typedef enum
{
    USART_StopBits_1 = 0x0000,
    USART_StopBits_0_5 = 0x1000,
    USART_StopBits_2 = 0x2000,
    USART_StopBits_1_5 = 0x3000,
} USART_StopBits;

typedef enum
{
    USART_Parity_No = 0x0000,
    USART_Parity_Even = 0x0400,
    USART_Parity_Odd = 0x0600,
} USART_Parity;

typedef enum
{
    USART_Mode_Rx = 0x0004,
    USART_Mode_Tx = 0x0008,
    USART_Mode_Rx_Tx = 0x000C,
} USART_Mode;

typedef struct
{
    USART_InitTypeDef USART_InitStruct;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    bool UseIt;
    uint32_t McuClock;
    USART_TypeDef *Usartx;
} UsartConf;

extern const UsartConf g_usartConf;

#endif // USART_CONF_H