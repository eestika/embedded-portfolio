#include "srm_rx_parser.h"

void srm_rx_parser_reset(srm_rx_parser_t *parser)
{
    if (parser == 0)
    {
        return;
    }

    parser->state = SRM_RX_WAIT_SOF;
    parser->index = 0U;
    parser->payload_len = 0U;
    parser->total_len = 0U;
    parser->frame_ready = false;
}

void srm_rx_parser_init(srm_rx_parser_t *parser)
{
    srm_rx_parser_reset(parser);
}

bool srm_rx_parser_feed(srm_rx_parser_t *parser, uint8_t byte)
{
    if (parser == 0)
    {
        return false;
    }

    switch (parser->state)
    {
        case SRM_RX_WAIT_SOF:
            if (byte == SRM_SOF)
            {
                parser->frame[0] = byte;
                parser->index = 1U;
                parser->state = SRM_RX_READ_HEADER;
            }
            break;

        case SRM_RX_READ_HEADER:
            parser->frame[parser->index++] = byte;

            if (parser->index == SRM_HEADER_SIZE)
            {
                parser->payload_len = parser->frame[7];

                if (parser->payload_len > SRM_MAX_PAYLOAD_LEN)
                {
                    srm_rx_parser_reset(parser);
                    return false;
                }

                parser->total_len = (uint16_t)(SRM_HEADER_SIZE + parser->payload_len + SRM_CRC_SIZE);

                if (parser->payload_len == 0U)
                {
                    parser->state = SRM_RX_READ_CRC_H;
                }
                else
                {
                    parser->state = SRM_RX_READ_PAYLOAD;
                }
            }
            break;

        case SRM_RX_READ_PAYLOAD:
            parser->frame[parser->index++] = byte;

            if (parser->index == (uint16_t)(SRM_HEADER_SIZE + parser->payload_len))
            {
                parser->state = SRM_RX_READ_CRC_H;
            }
            break;

        case SRM_RX_READ_CRC_H:
            parser->frame[parser->index++] = byte;
            parser->state = SRM_RX_READ_CRC_L;
            break;

        case SRM_RX_READ_CRC_L:
            parser->frame[parser->index++] = byte;

            if (parser->index == parser->total_len)
            {
                parser->frame_ready = true;
                return true;
            }
            else
            {
                srm_rx_parser_reset(parser);
                return false;
            }

        default:
            srm_rx_parser_reset(parser);
            return false;
    }

    return false;
}
