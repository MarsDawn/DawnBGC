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

void SysLedInit(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_12);

    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_12);

    gpio_bit_set(GPIOC, GPIO_PIN_12);
}

void SysLedCmd(ControlStatus cmd)
{
    if (cmd == DISABLE)
    {
        gpio_bit_set(GPIOC, GPIO_PIN_12);
    }
    else
    {
        gpio_bit_reset(GPIOC, GPIO_PIN_12);
    }
}

int main(void)
{

    ControlStatus SysLedStatus = DISABLE;

    
    __set_PRIMASK(1);

    
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x20000);

    
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    
    SystickInit();

    
    SysLedInit();
    
    TimeBaseInit();

    
    DebugUsartInit(115200);

    
    Icm206XXInit();

    
    GimbalInit();

    
    

    
    __set_PRIMASK(0);

    TimeBaseStart(1000 / SYS_LED_TOGGLE_FREQ, &TimeOutArray[FREE_TIMEOUT_MODULE]);

    while (1)
    {
        
        FrameUnpack(&DebugUsartFrame);

        
        if (TimeBaseTimeOut(&TimeOutArray[FREE_TIMEOUT_MODULE]))
        {
            
            TimeBaseStart(1000 / SYS_LED_TOGGLE_FREQ, &TimeOutArray[FREE_TIMEOUT_MODULE]);
            SysLedStatus = (ControlStatus)!SysLedStatus;
            SysLedCmd(SysLedStatus);
        }

        
        
    }
}
