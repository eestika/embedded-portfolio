#ifndef SRM_CRC_H
#define SRM_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

uint16_t srm_crc16_ccitt_false(const uint8_t *data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* SRM_CRC_H */
