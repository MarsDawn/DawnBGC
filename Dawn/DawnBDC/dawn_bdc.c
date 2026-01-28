/*
 * DawnBGC - Open Source Brushless Gimbal Controller
 *
 * Copyright (c) 2025 MarsDawn
 *
 * Project repository:
 *     https://github.com/MarsDawn
 *
 * This file is part of the DawnBGC project.
 *
 * Redistribution and use in source or binary forms, with or without
 * modification, are permitted under the MIT License, provided that
 * this copyright notice and this permission notice are retained.
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at:
 *
 *     https://opensource.org/licenses/MIT
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 */
#include "includes.h"

void BDCInit(void)
{
    BDCGpioInit();

    BDC_TurnOnPWM(&BLDCYaw);
    BDC_TurnOnPWM(&BLDCRoll);
    BDC_TurnOnPWM(&BLDCPitch);
}

static inline uint16_t BDC_MapMagToCcr(int16_t mag, uint16_t arr)
{
    if (mag < 0)
        mag = (int16_t)(-mag);

    if (mag > PWM_MAX_LIMIT)
        mag = PWM_MAX_LIMIT;

    return (uint16_t)(((int32_t)mag * (int32_t)arr) / (int32_t)PWM_MAX_LIMIT);
}

static inline void BDC_ApplySlowDecayDualPwm(int16_t u, uint16_t arr,
                                             volatile uint32_t *in1_ccr,
                                             volatile uint32_t *in2_ccr)
{
    uint16_t duty = BDC_MapMagToCcr(u -= PWM_MAX_LIMIT, arr);

    uint16_t full = arr;

    if (u >= 0)
    {
        *in1_ccr = full;
        *in2_ccr = duty;
    }
    else
    {
        *in1_ccr = duty;
        *in2_ccr = full;
    }
}

void BDCpwm(BLDCStruct *BLDCStruct)
{
    static int16_t u;
    u = (int16_t)(BLDCStruct->QuadOut);

    switch (BLDCStruct->BLDCId)
    {
    case BLDC_ID_YAW:
    {
        volatile uint32_t *in1 = (volatile uint32_t *)&TIMER_CH2CV(TIMER1);
        volatile uint32_t *in2 = (volatile uint32_t *)&TIMER_CH3CV(TIMER1);
        BDC_ApplySlowDecayDualPwm(u, (uint16_t)TIM1_ARR, in1, in2);
    }
    break;

    case BLDC_ID_ROLL:
    {
        volatile uint32_t *in1 = (volatile uint32_t *)&TIMER_CH0CV(TIMER0);
        volatile uint32_t *in2 = (volatile uint32_t *)&TIMER_CH3CV(TIMER0);
        BDC_ApplySlowDecayDualPwm(u, (uint16_t)TIM0_ARR, in1, in2);
    }
    break;

    case BLDC_ID_PITCH:
    {
        volatile uint32_t *in1 = (volatile uint32_t *)&TIMER_CH0CV(TIMER1);
        volatile uint32_t *in2 = (volatile uint32_t *)&TIMER_CH1CV(TIMER1);
        BDC_ApplySlowDecayDualPwm(u, (uint16_t)TIM1_ARR, in1, in2);
    }
    break;

    default:
        break;
    }
}

void BDC_TurnOnPWM(BLDCStruct *BLDCStruct)
{
    switch (BLDCStruct->BLDCId)
    {
    case BLDC_ID_YAW:
        TIMER_CH0CV(TIMER0) = 0;
        TIMER_CH1CV(TIMER0) = 0;

        timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_ENABLE);
        timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_ENABLE);
        break;

    case BLDC_ID_ROLL:
        TIMER_CH2CV(TIMER0) = 0;
        TIMER_CH3CV(TIMER0) = 0;

        timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_ENABLE);
        timer_channel_output_state_config(TIMER0, TIMER_CH_3, TIMER_CCX_ENABLE);
        break;

    case BLDC_ID_PITCH:
        TIMER_CH0CV(TIMER1) = 0;
        TIMER_CH1CV(TIMER1) = 0;

        timer_channel_output_state_config(TIMER1, TIMER_CH_0, TIMER_CCX_ENABLE);
        timer_channel_output_state_config(TIMER1, TIMER_CH_1, TIMER_CCX_ENABLE);
        break;

    default:
        break;
    }
}

void BDC_ShutOffPWM(BLDCStruct *BLDCStruct)
{
    switch (BLDCStruct->BLDCId)
    {
    case BLDC_ID_YAW:
        TIMER_CH0CV(TIMER0) = 0;
        TIMER_CH1CV(TIMER0) = 0;

        timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_DISABLE);
        timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_DISABLE);
        break;

    case BLDC_ID_ROLL:
        TIMER_CH2CV(TIMER0) = 0;
        TIMER_CH3CV(TIMER0) = 0;

        timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_DISABLE);
        timer_channel_output_state_config(TIMER0, TIMER_CH_3, TIMER_CCX_DISABLE);
        break;

    case BLDC_ID_PITCH:
        TIMER_CH0CV(TIMER1) = 0;
        TIMER_CH1CV(TIMER1) = 0;

        timer_channel_output_state_config(TIMER1, TIMER_CH_0, TIMER_CCX_DISABLE);
        timer_channel_output_state_config(TIMER1, TIMER_CH_1, TIMER_CCX_DISABLE);
        break;

    default:
        break;
    }
}
