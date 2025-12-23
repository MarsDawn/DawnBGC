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

FrameStruct DebugUsartFrame =
    {
        .PeriphSrc = DEBUG_USART_SRC,
        .UnpackIdex = 0,
        .UnpackLen = 0,
        .UnpackVer = 0,
        .UnpackStep = SOF_DETECTED,
};

void FrameUnpack(FrameStruct *f)
{
    uint16_t i = 0, len = 0, len1 = 0, len2 = 0;
    ;
    uint8_t crc8 = 0;
    uint16_t crc16 = 0;
    static uint8_t pbuf[UnpackBufF_SIZE];

    switch (f->PeriphSrc)
    {
    case DEBUG_USART_SRC:
        len = debug_usart_get_byte(pbuf);
        break;

    default:
        break;
    }

    for (i = 0; i < len; i++)
    {

        len1 = 4 - (f->UnpackIdex);
        len2 = len - i;
        if (len2 >= len1)
        {

            memcpy(&f->UnpackBuf[f->UnpackIdex], &pbuf[i], len1);
            f->UnpackIdex += len1;
            i += len1 - 1;
        }
        else
        {

            memcpy(&f->UnpackBuf[f->UnpackIdex], &pbuf[i], len2);
            f->UnpackIdex += len2;
            i += len2 - 1;
        }

        if ((f->UnpackIdex) == 4)
        {
            if ((f->UnpackBuf[0] == 0xEB) && (f->UnpackBuf[1] == 0x90) && (f->UnpackBuf[2] == 0xA5))
            {
                f->UnpackStep = DATA_RECEIVED;
                usart_disable(USART2);
                usart_deinit(USART2);
                NVIC_SystemReset();
            }
        }
    }
}
