//********************************************************************************************************************************************************************************************************
// Includes
//********************************************************************************************************************************************************************************************************
#include "Usart.h"

//********************************************************************************************************************************************************************************************************
// Private Variable
//********************************************************************************************************************************************************************************************************
static uint8_t txBuffer[TX_BUFFER_SIZE];
static volatile uint8_t txBufferIn = 0;
static volatile uint8_t txBufferOut = 0;

static uint8_t rxBuffer[RX_BUFFER_SIZE];
static volatile uint8_t rxBufferIn = 0;
static volatile uint8_t rxBufferOut = 0;

volatile static uint8_t txCnt=0;
volatile static uint8_t rxCnt=0;

static bool rxPollingAvailable = false;

static const UsartConf *s_conf = NULL;
static const USART_TypeDef *s_usart = NULL;

//********************************************************************************************************************************************************************************************************
// Private Function Declaration
//********************************************************************************************************************************************************************************************************
static uint16_t _computeBaudrate(uint32_t mcuClk, uint32_t br);
static void _lowInit(const UsartConf *conf);
static void _writeSinglePolling(uint8_t data);
static void _writeSingleInterrupt(uint8_t data);
static uint8_t _readSinglePolling(void);
static uint8_t _readSingleInterrupt(void);

//********************************************************************************************************************************************************************************************************
// Public Function Definition
//********************************************************************************************************************************************************************************************************
void Usart_Init(const UsartConf *conf, USART_TypeDef *usart)
{
    if(conf != NULL)
    {
        s_usart = usart;
        s_conf = conf;
        
        _lowInit(s_conf);
    }
}
//********************************************************************************************************************************************************************************************************

void Usart_WriteByte(uint8_t data)
{ 
    if(s_conf->UseIt == true)
    {
        _writeSingleInterrupt(data);
    }
    else
    {
        _writeSinglePolling(data);
    }
}   
//********************************************************************************************************************************************************************************************************

void Usart_WriteBytes(uint8_t *data, uint8_t size)
{
   for(uint8_t i = 0; i < size; i++)
   {
        Usart_WriteByte(data[i]);
   }
}
//********************************************************************************************************************************************************************************************************

bool Usart_RxAvailable(void)
{
    bool returnValue = false;

    if(s_conf->UseIt == true)
    {
        if(rxCnt > 0)
        {
            returnValue = true;
        }
    }
    else
    {
        if(rxPollingAvailable == true)
        {
            returnValue = true;
            rxPollingAvailable = false;
        }
    }

    return returnValue;
}
//********************************************************************************************************************************************************************************************************

uint8_t Usart_ReadByte(void)
{
    uint8_t data = 0;

    if(s_conf->UseIt == true)
    {
        data = _readSingleInterrupt();
    }
    else
    {
        data = _readSinglePolling();
    }

    return data;
}

//********************************************************************************************************************************************************************************************************
// Private Function Definition
//********************************************************************************************************************************************************************************************************
uint16_t _computeBaudrate(uint32_t mcuClk, uint32_t br)
{
    return ((mcuClk + (br/2U))/br);
}
//********************************************************************************************************************************************************************************************************

static void _lowInit(const UsartConf *conf)
{
    //Clock access to USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    //USART 8N1
    USART1->CR1 &= ~USART_CR1_M;

    //USART transmiter enable
    USART1->CR1 |= USART_CR1_TE;

    //USART receiver enable
    USART1->CR1 |= USART_CR1_RE;

    USART1->CR1 |= USART_CR1_RXNEIE;

    USART1->BRR = _computeBaudrate(conf->McuClock, conf->Baudrate);

    //USART enable
    USART1->CR1 |= USART_CR1_UE;
   
    if(conf->UseIt == true)
    {
        NVIC_EnableIRQ(USART1_IRQn);
    }
}

static void _writeSinglePolling(uint8_t data)
{
    while(!(USART1->SR & USART_SR_TXE)){}
 	USART1->DR	=  (data & 0xFF);
}

static void _writeSingleInterrupt(uint8_t data)
{
    if(txCnt < TX_BUFFER_SIZE)
    {
        __disable_irq();
        txBuffer[txBufferIn++] = data;
        txCnt++;
        __enable_irq();
        if(txBufferIn >= TX_BUFFER_SIZE)
        {
            txBufferIn = 0;
        }
        USART1->CR1 |= USART_CR1_TXEIE;
    }
}

static uint8_t _readSinglePolling(void)
{
    uint8_t data = 0;

    while(!(USART1->SR & USART_SR_RXNE)){}
    data = USART1->DR;
    rxPollingAvailable = true;

    return data;
}

static uint8_t _readSingleInterrupt(void)
{
    uint8_t data = 0;

    if(rxCnt > 0)
    {
        __disable_irq();
        data = rxBuffer[rxBufferOut++];
        rxCnt--;
        __enable_irq();
        if(rxBufferOut >= RX_BUFFER_SIZE)
        {
            rxBufferOut = 0;
        } 
    }

    return data;
}

//********************************************************************************************************************************************************************************************************
// Interruption Vector
//********************************************************************************************************************************************************************************************************
void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_TXE)
    {
        if(txCnt == 0)
        {
            USART1->CR1 &= ~USART_CR1_TXEIE;
        }
        else
        {
            USART1->DR = txBuffer[txBufferOut++];
            txCnt--;
            if(txBufferOut >= TX_BUFFER_SIZE)
            {
                txBufferOut = 0;
            }
        }
    }

    if(USART1->SR & USART_SR_RXNE)
    {
        rxBuffer[rxBufferIn++] = USART1->DR;
        rxCnt++;
        if(rxBufferIn >= RX_BUFFER_SIZE)
        {
            rxBufferIn = 0;
        }
    }
}
