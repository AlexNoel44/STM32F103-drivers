#include "Neo6m.h"
#include <stdlib.h>
#include <string.h>

typedef enum
{
    NEO6M_WAIT_START,
    NEO6M_RECEIVING,
}Neo6mState;

typedef struct
{
    float latitude;
    float longitude;
    float altitude;

    float hdop;

    uint8_t satellites;
    uint8_t fix;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

} Neo6mData;

#define NEO6M_FRAME_SIZE 128
static Neo6mState neo6m_state = NEO6M_WAIT_START;
static Neo6mData neo6m_data;
static uint8_t frame[NEO6M_FRAME_SIZE];
static uint8_t frame_index = 1;

static void _processFrame(const char *frame);
static void _parseFrame(const char *frame, Neo6mData *data);

void Neo6m_Init(void);

void Neo6m_Process(uint8_t data)
{
    _constrcutFrame(data);
}

float Neo6m_GetLatitude(void)
{
    return neo6m_data.latitude;
}

float Neo6m_GetLongitude(void)
{
    return neo6m_data.longitude;
}

float Neo6m_GetAltitude(void)
{
    return neo6m_data.altitude;
}

float Neo6m_GetHDOP(void)
{
    return neo6m_data.hdop;
}

uint8_t Neo6m_GetSatellites(void)
{
    return neo6m_data.satellites;
}

uint8_t Neo6m_GetFix(void)
{
    return neo6m_data.fix;
}

uint8_t Neo6m_Hour(void)
{
    return neo6m_data.hour;
}

uint8_t Neo6m_Minute(void)
{
    return neo6m_data.minute;
}

uint8_t Neo6m_Second(void)
{
    return neo6m_data.second;
}

void _constrcutFrame(uint8_t data)
{
    switch(neo6m_state)
    {
        case NEO6M_WAIT_START:
            if(data == '$')
            {
                neo6m_state = NEO6M_RECEIVING;
                frame[0] = data;
                frame_index = 1;
            }
            break;

        case NEO6M_RECEIVING:
            if(frame_index >= NEO6M_FRAME_SIZE - 1)
            {
                neo6m_state = NEO6M_WAIT_START;
                frame_index = 0;
                break;
            }

            frame[frame_index++] = data;

            if(data == '\n')
            {
                frame[frame_index] = '\0';
                _processFrame((const char *)frame);
                neo6m_state = NEO6M_WAIT_START;
                frame_index = 0;
            }
            break;
        default:
            break;
    }
}

uint8_t _getChecksum(const char *frame, uint8_t *checksum)
{
    const char *star = strchr(frame, '*');

    if(star == NULL)
    {
        return 0;
    }

    // À compléter : vérifier les deux caractères hexadécimaux
    char checksumStr[3];

    checksumStr[0] = star[1];
    checksumStr[1] = star[2];
    checksumStr[2] = '\0';

    *checksum = (uint8_t)strtoul(checksumStr, NULL, 16);

    return 1;
}

uint8_t _checksumCalc(const char *frame, uint8_t *checksum)
{
    int i = 1;
    *checksum = 0;

    if(frame[0] == '$')
    {
        while(frame[i] != '*' && frame[i] != '\0')
        {
            *checksum ^= frame[i];
            i++;
        }
    }
    return 1;
}

static uint8_t _validateFrame(const char *frame)
{
    uint8_t receivedChecksum;
    uint8_t calculatedChecksum;

    if(!_getChecksum(frame, &receivedChecksum))
    {
        return 0;
    }

    _checksumCalc(frame, &calculatedChecksum);

    return receivedChecksum == calculatedChecksum;
}

static void _processFrame(const char *frame)
{
    if(_validateFrame(frame))
    {
        _parseFrame(frame, &neo6m_data);
    }
}

static void _parseFrame(const char *frame, Neo6mData *data)
{
    const char *star = strchr(frame, '*');
    char sentence[NEO6M_FRAME_SIZE];
    char *fields[10];
    size_t length;
    size_t field_count = 1;

    if(star == NULL)
    {
        return;
    }

    length = (size_t)(star - frame);
    if(length >= sizeof(sentence))
    {
        return;
    }

    memcpy(sentence, frame, length);
    sentence[length] = '\0';

    if(strncmp(sentence, "$GPGGA,", 7) != 0 && strncmp(sentence, "$GNGGA,", 7) != 0)
    {
        return;
    }

    fields[0] = sentence;
    for(char *separator = sentence; *separator != '\0' && field_count < 10; separator++)
    {
        if(*separator == ',')
        {
            *separator = '\0';
            fields[field_count++] = separator + 1;
        }
    }

    if(field_count < 10)
    {
        return;
    }

    if(fields[1][0] != '\0')
    {
        unsigned int time = (unsigned int)strtoul(fields[1], NULL, 10);
        data->hour = (uint8_t)(time / 10000);
        data->minute = (uint8_t)((time / 100) % 100);
        data->second = (uint8_t)(time % 100);
    }
    else
    {
        data->hour = 0;
        data->minute = 0;
        data->second = 0;
    }

    if(fields[2][0] != '\0' && fields[3][0] != '\0')
    {
        float latitude = (float)atof(fields[2]);
        data->latitude = (float)((int)(latitude / 100)) + (latitude - (float)((int)(latitude / 100) * 100)) / 60.0f;
        if(fields[3][0] == 'S')
        {
            data->latitude = -data->latitude;
        }
    }
    else
    {
        data->latitude = 0.0f;
    }

    if(fields[4][0] != '\0' && fields[5][0] != '\0')
    {
        float longitude = (float)atof(fields[4]);
        data->longitude = (float)((int)(longitude / 100)) +
                          (longitude - (float)((int)(longitude / 100) * 100)) / 60.0f;
        if(fields[5][0] == 'W')
        {
            data->longitude = -data->longitude;
        }
    }
    else
    {
        data->longitude = 0.0f;
    }

    data->fix = (uint8_t)atoi(fields[6]);
    data->satellites = (uint8_t)atoi(fields[7]);
    data->hdop = (float)atof(fields[8]);
    data->altitude = (float)atof(fields[9]);
}