#include "stepper.h"

#include "main.h"
#include "stm32f2xx_hal.h"

typedef struct
{
    bool initialized;
    uint8_t phase_index;
    int32_t position_steps;
} stepper_ctx_t;

/*
 * FULL-STEP sequence for 28BYJ-48 through ULN2003
 *
 * Standard wiring:
 * PD4 -> IN1
 * PD5 -> IN2
 * PD6 -> IN3
 * PD7 -> IN4
 *
 * bit0 -> IN1
 * bit1 -> IN2
 * bit2 -> IN3
 * bit3 -> IN4
 */
static const uint8_t s_fullstep_seq[4] =
{
    0x03U,
    0x06U,
    0x0CU,
    0x09U
};

static stepper_ctx_t s_stepper = {0};

static void stepper_apply_phase(uint8_t phase_mask);

bool stepper_init(void)
{
    s_stepper.initialized = true;
    s_stepper.phase_index = 0U;
    s_stepper.position_steps = 0;

    stepper_release();
    return true;
}

void stepper_release(void)
{
    HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);
}

bool stepper_step_cw(void)
{
    if (!s_stepper.initialized)
    {
        return false;
    }

    s_stepper.phase_index++;
    if (s_stepper.phase_index >= 4U)
    {
        s_stepper.phase_index = 0U;
    }

    s_stepper.position_steps++;

    stepper_apply_phase(s_fullstep_seq[s_stepper.phase_index]);
    return true;
}

bool stepper_step_ccw(void)
{
    if (!s_stepper.initialized)
    {
        return false;
    }

    if (s_stepper.phase_index == 0U)
    {
        s_stepper.phase_index = 3U;
    }
    else
    {
        s_stepper.phase_index--;
    }

    s_stepper.position_steps--;

    stepper_apply_phase(s_fullstep_seq[s_stepper.phase_index]);
    return true;
}

bool stepper_rotate_cw(uint16_t steps, uint32_t step_delay_ms)
{
    uint16_t i;

    if (!s_stepper.initialized)
    {
        return false;
    }

    for (i = 0U; i < steps; i++)
    {
        if (!stepper_step_cw())
        {
            return false;
        }

        HAL_Delay(step_delay_ms);
    }

    return true;
}

bool stepper_rotate_ccw(uint16_t steps, uint32_t step_delay_ms)
{
    uint16_t i;

    if (!s_stepper.initialized)
    {
        return false;
    }

    for (i = 0U; i < steps; i++)
    {
        if (!stepper_step_ccw())
        {
            return false;
        }

        HAL_Delay(step_delay_ms);
    }

    return true;
}

int32_t stepper_get_position(void)
{
    return s_stepper.position_steps;
}

void stepper_reset_position(void)
{
    s_stepper.position_steps = 0;
}

bool stepper_go_home(uint32_t step_delay_ms)
{
    int32_t current_position;
    uint32_t steps_to_move;
    uint32_t i;

    current_position = s_stepper.position_steps;

    if (current_position == 0)
    {
        return true;
    }

    if (current_position > 0)
    {
        steps_to_move = (uint32_t)current_position;

        for (i = 0U; i < steps_to_move; i++)
        {
            if (!stepper_step_ccw())
            {
                return false;
            }

            HAL_Delay(step_delay_ms);
        }
    }
    else
    {
        steps_to_move = (uint32_t)(-current_position);

        for (i = 0U; i < steps_to_move; i++)
        {
            if (!stepper_step_cw())
            {
                return false;
            }

            HAL_Delay(step_delay_ms);
        }
    }

    s_stepper.position_steps = 0;
    return true;
}

static void stepper_apply_phase(uint8_t phase_mask)
{
    HAL_GPIO_WritePin(STEP_IN1_GPIO_Port,
                      STEP_IN1_Pin,
                      ((phase_mask & 0x01U) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(STEP_IN2_GPIO_Port,
                      STEP_IN2_Pin,
                      ((phase_mask & 0x02U) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(STEP_IN3_GPIO_Port,
                      STEP_IN3_Pin,
                      ((phase_mask & 0x04U) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(STEP_IN4_GPIO_Port,
                      STEP_IN4_Pin,
                      ((phase_mask & 0x08U) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
