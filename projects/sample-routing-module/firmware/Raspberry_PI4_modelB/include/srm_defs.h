#ifndef SRM_DEFS_H
#define SRM_DEFS_H

#define SRM_SOF               0xAAU
#define SRM_VERSION           0x01U

#define SRM_HEADER_SIZE       8U
#define SRM_CRC_SIZE          2U
#define SRM_MAX_PAYLOAD_LEN   64U
#define SRM_MAX_FRAME_SIZE    (SRM_HEADER_SIZE + SRM_MAX_PAYLOAD_LEN + SRM_CRC_SIZE)

#define SRM_CMD_PING_REQ      0x01U
#define SRM_CMD_PING_RSP      0x02U

#define SRM_CMD_LED_ON_REQ    0x10U
#define SRM_CMD_LED_ON_RSP    0x11U

#define SRM_CMD_LED_OFF_REQ   0x12U
#define SRM_CMD_LED_OFF_RSP   0x13U

#define SRM_CMD_STATUS_REQ    0x20U
#define SRM_CMD_STATUS_RSP    0x21U

#define SRM_CMD_LCD_CLEAR_REQ 0x30U
#define SRM_CMD_LCD_CLEAR_RSP 0x31U

#define SRM_CMD_LCD_WRITE_REQ 0x32U
#define SRM_CMD_LCD_WRITE_RSP 0x33U

#define SRM_NODE_ID_SLAVE1    0x01U
#define SRM_NODE_ID_MASTER    0x10U

#define SRM_LED_STATE_OFF     0x00U
#define SRM_LED_STATE_ON      0x01U

#define SRM_LCD_MAX_TEXT_LEN  16U

#endif /* SRM_DEFS_H */