#ifndef SRM_FRAME_H
#define SRM_FRAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include "srm_types.h"

size_t srm_frame_total_size(uint8_t payload_len);

srm_status_t srm_frame_build(
    srm_frame_t *frame,
    uint8_t flags,
    uint8_t src,
    uint8_t dst,
    uint8_t cmd,
    uint8_t seq,
    const uint8_t *payload,
    uint8_t payload_len);

srm_status_t srm_frame_serialize(
    const srm_frame_t *frame,
    uint8_t *out_buf,
    size_t out_buf_size,
    size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif /* SRM_FRAME_H */
