#ifndef PORT_CONF_H
#define PORT_CONF_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx.h"

#define USED_PINS 6

#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */

#define SPI1_SS         GPIO_Pin_0
#define SPI1_SS_INDEX   0
#define SPI1_SCK        GPIO_Pin_5
#define SPI1_SCK_INDEX  1
#define SPI1_MISO       GPIO_Pin_6
#define SPI1_MISO_INDEX 2
#define SPI1_MOSI       GPIO_Pin_7
#define SPI1_MOSI_INDEX 3
#define USART1_TX       GPIO_Pin_9
#define USART_TX_INDEX  4
#define USART1_RX       GPIO_Pin_10
#define USART_RX_INDEX  5

typedef enum
{ 
  GPIO_Speed_0MHz = 0,
  GPIO_Speed_10MHz,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

typedef struct
{
  GPIO_TypeDef      *GPIO_Port;
  uint16_t          GPIO_Pin;            
  GPIOSpeed_TypeDef GPIO_Speed;
  GPIOMode_TypeDef  GPIO_Mode;
}GPIO_Conf;


extern const GPIO_Conf g_GpioConf[USED_PINS];


#endif // PORT_CONF_H