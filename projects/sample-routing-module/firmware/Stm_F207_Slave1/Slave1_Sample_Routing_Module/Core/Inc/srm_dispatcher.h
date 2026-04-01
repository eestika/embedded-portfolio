#ifndef SRM_DISPATCHER_H
#define SRM_DISPATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void srm_dispatcher_init(void);
void srm_dispatcher_handle_frame(const uint8_t *frame, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* SRM_DISPATCHER_H */
