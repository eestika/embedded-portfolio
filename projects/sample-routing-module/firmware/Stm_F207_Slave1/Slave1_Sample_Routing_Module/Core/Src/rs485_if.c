#include "rs485_if.h"
#include "main.h"
#include "usart.h"
#include "uart_if.h"
#include "debug_console.h"
#include "srm_rx_buffer.h"
#include "srm_rx_parser.h"
#include "srm_crc.h"
#include "srm_defs.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define RS485_TX_TIMEOUT_MS    100U

static uart_if_t s_rs485_uart;
static srm_rx_buffer_t s_rx_buffer;
static srm_rx_parser_t s_rx_parser;
static uint32_t s_rx_overflow_count = 0U;

static void rs485_set_tx_mode(void)
{
#if defined(RS485_DE_Pin) && defined(RS485_DE_GPIO_Port)
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
#endif
}

static void rs485_set_rx_mode(void)
{
#if defined(RS485_DE_Pin) && defined(RS485_DE_GPIO_Port)
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);
#endif
}

static void rs485_on_rx_byte(uint8_t byte)
{
    if (!srm_rx_buffer_push(&s_rx_buffer, byte))
    {
        s_rx_overflow_count++;
    }
}

static void rs485_print_frame_hex(const uint8_t *frame, uint16_t len)
{
    char msg[8];
    uint16_t i;

    for (i = 0U; i < len; i++)
    {
        snprintf(msg, sizeof(msg), "%02X ", frame[i]);
        debug_console_write(msg);
    }

    debug_console_write_line("");
}

static bool rs485_send_srm_frame(uint8_t flags,
                                 uint8_t src,
                                 uint8_t dst,
                                 uint8_t cmd,
                                 uint8_t seq,
                                 const uint8_t *payload,
                                 uint8_t payload_len)
{
    uint8_t frame[SRM_MAX_FRAME_SIZE];
    uint16_t crc;
    uint16_t idx = 0U;
    uint8_t i;

    if (payload_len > SRM_MAX_PAYLOAD_LEN)
    {
        return false;
    }

    frame[idx++] = SRM_SOF;
    frame[idx++] = SRM_VERSION;
    frame[idx++] = flags;
    frame[idx++] = src;
    frame[idx++] = dst;
    frame[idx++] = cmd;
    frame[idx++] = seq;
    frame[idx++] = payload_len;

    for (i = 0U; i < payload_len; i++)
    {
        frame[idx++] = payload[i];
    }

    crc = srm_crc16_ccitt_false(&frame[1], (size_t)(7U + payload_len));

    frame[idx++] = (uint8_t)((crc >> 8) & 0xFFU);
    frame[idx++] = (uint8_t)(crc & 0xFFU);

    return rs485_if_send_bytes(frame, idx);
}

static void rs485_dispatch_srm_frame(const uint8_t *frame, uint16_t len)
{
    uint8_t src;
    uint8_t dst;
    uint8_t cmd;
    uint8_t seq;

    (void)len;

    if (frame == NULL)
    {
        return;
    }

    src = frame[3];
    dst = frame[4];
    cmd = frame[5];
    seq = frame[6];

    if (dst != SRM_NODE_ID_SLAVE1)
    {
        debug_console_write_line("SRM DISPATCH: frame not for this node");
        return;
    }

    switch (cmd)
    {
        case SRM_CMD_PING_REQ:
            debug_console_write_line("SRM DISPATCH: PING_REQ");

            if (rs485_send_srm_frame(0U,
                                     SRM_NODE_ID_SLAVE1,
                                     src,
                                     SRM_CMD_PING_RSP,
                                     seq,
                                     NULL,
                                     0U))
            {
                debug_console_write_line("SRM TX: PING_RSP sent");
            }
            else
            {
                debug_console_write_line("SRM TX ERROR: PING_RSP failed");
            }
            break;

        case SRM_CMD_LED_ON_REQ:
            debug_console_write_line("SRM DISPATCH: LED_ON_REQ");
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);

            if (rs485_send_srm_frame(0U,
                                     SRM_NODE_ID_SLAVE1,
                                     src,
                                     SRM_CMD_LED_ON_RSP,
                                     seq,
                                     NULL,
                                     0U))
            {
                debug_console_write_line("SRM TX: LED_ON_RSP sent");
            }
            else
            {
                debug_console_write_line("SRM TX ERROR: LED_ON_RSP failed");
            }
            break;

        case SRM_CMD_LED_OFF_REQ:
            debug_console_write_line("SRM DISPATCH: LED_OFF_REQ");
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

            if (rs485_send_srm_frame(0U,
                                     SRM_NODE_ID_SLAVE1,
                                     src,
                                     SRM_CMD_LED_OFF_RSP,
                                     seq,
                                     NULL,
                                     0U))
            {
                debug_console_write_line("SRM TX: LED_OFF_RSP sent");
            }
            else
            {
                debug_console_write_line("SRM TX ERROR: LED_OFF_RSP failed");
            }
            break;

        default:
            debug_console_write_line("SRM DISPATCH: unsupported CMD");
            break;
    }
}

static void rs485_handle_srm_frame(const uint8_t *frame, uint16_t len)
{
    uint8_t ver;
    uint8_t flags;
    uint8_t src;
    uint8_t dst;
    uint8_t cmd;
    uint8_t seq;
    uint8_t payload_len;
    uint16_t crc_rx;
    uint16_t crc_calc;
    char msg[96];

    if ((frame == NULL) || (len < (SRM_HEADER_SIZE + SRM_CRC_SIZE)))
    {
        debug_console_write_line("SRM RX ERROR: invalid frame length");
        return;
    }

    ver         = frame[1];
    flags       = frame[2];
    src         = frame[3];
    dst         = frame[4];
    cmd         = frame[5];
    seq         = frame[6];
    payload_len = frame[7];

    crc_rx = (uint16_t)(((uint16_t)frame[len - 2U] << 8) | frame[len - 1U]);
    crc_calc = srm_crc16_ccitt_false(&frame[1], (size_t)(7U + payload_len));

    debug_console_write_line("SRM RX FRAME:");
    rs485_print_frame_hex(frame, len);

    snprintf(msg, sizeof(msg), "VER=0x%02X FLAGS=0x%02X SRC=0x%02X DST=0x%02X",
             ver, flags, src, dst);
    debug_console_write_line(msg);

    snprintf(msg, sizeof(msg), "CMD=0x%02X SEQ=0x%02X LEN=0x%02X",
             cmd, seq, payload_len);
    debug_console_write_line(msg);

    snprintf(msg, sizeof(msg), "CRC RX=0x%04X CRC CALC=0x%04X",
             crc_rx, crc_calc);
    debug_console_write_line(msg);

    if (crc_rx == crc_calc)
    {
        debug_console_write_line("SRM RX CRC: OK");
        rs485_dispatch_srm_frame(frame, len);
    }
    else
    {
        debug_console_write_line("SRM RX CRC: FAIL");
    }
}

bool rs485_if_is_de_configured(void)
{
#if defined(RS485_DE_Pin) && defined(RS485_DE_GPIO_Port)
    return true;
#else
    return false;
#endif
}

void rs485_if_init(void)
{
    (void)uart_if_init(&s_rs485_uart, &huart1);
    uart_if_set_rx_callback(&s_rs485_uart, rs485_on_rx_byte);

    srm_rx_buffer_init(&s_rx_buffer);
    srm_rx_parser_init(&s_rx_parser);
    s_rx_overflow_count = 0U;

    rs485_set_rx_mode();

    if (uart_if_start_rx_it(&s_rs485_uart))
    {
        debug_console_write_line("USART1 RX interrupt started");
        debug_console_write_line("Waiting SRM frames on USART1...");
    }
    else
    {
        debug_console_write_line("ERR: USART1 RX interrupt start failed");
    }
}

void rs485_if_process_rx(void)
{
    uint8_t byte;
    char msg[80];

    while (srm_rx_buffer_pop(&s_rx_buffer, &byte))
    {
        if (srm_rx_parser_feed(&s_rx_parser, byte))
        {
            rs485_handle_srm_frame(s_rx_parser.frame, s_rx_parser.total_len);
            srm_rx_parser_reset(&s_rx_parser);
        }
    }

    if (s_rx_overflow_count > 0U)
    {
        snprintf(msg, sizeof(msg), "ERR: USART1 RX buffer overflow count = %lu",
                 (unsigned long)s_rx_overflow_count);
        debug_console_write_line(msg);
        s_rx_overflow_count = 0U;
    }
}

bool rs485_if_send_bytes(const uint8_t *data, uint16_t len)
{
    if ((data == NULL) || (len == 0U))
    {
        return false;
    }

    rs485_set_tx_mode();

    if (!uart_if_write(&s_rs485_uart, data, len, RS485_TX_TIMEOUT_MS))
    {
        rs485_set_rx_mode();
        return false;
    }

    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
    {
        /* wait for transmission complete */
    }

    rs485_set_rx_mode();
    return true;
}

bool rs485_if_send_string(const char *str)
{
    if (str == NULL)
    {
        return false;
    }

    return rs485_if_send_bytes((const uint8_t *)str, (uint16_t)strlen(str));
}

bool rs485_if_read_byte(uint8_t *byte)
{
    if (byte == NULL)
    {
        return false;
    }

    rs485_set_rx_mode();
    return uart_if_read_byte(&s_rs485_uart, byte);
}
