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


#ifndef __DAWN_BLDC_H
#define __DAWN_BLDC_H

#include "gd32f4xx.h"
#include "dawn_bldc_math.h"
#define BLDC_ID_PITCH ((uint8_t)0)
#define BLDC_ID_ROLL ((uint8_t)1)
#define BLDC_ID_YAW ((uint8_t)2)
#define HALL_ADC_VALUE_OFFSET_DEFAULT ((int16_t)1240)

#define SQRT_3                                   (1.732051f)
#define T                                        (HALF_PWM_PERIOD * 2)
#define T_SQRT3                                  ((uint16_t)(T * SQRT_3))
#define T_QUARTER                                ((uint16_t)(T / 4))
#define T_MAX_LIMIT                              ((int16_t)((int32_t)HALF_PWM_PERIOD * (int32_t)MAX_MODULATION_PERCENT / 100 ))    
#define SECTOR_1                                 ((uint8_t)1)
#define SECTOR_2                                 ((uint8_t)2)
#define SECTOR_3                                 ((uint8_t)3)
#define SECTOR_4                                 ((uint8_t)4)
#define SECTOR_5                                 ((uint8_t)5)
#define SECTOR_6                                 ((uint8_t)6)

extern BLDCStruct BLDCYaw;
extern BLDCStruct BLDCRoll;
extern BLDCStruct BLDCPitch;

void BLDCInit(void);
void BLDCCali(BLDCStruct *BLDCStruct);
void BLDCSvpwm(BLDCStruct *BLDCStruct);
void TurnOnPWM(BLDCStruct *BLDCStruct);
void ShutOffPWM(BLDCStruct *BLDCStruct);
void HallDataRead(BLDCStruct *BLDCStruct);
#endif
