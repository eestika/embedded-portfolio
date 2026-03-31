#ifndef SRM_RX_HANDLER_H
#define SRM_RX_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

void srm_rx_handler_init(void);
void srm_rx_handler_on_byte(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* SRM_RX_HANDLER_H */
