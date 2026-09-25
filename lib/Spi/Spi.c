#include "Spi.h"

//********************************************************************************************************************************************************************************************************
// Private Variable
//********************************************************************************************************************************************************************************************************
static uint8_t *txBuffer;
static uint8_t *rxBuffer;

//********************************************************************************************************************************************************************************************************
// Private Interrupt Variable
//********************************************************************************************************************************************************************************************************
volatile uint8_t txIndex = 0;
volatile uint8_t rxIndex = 0;
volatile uint8_t bufferSize = 0;
volatile bool busy = false;
volatile bool done = false;

const static SPI_Conf *s_conf = NULL;
static SPI_TypeDef *s_spix = NULL;

//********************************************************************************************************************************************************************************************************
// Private Function Declaration
//********************************************************************************************************************************************************************************************************
static void _enableSpiClock(SPI_TypeDef* SPIx);
static void _enableSpiInterrupt(SPI_TypeDef* SPIx);
static void _spiIrqHandler(void);
static bool _transferPolling(uint8_t *txData, uint8_t *rxData, uint8_t size);
static bool _transferInterrupt(uint8_t *txData, uint8_t *rxData, uint8_t size);
static bool _transfertDma(uint8_t *txData, uint8_t *rxData, uint8_t size);
static void _csHigh();
static void _csLow();

//********************************************************************************************************************************************************************************************************
// Public Function Definition
//********************************************************************************************************************************************************************************************************
void Spi_Init(const SPI_Conf *conf, SPI_TypeDef* SPIx)
{
    if(conf != NULL)
    {
        s_conf = conf;
        s_spix = SPIx;

        _enableSpiClock(s_spix);

        s_spix->CR1 |= conf->DataFrameFormat | conf->SoftwareSlaveManagement | conf->InternalSLaveSelect | conf->FrameFormat | conf->SpiEnable | conf->BaudRateControl | conf->MasterSelection | conf->ClockPolarity | conf->ClockPhase;

        if(s_conf->UseIt == true)
        {
            _enableSpiInterrupt(s_spix);
            s_spix->CR2 |= SPI_CR2_RXNEIE;
        }
    }
}
//********************************************************************************************************************************************************************************************************

void Spi_Transfer(uint8_t *txData, uint8_t *rxData, uint8_t size)
{
    if(s_conf->UseIt == true)
    {
        _transferInterrupt(txData, rxData, size);
    }
    else
    {
        _transferPolling(txData, rxData, size);
    }
}
//********************************************************************************************************************************************************************************************************

bool Spi_TransferDone(void)
{
    bool returnValue = false;

    if(done == true)
    {
        done = false;
        returnValue = true;
    }

    return returnValue;
}

//********************************************************************************************************************************************************************************************************
// Private Function Definition
//********************************************************************************************************************************************************************************************************
static void _enableSpiClock(SPI_TypeDef* SPIx)
{
    if(SPIx == SPI1)
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    else if(SPIx == SPI2)
        RCC->APB2ENR |= RCC_APB1ENR_SPI2EN;
}
//********************************************************************************************************************************************************************************************************

static void _enableSpiInterrupt(SPI_TypeDef* SPIx)
{
    if (SPIx == SPI1)
    {
        NVIC_EnableIRQ(SPI1_IRQn);
    }
    else if (SPIx == SPI2)
    {
        NVIC_EnableIRQ(SPI2_IRQn);
    }
}
//********************************************************************************************************************************************************************************************************

static void _spiIrqHandler(void)
{
    if(s_spix->SR & SPI_SR_TXE)
    {
        if(txIndex < bufferSize)
        {
            s_spix->DR = txBuffer[txIndex++];
        }
        else
        {
            s_spix->CR2 &= ~SPI_CR2_TXEIE;
        }
    }

    if(s_spix->SR & SPI_SR_RXNE)
    {
        rxBuffer[rxIndex++] = s_spix->DR;

        if(rxIndex >= bufferSize)
        {
            busy = false;
            done = true;
            _csHigh();
            s_spix->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE);
        }
    }
}
//********************************************************************************************************************************************************************************************************

void _csLow()
{
    Gpio_Low(s_conf->CsPort, s_conf->CsPin);

}
//********************************************************************************************************************************************************************************************************

void _csHigh()
{
    Gpio_High(s_conf->CsPort, s_conf->CsPin);
}
//********************************************************************************************************************************************************************************************************

static bool _transferPolling(uint8_t *txData, uint8_t *rxData, uint8_t size)
{
    bool returnValue = false;

    _csLow();

    for(uint8_t i = 0; i < size; i++)
    {
        while(!(s_spix->SR & SPI_SR_TXE));
        s_spix->DR = txData[i];
        while(!(s_spix->SR & SPI_SR_RXNE));
        rxData[i] = s_spix->DR;
        returnValue = true;
    }

    _csHigh();

    return returnValue;
}
//********************************************************************************************************************************************************************************************************

static bool _transferInterrupt(uint8_t *txData, uint8_t *rxData, uint8_t size)
{
    bool returnValue = false;

    if(busy == false)
    {
        if(txData != NULL)
        {
            txBuffer = txData;
        }
        if(rxData != NULL)
        {
            rxBuffer = rxData;
        }
        bufferSize = size;
        txIndex = 0;
        rxIndex = 0;
        busy = true;
        done = false;
        returnValue = true;
        _csLow();
        s_spix->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;
    }
    return returnValue;        
}

//********************************************************************************************************************************************************************************************************
// Interruption Vector
//********************************************************************************************************************************************************************************************************
void SPI1_IRQHandler(void)
{
    _spiIrqHandler();
}

void SPI2_IRQHandler(void)
{
    _spiIrqHandler();
}
