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
#ifndef __DAWN_BDC_H
#define __DAWN_BDC_H

#include "gd32f4xx.h"

#define PWM_MAX_LIMIT   ((int16_t)32767)

extern void BDCInit(void);
extern void BDC_TurnOnPWM(BLDCStruct *BLDCStruct);
extern void BDC_ShutOffPWM(BLDCStruct *BLDCStruct);
extern void BDCpwm(BLDCStruct *BLDCStruct);
#endif
