#ifndef SRM_RX_PARSER_H
#define SRM_RX_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "srm_defs.h"

typedef enum
{
    SRM_RX_WAIT_SOF = 0,
    SRM_RX_READ_HEADER,
    SRM_RX_READ_PAYLOAD,
    SRM_RX_READ_CRC_H,
    SRM_RX_READ_CRC_L
} srm_rx_state_t;

typedef struct
{
    srm_rx_state_t state;
    uint8_t frame[SRM_MAX_FRAME_SIZE];
    uint16_t index;
    uint8_t payload_len;
    uint16_t total_len;
    bool frame_ready;
} srm_rx_parser_t;

void srm_rx_parser_init(srm_rx_parser_t *parser);
bool srm_rx_parser_feed(srm_rx_parser_t *parser, uint8_t byte);
void srm_rx_parser_reset(srm_rx_parser_t *parser);

#ifdef __cplusplus
}
#endif

#endif /* SRM_RX_PARSER_H */
