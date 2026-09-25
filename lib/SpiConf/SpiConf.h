#ifndef SPI_CONF_H
#define SPI_CONF_H

#include <stdint.h>
#include <stdbool.h>
#include "GpioConf.h"

typedef enum
{
    SPI_BIDIMODE_2LUdDMS = 0x0000,
    SPI_BIDIMODE_1LBdDMS = 0x8000,
}SPI_BIDIMODE;

typedef enum 
{
    SPI_BIDIOE_OutputDisable = 0x0000,
    SPI_BIDIOE_OutputEnable  = 0x4000,
}SPI_BIDIOE;

typedef enum 
{
    SPI_CRCEN_Disabled = 0x0000,
    SPI_CRCEN_Enable = 0x2000,
}SPI_CRCEN;

typedef enum 
{
    SPI_CRCNEXT_NCRCP = 0x0000,
    SPI_CRCNEXT_CRCP  = 0x1000,
}SPI_CRCNEXT;

typedef enum
{
    SPI_DFF_8   = 0x0000,
    SPI_DFF_16  = 0x0800,
}SPI_DFF;

typedef enum 
{
    SPI_RXONLY_FullDuplex = 0x0000,
    SPI_RXONLY_OutputDisabled = 0x0400,
}SPI_RXONLY;

typedef enum
{
    SPI_SSM_Disabled = 0x0000,
    SPI_SSM_Enable = 0x0200,
}SPI_SSM;

typedef enum
{
    SPI_SSI_Disabled = 0x0000,
    SPI_SSI_Enable = 0x0100 
}SPI_SSI;

typedef enum
{
    SPI_LSBFIRST_MSB = 0x0000,
    SPI_LSBFIRST_LSB = 0x0080,
}SPI_LSBFIRST;

typedef enum
{
    SPI_SPE_Disabled = 0x0000,
    SPI_SPE_Enabled = 0x0040,
}SPI_SPE;

typedef enum
{
    SPI_BR_2 = 0x0000,
    SPI_BR_4 = 0x0008,
    SPI_BR_8 = 0x0010,
    SPI_BR_16 = 0x0018,
    SPI_BR_32 = 0x0020,
    SPI_BR_64 = 0x0028,
    SPI_BR_128 = 0x0030,
    SPI_BR_256 = 0x0038,
}SPI_BR;

typedef enum
{
    SPI_MSTR_Master = 0x0004,
    SPI_MSTR_Slave  = 0x0000,
}SPI_MSTR;

typedef enum 
{
    SPI_CPOL_0 = 0x0000,
    SPI_CPOL_1 = 0x0002
}SPI_CPOL;

typedef enum 
{
    SPI_CPHA_0 = 0x0000,
    SPI_CPHA_1 = 0x0001
}SPI_CPHA;

typedef enum
{
    SPI_MODE_Polling,
    SPI_MODE_Interrupt,
    SPI_MODE_DMA
}SPI_Mode;

typedef struct 
{
    SPI_BIDIMODE BidirectionalDataMode;
    SPI_BIDIOE BidirectionalOutput;
    SPI_CRCEN CrcCalculation;
    SPI_CRCNEXT CrcTransfertNext;
    SPI_DFF DataFrameFormat;
    SPI_RXONLY ReceiveOnly;
    SPI_SSM SoftwareSlaveManagement;
    SPI_SSI InternalSLaveSelect;
    SPI_LSBFIRST FrameFormat;
    SPI_SPE SpiEnable;
    SPI_BR BaudRateControl;
    SPI_MSTR MasterSelection;
    SPI_CPOL ClockPolarity;
    SPI_CPHA ClockPhase;
    bool UseIt;
    GPIO_TypeDef *CsPort;
    uint16_t CsPin;
}SPI_Conf;

extern const SPI_Conf g_SpiConf;


#endif // SPI_CONF_H