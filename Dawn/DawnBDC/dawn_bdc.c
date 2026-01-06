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
    /* mag 取非负 */
    if (mag < 0)
        mag = (int16_t)(-mag);

    /* 限幅 */
    if (mag > PWM_MAX_LIMIT)
        mag = PWM_MAX_LIMIT;

    /* 线性映射：duty = mag/PWM_MAX_LIMIT * arr
     * 用 32 位防止溢出
     */
    return (uint16_t)(((int32_t)mag * (int32_t)arr) / (int32_t)PWM_MAX_LIMIT);
}

/* 设置某个“电机”的双 PWM（慢衰减）
 * u：有符号命令，正负代表方向，幅值代表占空比
 * arr：该定时器的 ARR
 * in1/in2：对应两个 CCR 的写入接口（用宏/寄存器传进来）
 */
static inline void BDC_ApplySlowDecayDualPwm(int16_t u, uint16_t arr,
                                             volatile uint32_t *in1_ccr,
                                             volatile uint32_t *in2_ccr)
{   // 慢衰减 指令要取
    uint16_t duty = BDC_MapMagToCcr(u-=PWM_MAX_LIMIT, arr);

    /* “100%”输出：PWM1 模式下用 CCR=ARR（几乎恒高） */
    uint16_t full = arr;

    if (u >= 0)
    {
        /* 正向：IN1=1, IN2=PWM */
        *in1_ccr = full;
        *in2_ccr = duty;
    }
    else
    {
        /* 反向：IN1=PWM, IN2=1 */
        *in1_ccr = duty;
        *in2_ccr = full;
    }
}

/* =========================
 * 1) “BDC版的 BLDCSvpwm”：按ID写入两路PWM
 * ========================= */
void BDCpwm(BLDCStruct *BLDCStruct)
{
    static int16_t u;
    u = (int16_t)(BLDCStruct->QuadOut);

    switch (BLDCStruct->BLDCId)
    {
    case BLDC_ID_YAW:
    {
        /* YAW：TIMER0_CH0 = IN1，TIMER0_CH1 = IN2 */
        volatile uint32_t *in1 = (volatile uint32_t *)&TIMER_CH0CV(TIMER0);
        volatile uint32_t *in2 = (volatile uint32_t *)&TIMER_CH1CV(TIMER0);
        BDC_ApplySlowDecayDualPwm(u, (uint16_t)TIM0_ARR, in1, in2);
    }
    break;

    case BLDC_ID_ROLL:
    {
        /* ROLL：TIMER0_CH2 = IN1，TIMER0_CH3 = IN2 */
        volatile uint32_t *in1 = (volatile uint32_t *)&TIMER_CH2CV(TIMER0);
        volatile uint32_t *in2 = (volatile uint32_t *)&TIMER_CH3CV(TIMER0);
        BDC_ApplySlowDecayDualPwm(u, (uint16_t)TIM0_ARR, in1, in2);
    }
    break;

    case BLDC_ID_PITCH:
    {
        /* PITCH：TIMER1_CH0(PB8) = IN1，TIMER1_CH1(PB9) = IN2 */
        volatile uint32_t *in1 = (volatile uint32_t *)&TIMER_CH0CV(TIMER1);
        volatile uint32_t *in2 = (volatile uint32_t *)&TIMER_CH1CV(TIMER1);
        BDC_ApplySlowDecayDualPwm(u, (uint16_t)TIM1_ARR, in1, in2);
    }
    break;

    default:
        break;
    }
}

/* =========================
 * 2) TurnOnPWM：上电后打开输出（两路都使能），并置为“安全态”
 * ========================= */
void BDC_TurnOnPWM(BLDCStruct *BLDCStruct)
{
    switch (BLDCStruct->BLDCId)
    {
    case BLDC_ID_YAW:
        /* 上电先置 0（滑行/coast），避免误转 */
        TIMER_CH0CV(TIMER0) = 0; /* IN1 */
        TIMER_CH1CV(TIMER0) = 0; /* IN2 */

        timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_ENABLE);
        timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_ENABLE);
        break;

    case BLDC_ID_ROLL:
        TIMER_CH2CV(TIMER0) = 0; /* IN1 */
        TIMER_CH3CV(TIMER0) = 0; /* IN2 */

        timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_ENABLE);
        timer_channel_output_state_config(TIMER0, TIMER_CH_3, TIMER_CCX_ENABLE);
        break;

    case BLDC_ID_PITCH:
        TIMER_CH0CV(TIMER1) = 0; /* IN1 */
        TIMER_CH1CV(TIMER1) = 0; /* IN2 */

        timer_channel_output_state_config(TIMER1, TIMER_CH_0, TIMER_CCX_ENABLE);
        timer_channel_output_state_config(TIMER1, TIMER_CH_1, TIMER_CCX_ENABLE);
        break;

    default:
        break;
    }
}

/* =========================
 * 3) ShutOffPWM：关闭输出（两路 disable），并可选置零
 * ========================= */
void BDC_ShutOffPWM(BLDCStruct *BLDCStruct)
{
    switch (BLDCStruct->BLDCId)
    {
    case BLDC_ID_YAW:
        /* 可选：先置 0 再 disable，避免最后一拍残留 */
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
