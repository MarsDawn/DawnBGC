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

customer_operation_struct customer_operation =
    {
        .CtrlMode.Pitch = ATTITUDE_MODE,
        .CtrlMode.Roll = ATTITUDE_MODE,
        .CtrlMode.Yaw = ANGLE_MODE,
        .GeoEulerTraget =
            {
                .Order = EULER_ORDER_ZXY,
                .Pitch = 0.0f,
                .Roll = 0.0f,
                .Yaw = 0.0f,
            },
};

void gimbal_business(void)
{

    if (GimbalSystem.GimbalState == SYSTEM_STATUS)
    {
        motor_stall_process();
    }
}
void motor_stall_process(void)
{
    static uint32_t StallTime = 0;
    static uint8_t StallTimes = 0;

    
    if (
        (GimbalSystem.ArmTorqueRef.Data.Latge) < (GimbalSystem.ArmRadsPID.Data.Latge.OutLimitMin + 100.f) || (GimbalSystem.ArmTorqueRef.Data.Latge) > (GimbalSystem.ArmRadsPID.Data.Latge.OutLimitMax - 100.0f) || (GimbalSystem.ArmTorqueRef.Data.Middle) < (GimbalSystem.ArmRadsPID.Data.Middle.OutLimitMin + 100.0f) || (GimbalSystem.ArmTorqueRef.Data.Middle) > (GimbalSystem.ArmRadsPID.Data.Middle.OutLimitMax - 100.0f) || (GimbalSystem.ArmTorqueRef.Data.Small) < (GimbalSystem.ArmRadsPID.Data.Small.OutLimitMin + 100.0f) || (GimbalSystem.ArmTorqueRef.Data.Small) > (GimbalSystem.ArmRadsPID.Data.Small.OutLimitMax - 100.0f))
    {
        StallTime++;
        if (StallTime > (5.0f * CLOSELOOP_EXECUTE_FREQ))
        {
            
            StallTime = 0;

            StallTimes++;

            ShutOffPWM(&BLDCYaw);
            ShutOffPWM(&BLDCRoll);
            ShutOffPWM(&BLDCPitch);
        }
    }
    else
        StallTime = 0;
}
