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
TimeBaseTimeOut_struct TimeOutArray[TOTAL_TIMEOUT_MODULE];

void TimeBaseInit(void)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER13);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    
    timer_deinit(TIMER13);
    timer_initpara.prescaler = TIMEBASE_PRSC;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = TIMEBASE_PERIOD - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER13, &timer_initpara);
    
    nvic_irq_enable(TIMER7_TRG_CMT_TIMER13_IRQn, TIMEBASE_PRE_PRIORITY, TIMEBASE_SUB_PRIORITY);
    timer_interrupt_enable(TIMER13, TIMER_INT_UP);
    timer_counter_value_config(TIMER13 , 0);
    timer_enable(TIMER13);
    
    TimeOutArray[FREE_TIMEOUT_MODULE].en = TRUE;
}

void TimeBaseStart(uint16_t period, TimeBaseTimeOut_struct *time_out)
{
    time_out->cnt = 0;
    time_out->period = period;
    time_out->en = TRUE;
}

void TimeBaseStop(TimeBaseTimeOut_struct *time_out)
{
    time_out->en = FALSE;    
    time_out->cnt = 0;
}

uint8_t TimeBaseTimeOut(TimeBaseTimeOut_struct *time_out)
{
    if((time_out->cnt) > (time_out->period))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void TIMER7_TRG_CMT_TIMER13_IRQHandler(void)
{
    static uint8_t isr_cnt = 0;
    uint8_t i = 0;
    
    if(timer_interrupt_flag_get(TIMER13, TIMER_INT_UP))
    {
        timer_interrupt_flag_clear(TIMER13, TIMER_INT_UP);

        isr_cnt++;
        if(isr_cnt > ((TIMEBASE_FREQ / 1000) - 1))
        {
            isr_cnt = 0;
            for(i = 0; i < TOTAL_TIMEOUT_MODULE; i++)
            {
                if(TimeOutArray[i].en)
                {
                    TimeOutArray[i].cnt++;
                }
            }
        }
    }
}
