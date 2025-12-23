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

static uint8_t  fac_us=0; 
static uint16_t fac_ms=0; 

void SystickInit(void)
{
    uint8_t sysclk_mhz = 0;
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);

    
    sysclk_mhz = SYSTERM_CLK / 1000000;
    fac_us = sysclk_mhz / 8;
    fac_ms = (uint16_t)fac_us * 1000;
}

void systick_dly_us(uint32_t nus)
{
    uint32_t temp;

    
    SysTick->LOAD = nus * fac_us;
    
    SysTick->VAL = 0x00;
    
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp & 0x01)&& !(temp & (1<<16)));
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    
    SysTick->VAL = 0x00;
}

void systick_dly_ms(uint16_t nms)
{
    uint32_t temp;

    
    SysTick->LOAD=(uint32_t)nms * fac_ms;
    
    SysTick->VAL = 0x00;
    
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01)&& !(temp & (1<<16)));
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    
    SysTick->VAL = 0x00;
}
