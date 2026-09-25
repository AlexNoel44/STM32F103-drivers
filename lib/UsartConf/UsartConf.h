#ifndef USART_CONF_H
#define USART_CONF_H

#include <stdint.h>
#include <stdbool.h>

#define MCU_CLOCK 8000000
#define BAUDRATE  9600

typedef struct
{
    uint64_t Baudrate;
    uint64_t McuClock;
    bool     UseIt;
}UsartConf;

typedef enum
{
    USART_Enable = 0x2000,
    USART_Disable = 0x0000,
}USART_UE;

typedef enum
{
    USART_MODE_Polling,
    USART_MODE_Interrupt,
    USART_MODE_DMA,
}USART_Mode;

typedef struct
{
    uint32_t USART_BaudRate;                                                    
    uint16_t USART_WordLength;                                               
    uint16_t USART_StopBits;                                                  
    uint16_t USART_Parity;                                              
    uint16_t USART_Mode;              
    uint16_t USART_HardwareFlowControl;
    USART_Mode USART_CurrentMode;
} USART_InitTypeDef;

typedef struct
{
    uint16_t USART_Clock;                          
    uint16_t USART_CPOL;    
    uint16_t USART_CPHA;    
    uint16_t USART_LastBit;                          
} USART_ClockInitTypeDef;




extern const UsartConf g_usartConf;

#endif // USART_CONF_H