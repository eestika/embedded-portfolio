#ifndef STEPPER_H
#define STEPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool stepper_init(void);
void stepper_release(void);

bool stepper_step_cw(void);
bool stepper_step_ccw(void);

bool stepper_rotate_cw(uint16_t steps, uint32_t step_delay_ms);
bool stepper_rotate_ccw(uint16_t steps, uint32_t step_delay_ms);

int32_t stepper_get_position(void);
void stepper_reset_position(void);
bool stepper_go_home(uint32_t step_delay_ms);

#ifdef __cplusplus
}
#endif

#endif /* STEPPER_H */
