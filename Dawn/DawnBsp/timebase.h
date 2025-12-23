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


#ifndef __TIMEBASE_H
#define __TIMEBASE_H

#include "gd32f4xx.h"

#define TIMEBASE_FREQ                            ((uint16_t)4000)      
#define TIMEBASE_PRSC                            ((uint8_t)4)
#define TIMEBASE_PERIOD                          ((uint16_t)(SYSTERM_CLK / (uint32_t)(TIMEBASE_FREQ *(TIMEBASE_PRSC + 1))))

#define TIMEBASE_PRE_PRIORITY                    ((uint8_t)3)
#define TIMEBASE_SUB_PRIORITY                    ((uint8_t)3)

#define TOTAL_TIMEOUT_MODULE                     ((uint8_t)3)
#define FREE_TIMEOUT_MODULE                      ((uint8_t)0)
#define LABVIEW_TIMEOUT_MODULE                   ((uint8_t)1)
#define GIMBAL_TIMEOUT_MODULE                    ((uint8_t)2)
typedef struct
{
    uint8_t en;
    uint16_t cnt;
    uint16_t period;
}TimeBaseTimeOut_struct;

extern TimeBaseTimeOut_struct TimeOutArray[TOTAL_TIMEOUT_MODULE];

void TimeBaseInit(void);
void TimeBaseStart(uint16_t period, TimeBaseTimeOut_struct *time_out);
void TimeBaseStop(TimeBaseTimeOut_struct *time_out);
uint8_t TimeBaseTimeOut(TimeBaseTimeOut_struct *time_out);
#endif
