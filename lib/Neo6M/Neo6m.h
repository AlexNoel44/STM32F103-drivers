#include "Usart.h"

void Neo6m_Init(void);
void Neo6m_Process(uint8_t data);
float Neo6m_GetLatitude(void);
float Neo6m_GetLongitude(void);
float Neo6m_GetAltitude(void);
float Neo6m_GetHDOP(void);
uint8_t Neo6m_GetSatellites(void);
uint8_t Neo6m_GetFix(void);
uint8_t Neo6m_Hour(void);
uint8_t Neo6m_Minute(void);
uint8_t Neo6m_Second(void);