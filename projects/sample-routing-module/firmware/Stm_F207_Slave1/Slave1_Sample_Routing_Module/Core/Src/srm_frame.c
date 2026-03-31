#include "srm_frame.h"
#include "srm_crc.h"
#include "srm_defs.h"

size_t srm_frame_total_size(uint8_t payload_len)
{
    return (size_t)SRM_HEADER_SIZE + (size_t)payload_len + (size_t)SRM_CRC_SIZE;
}

srm_status_t srm_frame_build(
    srm_frame_t *frame,
    uint8_t flags,
    uint8_t src,
    uint8_t dst,
    uint8_t cmd,
    uint8_t seq,
    const uint8_t *payload,
    uint8_t payload_len)
{
    uint8_t i;

    if (frame == NULL)
    {
        return SRM_ERR_NULL;
    }

    if (payload_len > SRM_MAX_PAYLOAD_LEN)
    {
        return SRM_ERR_PAYLOAD_TOO_LONG;
    }

    frame->ver = SRM_VERSION;
    frame->flags = flags;
    frame->src = src;
    frame->dst = dst;
    frame->cmd = cmd;
    frame->seq = seq;
    frame->len = payload_len;

    for (i = 0U; i < payload_len; i++)
    {
        frame->payload[i] = payload[i];
    }

    return SRM_OK;
}

srm_status_t srm_frame_serialize(
    const srm_frame_t *frame,
    uint8_t *out_buf,
    size_t out_buf_size,
    size_t *out_len)
{
    size_t total_len;
    size_t crc_region_len;
    uint16_t crc;
    uint8_t i;
    size_t idx;

    if ((frame == NULL) || (out_buf == NULL) || (out_len == NULL))
    {
        return SRM_ERR_NULL;
    }

    if (frame->ver != SRM_VERSION)
    {
        return SRM_ERR_INVALID_VERSION;
    }

    total_len = srm_frame_total_size(frame->len);

    if (out_buf_size < total_len)
    {
        return SRM_ERR_BUFFER_TOO_SMALL;
    }

    idx = 0U;

    out_buf[idx++] = SRM_SOF;
    out_buf[idx++] = frame->ver;
    out_buf[idx++] = frame->flags;
    out_buf[idx++] = frame->src;
    out_buf[idx++] = frame->dst;
    out_buf[idx++] = frame->cmd;
    out_buf[idx++] = frame->seq;
    out_buf[idx++] = frame->len;

    for (i = 0U; i < frame->len; i++)
    {
        out_buf[idx++] = frame->payload[i];
    }

    crc_region_len = (size_t)7U + (size_t)frame->len;
    crc = srm_crc16_ccitt_false(&out_buf[1], crc_region_len);

    out_buf[idx++] = (uint8_t)((crc >> 8) & 0xFFU);
    out_buf[idx++] = (uint8_t)(crc & 0xFFU);

    *out_len = idx;

    return SRM_OK;
}
