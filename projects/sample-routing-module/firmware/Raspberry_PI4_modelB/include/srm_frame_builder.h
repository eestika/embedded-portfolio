#ifndef SRM_FRAME_BUILDER_H
#define SRM_FRAME_BUILDER_H

#include <stddef.h>
#include <stdint.h>

int srm_build_frame(uint8_t *out_buf,
                    size_t out_buf_size,
                    size_t *out_len,
                    uint8_t ver,
                    uint8_t flags,
                    uint8_t src,
                    uint8_t dst,
                    uint8_t cmd,
                    uint8_t seq,
                    const uint8_t *payload,
                    uint8_t payload_len);

#endif /* SRM_FRAME_BUILDER_H */