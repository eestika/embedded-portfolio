#include "srm_crc.h"

uint16_t srm_crc16_ccitt_false(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFFU;
    size_t i;
    uint8_t bit;

    if (data == 0)
    {
        return crc;
    }

    for (i = 0; i < len; i++)
    {
        crc ^= ((uint16_t)data[i] << 8);

        for (bit = 0; bit < 8U; bit++)
        {
            if ((crc & 0x8000U) != 0U)
            {
                crc = (uint16_t)((crc << 1) ^ 0x1021U);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}