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
ParkStruct BLDCClarke(HallStruct HallStruct)
{
    ParkStruct ABOut;

    ABOut.A = HallStruct.X;
    ABOut.B = ((HallStruct.X) + (HallStruct.Y) + (HallStruct.Y)) * 0.577f ;

    return ABOut;
}

QuadDirectStruct BLDCPark(ParkStruct ABIn, float Rad)
{
    QuadDirectStruct QuadDirectOut;

    QuadDirectOut.Direct = (ABIn.A) * cosf(Rad) + (ABIn.B) * sinf(Rad);
    QuadDirectOut.Quad = -(ABIn.A) * sinf(Rad) + (ABIn.B) * cosf(Rad);

    return QuadDirectOut;
}

ParkStruct BLDCRevPark(QuadDirectStruct QuadDirect, float Rad)
{
    ParkStruct ABOut;

    ABOut.A = (QuadDirect.Direct) * cosf(Rad) - (QuadDirect.Quad) * sinf(Rad);
    ABOut.B = (QuadDirect.Direct) * sinf(Rad) + (QuadDirect.Quad) * cosf(Rad);

    return ABOut;
}
