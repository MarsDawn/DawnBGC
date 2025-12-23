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


#ifndef __GPIO_H
#define __GPIO_H

#include "gd32f4xx.h"
#include "dawn_bldc_math.h"
#define PWM_FREQ                                 ((uint16_t)40000)   
#define HALF_PWM_PERIOD                          ((uint16_t)(SYSTERM_CLK * 0.5f / (uint32_t)(2 * PWM_FREQ)))
#define MAX_MODULATION_PERCENT                   ((uint8_t)100)
#define FAULT_PRE_PRIORITY                       ((uint8_t)0)
#define FAULT_SUB_PRIORITY                       ((uint8_t)0)

extern uint32_t AdcDate [5];

void BLDCGpioInit(void);
#endif

