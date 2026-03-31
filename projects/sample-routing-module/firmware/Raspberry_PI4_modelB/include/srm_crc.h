#ifndef SRM_CRC_H
#define SRM_CRC_H

#include <stddef.h>
#include <stdint.h>

uint16_t srm_crc16_ccitt_false(const uint8_t *data, size_t len);

#endif /* SRM_CRC_H */