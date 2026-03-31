#ifndef SRM_RX_BUFFER_H
#define SRM_RX_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define SRM_RX_BUFFER_SIZE    256U

typedef struct
{
    uint8_t data[SRM_RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t count;
} srm_rx_buffer_t;

void srm_rx_buffer_init(srm_rx_buffer_t *buffer);
bool srm_rx_buffer_push(srm_rx_buffer_t *buffer, uint8_t byte);
bool srm_rx_buffer_pop(srm_rx_buffer_t *buffer, uint8_t *byte);
bool srm_rx_buffer_is_empty(const srm_rx_buffer_t *buffer);
bool srm_rx_buffer_is_full(const srm_rx_buffer_t *buffer);
uint16_t srm_rx_buffer_count(const srm_rx_buffer_t *buffer);

#ifdef __cplusplus
}
#endif

#endif /* SRM_RX_BUFFER_H */
