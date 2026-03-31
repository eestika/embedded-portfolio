#include "srm_frame_builder.h"
#include "srm_defs.h"
#include "srm_crc.h"

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
                    uint8_t payload_len)
{
    size_t total_len;
    size_t idx;
    size_t i;
    uint16_t crc;

    if ((out_buf == NULL) || (out_len == NULL))
    {
        return -1;
    }

    if (payload_len > SRM_MAX_PAYLOAD_LEN)
    {
        return -1;
    }

    total_len = SRM_HEADER_SIZE + payload_len + SRM_CRC_SIZE;
    if (out_buf_size < total_len)
    {
        return -1;
    }

    idx = 0U;

    out_buf[idx++] = SRM_SOF;
    out_buf[idx++] = ver;
    out_buf[idx++] = flags;
    out_buf[idx++] = src;
    out_buf[idx++] = dst;
    out_buf[idx++] = cmd;
    out_buf[idx++] = seq;
    out_buf[idx++] = payload_len;

    for (i = 0U; i < payload_len; i++)
    {
        out_buf[idx++] = payload[i];
    }

    crc = srm_crc16_ccitt_false(&out_buf[1], (size_t)(7U + payload_len));

    out_buf[idx++] = (uint8_t)((crc >> 8) & 0xFFU);
    out_buf[idx++] = (uint8_t)(crc & 0xFFU);

    *out_len = idx;
    return 0;
}