#include "GpioConf.h"

const GPIO_Conf g_GpioConf[USED_PINS] = 
{
    /*SPI1*/
        /*SPI1_SCK*/
        {
            .GPIO_Port = GPIOA,
            .GPIO_Pin = GPIO_Pin_5,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = GPIO_Mode_AF_PP
        },
        /*SPI1_MOSI*/
        {
            .GPIO_Port = GPIOA,
            .GPIO_Pin = GPIO_Pin_7,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = GPIO_Mode_AF_PP
        },
        /*SPI1_MISO*/
        {
            .GPIO_Port = GPIOA,
            .GPIO_Pin = GPIO_Pin_6,
            .GPIO_Speed = GPIO_Speed_0MHz,
            .GPIO_Mode = GPIO_Mode_IN_FLOATING
        },
        /*SPI1_SS*/
        {
            .GPIO_Port = GPIOA,
            .GPIO_Pin = GPIO_Pin_0,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = GPIO_Mode_Out_PP
        },
    /*UART1*/
        /*UART1_TX*/
        {
            .GPIO_Port = GPIOA,
            .GPIO_Pin = GPIO_Pin_9,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = GPIO_Mode_AF_PP,
        },
        /*UART1_RX*/
        {
            .GPIO_Port = GPIOA,
            .GPIO_Pin = GPIO_Pin_10,
            .GPIO_Speed = GPIO_Speed_0MHz,
            .GPIO_Mode = GPIO_Mode_IN_FLOATING,
        }
};
