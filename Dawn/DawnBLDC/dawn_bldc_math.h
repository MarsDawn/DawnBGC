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


#ifndef __DAWN_BLDC_MATH_H
#define __DAWN_BLDC_MATH_H

#include "gd32f4xx.h"
typedef struct
{
    float X;
    float Y;
}HallStruct;
typedef struct
{
    float A;
    float B;
}ParkStruct;
typedef struct
{
    float Direct;
    float Quad;
}QuadDirectStruct;
ParkStruct BLDCClarke(HallStruct HallStruct);
QuadDirectStruct BLDCPark(ParkStruct ABIn, float Rad);
ParkStruct BLDCRevPark(QuadDirectStruct QuadDirect, float Rad);
#endif
