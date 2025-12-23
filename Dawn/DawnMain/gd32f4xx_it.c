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


#include "gd32f4xx_it.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    
    while (1){
    }
}

void MemManage_Handler(void)
{
    
    while (1){
    }
}

void BusFault_Handler(void)
{
    
    while (1){
    }
}

void UsageFault_Handler(void)
{
    
    while (1){
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}
