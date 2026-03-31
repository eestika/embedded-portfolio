#ifndef SRM_TYPES_H
#define SRM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "srm_defs.h"

typedef struct
{
    uint8_t ver;
    uint8_t flags;
    uint8_t src;
    uint8_t dst;
    uint8_t cmd;
    uint8_t seq;
    uint8_t len;
    uint8_t payload[SRM_MAX_PAYLOAD_LEN];
} srm_frame_t;

typedef enum
{
    SRM_OK = 0,
    SRM_ERR_NULL,
    SRM_ERR_PAYLOAD_TOO_LONG,
    SRM_ERR_BUFFER_TOO_SMALL,
    SRM_ERR_INVALID_VERSION
} srm_status_t;

#ifdef __cplusplus
}
#endif

#endif /* SRM_TYPES_H */
