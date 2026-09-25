#include "Gpio.h"

//********************************************************************************************************************************************************************************************************
// Private Function Declaration
//********************************************************************************************************************************************************************************************************
static void _enableGpioClock(GPIO_TypeDef* GPIOx);

//********************************************************************************************************************************************************************************************************
// Public Function Definition
//********************************************************************************************************************************************************************************************************
void Gpio_Init(const GPIO_Conf* conf)
{
    uint32_t tmpreg = 0x00, currentMode = 0x00, pinPos=0x00, pin = 0x00, pos=0x00;
    GPIO_TypeDef* GPIOx = NULL;

    for(uint8_t i = 0; i < USED_PINS; i++)
    {
        GPIOx = conf[i].GPIO_Port;
        _enableGpioClock(GPIOx);

        currentMode = ((uint32_t)conf[i].GPIO_Mode) & 0x0F;
        if(((uint32_t)conf[i].GPIO_Mode) & 0x10)
        {
            currentMode |= ((uint32_t)conf[i].GPIO_Speed);
        }

        pin = ((uint32_t)conf[i].GPIO_Pin);
        pinPos=0;
        while(pin > 1)
        {
            pin >>= 1;
            pinPos++;
        }

        if(pinPos < 8)
        {
            tmpreg = GPIOx->CRL;
            pos = pinPos << 2;
            tmpreg &= ~(0xF << pos);
            tmpreg |=  (currentMode << pos);
            GPIOx->CRL = tmpreg;
        }
        else
        {
            tmpreg = GPIOx->CRH;
            pos = (pinPos - 8) << 2;
            tmpreg &= ~(0xF << pos);
            tmpreg |=  (currentMode << pos);
            GPIOx->CRH=tmpreg;
        }

        if(conf[i].GPIO_Mode == GPIO_Mode_IPU)
            GPIOx->BSRR = conf[i].GPIO_Pin;
        else if(conf[i].GPIO_Mode == GPIO_Mode_IPD)
            GPIOx->BRR  = conf[i].GPIO_Pin;
    }
}
//********************************************************************************************************************************************************************************************************

void Gpio_High(GPIO_TypeDef* GPIOx, uint16_t gpio)
{
    GPIOx->BSRR = gpio;
}
//********************************************************************************************************************************************************************************************************

void Gpio_Low(GPIO_TypeDef* GPIOx, uint16_t gpio)
{
    GPIOx->BRR = gpio;
}

//********************************************************************************************************************************************************************************************************
// Public Function Definition
//********************************************************************************************************************************************************************************************************
static void _enableGpioClock(GPIO_TypeDef* GPIOx)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    if(GPIOx == GPIOA)       RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    else if(GPIOx == GPIOB)  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    else if(GPIOx == GPIOC)  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    else if(GPIOx == GPIOD)  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
}

//1. Activate the clock acces to the GPIOx.
//2. Activate the clock access to the alternate functions.
//3. Configure the pins.