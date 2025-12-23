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


#ifndef __DAWN_FUNCTION_H
#define __DAWN_FUNCTION_H

#include "gd32f4xx.h"
#include "dawn_gimbal_system.h"
#include "dawn_gimbal_math.h"
typedef struct
{
    EulerStruct CtrlMode;
    EulerStruct GeoEulerTraget;            
    uint8_t motor_hall_cali_flag;         
    uint8_t gimbal_zero_mech_flag;        
    uint8_t w_mode_flag;
} customer_operation_struct;

extern customer_operation_struct customer_operation;

void gimbal_business(void);
void motor_stall_process(void);

#endif
