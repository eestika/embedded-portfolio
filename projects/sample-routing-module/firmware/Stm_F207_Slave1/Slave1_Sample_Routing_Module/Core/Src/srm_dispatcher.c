#include "srm_dispatcher.h"

#include "main.h"
#include "debug_console.h"
#include "rs485_if.h"
#include "srm_crc.h"
#include "srm_defs.h"
#include "lcd.h"
#include "lcd_ui.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static bool srm_dispatcher_send_frame(uint8_t flags,
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

static uint8_t srm_dispatcher_get_led_state(void)
{
    GPIO_PinState state = HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin);
    return (state == GPIO_PIN_SET) ? SRM_LED_STATE_ON : SRM_LED_STATE_OFF;
}

static void srm_dispatcher_build_status_payload(uint8_t *payload, uint8_t *payload_len)
{
    uint32_t uptime_ms;

    if ((payload == NULL) || (payload_len == NULL))
    {
        return;
    }

    uptime_ms = HAL_GetTick();

    payload[0] = srm_dispatcher_get_led_state();
    payload[1] = (uint8_t)((uptime_ms >> 24) & 0xFFU);
    payload[2] = (uint8_t)((uptime_ms >> 16) & 0xFFU);
    payload[3] = (uint8_t)((uptime_ms >> 8) & 0xFFU);
    payload[4] = (uint8_t)(uptime_ms & 0xFFU);

    *payload_len = 5U;
}

void srm_dispatcher_init(void)
{
    /* Reserved for future use */
}

void srm_dispatcher_handle_frame(const uint8_t *frame, uint16_t len)
{
    uint8_t src;
    uint8_t dst;
    uint8_t cmd;
    uint8_t seq;
    uint8_t payload_len;
    const uint8_t *payload;
    uint8_t status_payload[5];
    uint8_t status_len = 0U;

    (void)len;

    if (frame == NULL)
    {
        return;
    }

    src = frame[3];
    dst = frame[4];
    cmd = frame[5];
    seq = frame[6];
    payload_len = frame[7];
    payload = &frame[8];

    if (dst != SRM_NODE_ID_SLAVE1)
    {
        debug_console_write_line("SRM DISPATCH: frame not for this node");
        return;
    }

    switch (cmd)
    {
        case SRM_CMD_PING_REQ:
            debug_console_write_line("SRM DISPATCH: PING_REQ");

            (void)lcd_ui_show_temporary_message("PONG", 1200U);

            if (srm_dispatcher_send_frame(0U,
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

            (void)lcd_ui_show_temporary_message("LED ON", 1200U);

            if (srm_dispatcher_send_frame(0U,
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

            (void)lcd_ui_show_temporary_message("LED OFF", 1200U);

            if (srm_dispatcher_send_frame(0U,
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

        case SRM_CMD_STATUS_REQ:
            debug_console_write_line("SRM DISPATCH: STATUS_REQ");
            srm_dispatcher_build_status_payload(status_payload, &status_len);

            if (srm_dispatcher_send_frame(0U,
                                          SRM_NODE_ID_SLAVE1,
                                          src,
                                          SRM_CMD_STATUS_RSP,
                                          seq,
                                          status_payload,
                                          status_len))
            {
                debug_console_write_line("SRM TX: STATUS_RSP sent");
            }
            else
            {
                debug_console_write_line("SRM TX ERROR: STATUS_RSP failed");
            }
            break;

        case SRM_CMD_LCD_CLEAR_REQ:
            debug_console_write_line("SRM DISPATCH: LCD_CLEAR_REQ");

            (void)lcd_ui_show_ready();

            if (srm_dispatcher_send_frame(0U,
                                          SRM_NODE_ID_SLAVE1,
                                          src,
                                          SRM_CMD_LCD_CLEAR_RSP,
                                          seq,
                                          NULL,
                                          0U))
            {
                debug_console_write_line("SRM TX: LCD_CLEAR_RSP sent");
            }
            else
            {
                debug_console_write_line("SRM TX ERROR: LCD_CLEAR_RSP failed");
            }
            break;

        case SRM_CMD_LCD_WRITE_REQ:
        {
            uint8_t line_index;
            uint8_t text_len;
            char text_buf[17];
            bool write_ok;

            debug_console_write_line("SRM DISPATCH: LCD_WRITE_REQ");

            if ((payload == NULL) || (payload_len < 1U))
            {
                debug_console_write_line("SRM ERROR: LCD_WRITE_REQ invalid payload");
                break;
            }

            line_index = payload[0];
            text_len = (uint8_t)(payload_len - 1U);

            if (line_index > 1U)
            {
                debug_console_write_line("SRM ERROR: LCD_WRITE_REQ invalid line index");
                break;
            }

            if (text_len > 16U)
            {
                text_len = 16U;
            }

            memset(text_buf, 0, sizeof(text_buf));

            if (text_len > 0U)
            {
                memcpy(text_buf, &payload[1], text_len);
            }

            text_buf[text_len] = '\0';

            lcd_ui_enter_manual_mode();
            write_ok = lcd_write_line(line_index, text_buf);

            if (!write_ok)
            {
                debug_console_write_line("SRM ERROR: lcd_write_line failed");
            }

            if (srm_dispatcher_send_frame(0U,
                                          SRM_NODE_ID_SLAVE1,
                                          src,
                                          SRM_CMD_LCD_WRITE_RSP,
                                          seq,
                                          NULL,
                                          0U))
            {
                debug_console_write_line("SRM TX: LCD_WRITE_RSP sent");
            }
            else
            {
                debug_console_write_line("SRM TX ERROR: LCD_WRITE_RSP failed");
            }
            break;
        }

        default:
            debug_console_write_line("SRM DISPATCH: unsupported command");
            break;
    }
}

