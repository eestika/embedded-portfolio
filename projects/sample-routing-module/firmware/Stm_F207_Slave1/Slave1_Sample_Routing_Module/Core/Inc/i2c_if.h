#ifndef I2C_IF_H
#define I2C_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

typedef struct
{
    I2C_HandleTypeDef *hi2c;
} i2c_if_t;

bool i2c_if_init(i2c_if_t *iface, I2C_HandleTypeDef *hi2c);
bool i2c_if_write(i2c_if_t *iface, uint16_t dev_addr, const uint8_t *data, uint16_t len, uint32_t timeout);
bool i2c_if_read(i2c_if_t *iface, uint16_t dev_addr, uint8_t *data, uint16_t len, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* I2C_IF_H */
