#include "SpiConf.h"

const SPI_Conf g_SpiConf = 
{
    .DataFrameFormat = SPI_DFF_8,
    .SoftwareSlaveManagement=SPI_SSM_Enable,
    .InternalSLaveSelect = SPI_SSI_Enable,
    .FrameFormat=SPI_LSBFIRST_MSB,
    .SpiEnable=SPI_SPE_Enabled,
    .BaudRateControl=SPI_BR_64,
    .MasterSelection=SPI_MSTR_Master,
    .ClockPolarity=SPI_CPOL_0,
    .ClockPhase=SPI_CPHA_0,
    .UseIt = false,
    .CsPin = SPI1_SS,
    .CsPort = GPIOA,
};
