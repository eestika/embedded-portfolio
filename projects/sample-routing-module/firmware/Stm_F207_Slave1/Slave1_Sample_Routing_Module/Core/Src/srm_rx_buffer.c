#include "srm_rx_buffer.h"

void srm_rx_buffer_init(srm_rx_buffer_t *buffer)
{
    if (buffer == 0)
    {
        return;
    }

    buffer->head = 0U;
    buffer->tail = 0U;
    buffer->count = 0U;
}

bool srm_rx_buffer_is_empty(const srm_rx_buffer_t *buffer)
{
    if (buffer == 0)
    {
        return true;
    }

    return (buffer->count == 0U);
}

bool srm_rx_buffer_is_full(const srm_rx_buffer_t *buffer)
{
    if (buffer == 0)
    {
        return false;
    }

    return (buffer->count >= SRM_RX_BUFFER_SIZE);
}

uint16_t srm_rx_buffer_count(const srm_rx_buffer_t *buffer)
{
    if (buffer == 0)
    {
        return 0U;
    }

    return buffer->count;
}

bool srm_rx_buffer_push(srm_rx_buffer_t *buffer, uint8_t byte)
{
    uint16_t next_head;

    if (buffer == 0)
    {
        return false;
    }

    if (srm_rx_buffer_is_full(buffer))
    {
        return false;
    }

    buffer->data[buffer->head] = byte;

    next_head = (uint16_t)(buffer->head + 1U);
    if (next_head >= SRM_RX_BUFFER_SIZE)
    {
        next_head = 0U;
    }

    buffer->head = next_head;
    buffer->count++;

    return true;
}

bool srm_rx_buffer_pop(srm_rx_buffer_t *buffer, uint8_t *byte)
{
    uint16_t next_tail;

    if ((buffer == 0) || (byte == 0))
    {
        return false;
    }

    if (srm_rx_buffer_is_empty(buffer))
    {
        return false;
    }

    *byte = buffer->data[buffer->tail];

    next_tail = (uint16_t)(buffer->tail + 1U);
    if (next_tail >= SRM_RX_BUFFER_SIZE)
    {
        next_tail = 0U;
    }

    buffer->tail = next_tail;
    buffer->count--;

    return true;
}
