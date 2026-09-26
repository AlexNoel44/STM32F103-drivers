//********************************************************************************************************************************************************************************************************
// Includes
//********************************************************************************************************************************************************************************************************
#include "Usart.h"

UsartHandle g_usart1 = {0};
UsartHandle g_usart2 = {0};
UsartHandle g_usart3 = {0};

//********************************************************************************************************************************************************************************************************
// Private Function Declaration
//********************************************************************************************************************************************************************************************************
static uint16_t _computeBaudrate(uint32_t mcuClk, uint32_t br);
static void _enableUsartClock(USART_TypeDef *USARTx);
static void _enableUsartInterrupt(USART_TypeDef *USARTx);
static void _usartIrqHandler(UsartHandle *handle);
static void _lowInit(UsartHandle *handle);
static void _writeSinglePolling(UsartHandle *handle, uint8_t data);
static void _writeSingleInterrupt(UsartHandle *handle, uint8_t data);
static uint8_t _readSinglePolling(UsartHandle *handle);
static uint8_t _readSingleInterrupt(UsartHandle *handle);

//********************************************************************************************************************************************************************************************************
// Public Function Definition
//********************************************************************************************************************************************************************************************************
void Usart_Init(UsartHandle *handle, const UsartConf *conf, USART_TypeDef *usart)
{
    if((handle != NULL) && (conf != NULL))
    {
        handle->conf = conf;
        handle->usart = usart;
        handle->txBufferIn = 0;
        handle->txBufferOut = 0;
        handle->txCnt = 0;
        handle->rxBufferIn = 0;
        handle->rxBufferOut = 0;
        handle->rxCnt = 0;
        handle->rxPollingAvailable = false;

        _lowInit(handle);
    }
}
//********************************************************************************************************************************************************************************************************

void Usart_WriteByte(UsartHandle *handle, uint8_t data)
{
    if((handle != NULL) && (handle->conf != NULL))
    {
        if(handle->conf->UseIt == true)
        {
            _writeSingleInterrupt(handle, data);
        }
        else
        {
            _writeSinglePolling(handle, data);
        }
    }
}
//********************************************************************************************************************************************************************************************************

void Usart_WriteBytes(UsartHandle *handle, uint8_t *data, uint8_t size)
{
    if((handle != NULL) && (data != NULL))
    {
        for(uint8_t i = 0; i < size; i++)
        {
            Usart_WriteByte(handle, data[i]);
        }
    }
}
//********************************************************************************************************************************************************************************************************

bool Usart_RxAvailable(UsartHandle *handle)
{
    bool returnValue = false;

    if(handle != NULL)
    {
        if(handle->conf->UseIt == true)
        {
            if(handle->rxCnt > 0)
            {
                returnValue = true;
            }
        }
        else
        {
            if(handle->rxPollingAvailable == true)
            {
                returnValue = true;
                handle->rxPollingAvailable = false;
            }
        }
    }

    return returnValue;
}
//********************************************************************************************************************************************************************************************************

uint8_t Usart_ReadByte(UsartHandle *handle)
{
    uint8_t data = 0;

    if(handle != NULL)
    {
        if(handle->conf->UseIt == true)
        {
            data = _readSingleInterrupt(handle);
        }
        else
        {
            data = _readSinglePolling(handle);
        }
    }

    return data;
}

//********************************************************************************************************************************************************************************************************
// Private Function Definition
//********************************************************************************************************************************************************************************************************
static uint16_t _computeBaudrate(uint32_t mcuClk, uint32_t br)
{
    return ((mcuClk + (br / 2U)) / br);
}
//********************************************************************************************************************************************************************************************************

static void _enableUsartClock(USART_TypeDef *USARTx)
{
    if(USARTx == USART1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if(USARTx == USART2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if(USARTx == USART3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    }
}
//********************************************************************************************************************************************************************************************************

static void _enableUsartInterrupt(USART_TypeDef *USARTx)
{
    if(USARTx == USART1)
    {
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if(USARTx == USART2)
    {
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if(USARTx == USART3)
    {
        NVIC_EnableIRQ(USART3_IRQn);
    }
}
//********************************************************************************************************************************************************************************************************

static void _lowInit(UsartHandle *handle)
{
    if(handle != NULL)
    {
        _enableUsartClock(handle->usart);

        handle->usart->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
        handle->usart->CR2 &= ~USART_CR2_STOP;
        handle->usart->CR1 |= handle->conf->USART_InitStruct.USART_Mode | handle->conf->USART_InitStruct.USART_WordLength | handle->conf->USART_InitStruct.USART_Parity;
        handle->usart->CR2 |= handle->conf->USART_InitStruct.USART_StopBits;
        handle->usart->BRR = _computeBaudrate(handle->conf->McuClock, handle->conf->USART_InitStruct.USART_BaudRate);
        handle->usart->CR1 |= USART_CR1_UE;

        if(handle->conf->UseIt == true)
        {
            _enableUsartInterrupt(handle->usart);
            handle->usart->CR1 |= USART_CR1_RXNEIE;
        }
    }
}
//********************************************************************************************************************************************************************************************************

static void _writeSinglePolling(UsartHandle *handle, uint8_t data)
{
    while(!(handle->usart->SR & USART_SR_TXE))
    {
    }
    handle->usart->DR = (data & 0xFF);
}
//********************************************************************************************************************************************************************************************************

static void _writeSingleInterrupt(UsartHandle *handle, uint8_t data)
{
    if(handle->txCnt < TX_BUFFER_SIZE)
    {
        __disable_irq();
        handle->txBuffer[handle->txBufferIn++] = data;
        handle->txCnt++;
        __enable_irq();
        if(handle->txBufferIn >= TX_BUFFER_SIZE)
        {
            handle->txBufferIn = 0;
        }
        handle->usart->CR1 |= USART_CR1_TXEIE;
    }
}
//********************************************************************************************************************************************************************************************************

static uint8_t _readSinglePolling(UsartHandle *handle)
{
    uint8_t data = 0;

    while(!(handle->usart->SR & USART_SR_RXNE))
    {
    }
    data = handle->usart->DR;
    handle->rxPollingAvailable = true;

    return data;
}
//********************************************************************************************************************************************************************************************************

static uint8_t _readSingleInterrupt(UsartHandle *handle)
{
    uint8_t data = 0;

    if(handle->rxCnt > 0)
    {
        __disable_irq();
        data = handle->rxBuffer[handle->rxBufferOut++];
        handle->rxCnt--;
        __enable_irq();
        if(handle->rxBufferOut >= RX_BUFFER_SIZE)
        {
            handle->rxBufferOut = 0;
        }
    }

    return data;
}
//********************************************************************************************************************************************************************************************************

static void _usartIrqHandler(UsartHandle *handle)
{
    if(handle != NULL)
    {
        if(handle->usart->SR & USART_SR_TXE)
        {
            if(handle->txCnt == 0)
            {
                handle->usart->CR1 &= ~USART_CR1_TXEIE;
            }
            else
            {
                handle->usart->DR = handle->txBuffer[handle->txBufferOut++];
                handle->txCnt--;
                if(handle->txBufferOut >= TX_BUFFER_SIZE)
                {
                    handle->txBufferOut = 0;
                }
            }
        }

        if(handle->usart->SR & USART_SR_RXNE)
        {
            handle->rxBuffer[handle->rxBufferIn++] = handle->usart->DR;
            handle->rxCnt++;
            if(handle->rxBufferIn >= RX_BUFFER_SIZE)
            {
                handle->rxBufferIn = 0;
            }
        }
    }
}
//********************************************************************************************************************************************************************************************************
// Interruption Vector
//********************************************************************************************************************************************************************************************************
void USART1_IRQHandler(void)
{
    _usartIrqHandler(&g_usart1);
}

void USART2_IRQHandler(void)
{
    _usartIrqHandler(&g_usart2);
}

void USART3_IRQHandler(void)
{
    _usartIrqHandler(&g_usart3);
}
