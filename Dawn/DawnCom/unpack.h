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


#ifndef __UNPACK_H
#define __UNPACK_H

#include "gd32f4xx.h"

#define UnpackBufF_SIZE ((uint16_t)256)
#define UPDATA_FLAG 0x2FD
typedef enum
{
    DEBUG_USART_SRC = 0,
} PeriphSrcEnum;

typedef enum
{
    DEBUG_USART_DST = 0,
} PeriphDstEnum;

typedef enum
{
    SOF_DETECTED = 0,
    HEADER_DETECTED,
    DATA_RECEIVED,
} UnpackStepEnum;
typedef struct
{
    PeriphSrcEnum PeriphSrc;
    uint16_t UnpackIdex;
    uint8_t UnpackBuf[UnpackBufF_SIZE];
    uint16_t UnpackLen;
    uint8_t UnpackVer;
    UnpackStepEnum UnpackStep;
} FrameStruct;
extern FrameStruct DebugUsartFrame;

void FrameUnpack(FrameStruct *f);
#endif
