#include "i2c_if.h"

bool i2c_if_init(i2c_if_t *iface, I2C_HandleTypeDef *hi2c)
{
    if ((iface == NULL) || (hi2c == NULL))
    {
        return false;
    }

    iface->hi2c = hi2c;
    return true;
}

bool i2c_if_write(i2c_if_t *iface, uint16_t dev_addr, const uint8_t *data, uint16_t len, uint32_t timeout)
{
    if ((iface == NULL) || (iface->hi2c == NULL) || (data == NULL) || (len == 0U))
    {
        return false;
    }

    return (HAL_I2C_Master_Transmit(iface->hi2c, dev_addr, (uint8_t *)data, len, timeout) == HAL_OK);
}

bool i2c_if_read(i2c_if_t *iface, uint16_t dev_addr, uint8_t *data, uint16_t len, uint32_t timeout)
{
    if ((iface == NULL) || (iface->hi2c == NULL) || (data == NULL) || (len == 0U))
    {
        return false;
    }

    return (HAL_I2C_Master_Receive(iface->hi2c, dev_addr, data, len, timeout) == HAL_OK);
}
