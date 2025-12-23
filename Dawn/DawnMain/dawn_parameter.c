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

GimbalSystemStruct GimbalSystem =
    {
        
        .GimbalState = IMU_STATUS,
        .CtrlMode.Pitch = ATTITUDE_MODE,
        .CtrlMode.Roll = ATTITUDE_MODE,
        .CtrlMode.Yaw = ANGLE_MODE,
        .RestartFlag = TRUE,
        .JacbInv = TRUE,
        .PositionLoopFlag = FALSE,

        
        .PlaneAttitudePID =
            {
                .PlaneKpValue = 0.4f,
                .PlaneKiValue = 0.00001f,
                .AccThresholdCoeA = 0.05f,
                .AccThresholdCoeB = 0.15f,
            },
        .ArmAngleCompPID =
            {
                .ArmKp = 50.0f,   
                .ArmKi = 0.0001f, 
                .FilterCoefLarge = 0.98f,
                .FilterCoefMiddle = 0.98f,
                .FilterCoefSmall = 0.98f,
            },

        
        .GeoEulerACC.Order = EULER_ORDER_ZXY,
        .GeoEulerQuat.Order = EULER_ORDER_ZXY,
        .GeoEulerTraget.Order = EULER_ORDER_ZXY,
        .GeoEulerRefOffset.Order = EULER_ORDER_ZXY,
        .GeoEulerBase.Order = EULER_ORDER_ZXY,

        
        .QuaternionPlane =
            {
                .q0 = 1.0f,
                .q1 = 0.0f,
                .q2 = 0.0f,
                .q3 = 0.0f,
            },

        
        .ArmDir.Data =
            {
                
                .Latge = 1.0f,
                .Middle = 1.0f,
                .Small = -1.0f,
            },

        
        .ArmAngleOffset.Data =
            {
                .Latge = 0.28f,
                .Middle = 0.0f,
                .Small = 0.0f,
            },

        
        .ArmAngleRefMin.Data =
            {
                .Latge = -1.25f, 
                .Middle = -0.63f,
                .Small = -0.88f, 
            },
        .ArmAngleRefMax.Data =
            {
                .Latge = 1.25f, 
                .Middle = 0.63f,
                .Small = 1.70f, 
            },

        
        .ArmRadsRefACCMax.Data =
            {
                .Latge = 100.0f,
                .Middle = 100.0f,
                .Small = 100.0f,
            },

        .ArmAnglePIDKpAttitude.Data =
            {
                .Latge = 100.0f,
                .Middle = 100.0f,
                .Small = 100.0f,
            },

        .ArmAnglePIDKpAngle.Data =
            {
                .Latge = 3.0f,
                .Middle = 3.0f,
                .Small = 10.0f,
            },
        
        
        
        
        
        
        
        .ArmAnglePID.Data.Latge =
            {
                .Kp = 200.0f,
                .Ki = 0.0f,
                .Integral = 0.0f,
                .IntegralMin = -5.0f,
                .IntegralMax = 5.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -5.0f,
                .OutLimitMax = 5.0f,
            },
        .ArmAnglePID.Data.Middle =
            {
                .Kp = 10.0f,
                .Ki = 0.0f,
                .Integral = 0.0f,
                .IntegralMin = -5.0f,
                .IntegralMax = 5.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -5.0f,
                .OutLimitMax = 5.0f,
            },
        .ArmAnglePID.Data.Small =
            {
                .Kp = 0.5f,
                .Ki = 0.0001f,
                .Integral = 0.0f,
                .IntegralMin = -5.0f,
                .IntegralMax = 5.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -5.0f,
                .OutLimitMax = 5.0f,
            },

        
        .ArmRadsPID.Data.Latge =
            {
                .Kp = 4000.0f,
                .Ki = 200.0f,
                .Integral = 0.0f,
                .IntegralMin = -12000.0f,
                .IntegralMax = 12000.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -12000.0f,
                .OutLimitMax = 12000.0f,
            },
        .ArmRadsPID.Data.Middle =
            {
                .Kp = 3000.0f,
                .Ki = 100.0f,
                .Integral = 0.0f,
                .IntegralMin = -25000.0f,
                .IntegralMax = 15000.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -25000.0f,
                .OutLimitMax = 15000.0f,
            },
        .ArmRadsPID.Data.Small =
            {
                .Kp = 3500.0f,
                .Ki = 100.0f,
                .Integral = 0.0f,
                .IntegralMin = -15000.0f,
                .IntegralMax = 15000.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -15000.0f,
                .OutLimitMax = 15000.0f,
            },
            
        .BLDCAnglePID.Data =
        {
            .Latge = 0.0f,
            .Middle = 1.5f,
            .Small = 10.0f,
        },
        
        .BLDCRadsPID.Data.Latge =
            {
                .Kp = 8000.0f,
                .Ki = 10.0f,
                .Integral = 0.0f,
                .IntegralMin = -12000.0f,
                .IntegralMax = 2000.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -12000.0f,
                .OutLimitMax = 2000.0f,
            },
        .BLDCRadsPID.Data.Middle =
            {
                .Kp = 4000.0f,
                .Ki = 20.0f,
                .Integral = 0.0f,
                .IntegralMin = -17000.0f,
                .IntegralMax = 2000.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -17000.0f,
                .OutLimitMax = 2000.0f,
            },
        .BLDCRadsPID.Data.Small =
            {
                .Kp = 16500.0f,
                .Ki = 150.0f,
                .Integral = 0.0f,
                .IntegralMin = -7000.0f,
                .IntegralMax = 7000.0f,
                .Kd = 0.0f,
                .OldError = 0.0f,
                .FliterCoef = 0.0f,
                .OutLimitMin = -7000.0f,
                .OutLimitMax = 7000.0f,
            },
        
};



