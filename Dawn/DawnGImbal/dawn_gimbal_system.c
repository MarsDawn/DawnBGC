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

static void BLDCUpdata(void);
static void IMUUpdata(void);
static uint8_t CheckBLDCStatus(void);
static void BLDCTorqueUpdata(void);
static float AngleRadsLimit(float AngleErr, float RadsRef, float MaxACC);
void GimbalInit(void)
{
    BLDCInit();
}

Matrix3X3Union local_ArmDCM_trans;

void GimbalWork(void)
{
    uint8_t i = 0, j = 0;
    

    
    BLDCUpdata();

    
    IMUUpdata();

    MatrixSub(3, 1, &GimbalSystem.PlaneRadsRaw.Ele[0], &GimbalSystem.PlaneRadsBias.Ele[0], &GimbalSystem.PlaneRads.Ele[0]);
    
    GimbalSystem.PlaneACC.Data.X = GimbalSystem.PlaneACCRaw.Data.X;
    GimbalSystem.PlaneACC.Data.Y = GimbalSystem.PlaneACCRaw.Data.Y;
    GimbalSystem.PlaneACC.Data.Z = GimbalSystem.PlaneACCRaw.Data.Z;

    
    AttitudeUpdata();

    
    ACC2Euler(&GimbalSystem.PlaneACC, &GimbalSystem.GeoEulerACC);
    AttitudeCali();

    
    MatrixMultiply(3, 3, 1, &GimbalSystem.JacbState.Ele[0], &GimbalSystem.PlaneRadsCali.Ele[0], &GimbalSystem.ArmRadsRaw.Ele[0]);

    
    
    

    
    switch (GimbalSystem.GimbalState)
    {
    case IMU_STATUS:
        if (((GimbalSystem.PlaneACCRaw.Data.X) != 0.0f) && ((GimbalSystem.PlaneRadsRaw.Data.X) != 0.0f))
        {
            GimbalSystem.GimbalState = BLDC_STATUS;
        }
        break;

    case BLDC_STATUS:
        if (CheckBLDCStatus())
        {
            GimbalSystem.GimbalState = ATTITUDE_STATUS;
        }
        break;

    case ATTITUDE_STATUS:
        if (GimbalSystem.RestartFlag)
        {
            GimbalSystem.RestartFlag = FALSE;

            
            ACC2Euler(&GimbalSystem.PlaneACC, &GimbalSystem.GeoEulerACC);
            Euler2Quat(&GimbalSystem.GeoEulerACC, &GimbalSystem.QuaternionPlane);
            GimbalSystem.PlaneAttitudePID.IntegralX = 0.0f;
            GimbalSystem.PlaneAttitudePID.IntegralZ = 0.0f;
            GimbalSystem.PlaneAttitudePID.IntegralY = 0.0f;

            GimbalSystem.GimbalState = SYSTEM_STATUS;
        }
        break;

    case SYSTEM_STATUS:
        
        GimbalSystem.GeoEulerErr.Data.Y = (GimbalSystem.GeoEulerTraget.Pitch) + (GimbalSystem.GeoEulerRefOffset.Pitch) - (GimbalSystem.GeoEulerQuat.Pitch);
        GimbalSystem.GeoEulerErr.Data.X = (GimbalSystem.GeoEulerTraget.Roll) + (GimbalSystem.GeoEulerRefOffset.Roll) - (GimbalSystem.GeoEulerQuat.Roll);

        for (i = 0; i < 3; i++)
        {
            if (GimbalSystem.GeoEulerErr.Ele[i] > MAX_ANGLE_ERR)
            {
                GimbalSystem.GeoEulerErr.Ele[i] = MAX_ANGLE_ERR;
            }
            else if (GimbalSystem.GeoEulerErr.Ele[i] < -MAX_ANGLE_ERR)
            {
                GimbalSystem.GeoEulerErr.Ele[i] = -MAX_ANGLE_ERR;
            }
        }

        
        MatrixMultiply(3, 3, 1, &GimbalSystem.CameraDCM.Ele[0], &GimbalSystem.GeoEulerErr.Ele[0], &GimbalSystem.PlaneEulerErr.Ele[0]);
        MatrixMultiply(3, 3, 1, &GimbalSystem.JacbState.Ele[0], &GimbalSystem.PlaneEulerErr.Ele[0], &GimbalSystem.ArmAngleErr.Ele[0]);

        
        if ((GimbalSystem.CtrlMode.Pitch) == ANGLE_MODE)
        {
            
            GimbalSystem.ArmAngleErr.Data.Small = (GimbalSystem.GeoEulerTraget.Pitch) + (GimbalSystem.GeoEulerRefOffset.Pitch) - (GimbalSystem.ArmAngle.Data.Small);
            GimbalSystem.ArmAnglePID.Data.Small.Kp = GimbalSystem.ArmAnglePIDKpAngle.Data.Small;
        }
        else if ((GimbalSystem.CtrlMode.Pitch) == ATTITUDE_MODE)
        {
            
            
            GimbalSystem.ArmAnglePID.Data.Small.Kp = GimbalSystem.ArmAnglePIDKpAttitude.Data.Small;
        }

        
        if ((GimbalSystem.CtrlMode.Roll) == ANGLE_MODE)
        {
            
            GimbalSystem.ArmAngleErr.Data.Middle = (GimbalSystem.GeoEulerTraget.Roll) + (GimbalSystem.GeoEulerRefOffset.Roll) - (GimbalSystem.ArmAngle.Data.Middle);
            GimbalSystem.ArmAnglePID.Data.Middle.Kp = GimbalSystem.ArmAnglePIDKpAngle.Data.Middle;
        }
        else if ((GimbalSystem.CtrlMode.Roll) == ATTITUDE_MODE)
        {
            
            
            GimbalSystem.ArmAnglePID.Data.Middle.Kp = GimbalSystem.ArmAnglePIDKpAttitude.Data.Middle;
        }

        
        if ((GimbalSystem.CtrlMode.Yaw) == ANGLE_MODE)
        {
            
            GimbalSystem.ArmAngleErr.Data.Latge = (GimbalSystem.GeoEulerTraget.Yaw) + (GimbalSystem.GeoEulerRefOffset.Yaw) - (GimbalSystem.ArmAngle.Data.Latge);
            
            

            
            GimbalSystem.ArmAnglePID.Data.Latge.Kp = GimbalSystem.ArmAnglePIDKpAngle.Data.Latge;
        }
        else if ((GimbalSystem.CtrlMode.Yaw) == ATTITUDE_MODE)
        {
            GimbalSystem.ArmAnglePID.Data.Latge.Kp = GimbalSystem.ArmAnglePIDKpAttitude.Data.Latge;
        }

        
        GimbalSystem.ArmRads.Data.Latge = GimbalSystem.ArmRadsRaw.Data.Latge + GimbalSystem.ArmRadsBias.Data.Latge;
        GimbalSystem.ArmRads.Data.Small = GimbalSystem.ArmRadsRaw.Data.Small;
        GimbalSystem.ArmRads.Data.Middle = GimbalSystem.ArmRadsRaw.Data.Middle;

        
        MatrixAdd(3, 1, &GimbalSystem.ArmAngle.Ele[0], &GimbalSystem.ArmAngleErr.Ele[0], &GimbalSystem.ArmAngleRef.Ele[0]);

        
        
        
        
        for (i = 0; i < 3; i++)
        {
            GimbalSystem.ArmRadsRef.Ele[i] = PidControl((GimbalSystem.ArmAngleRef.Ele[i]) - (GimbalSystem.ArmAngle.Ele[i]),
                                                   &GimbalSystem.ArmAnglePID.Ele[i]);

            
            GimbalSystem.ArmRadsRef.Ele[i] = AngleRadsLimit(GimbalSystem.ArmAngleErr.Ele[i],
                                                    GimbalSystem.ArmRadsRef.Ele[i],
                                                    GimbalSystem.ArmRadsRefACCMax.Ele[i]);
            
            
            
            
            
            
            
            
            
            
            
            GimbalSystem.ArmTorqueRef.Ele[i] = PidControl(GimbalSystem.ArmRadsRef.Ele[i] - GimbalSystem.ArmRads.Ele[i],
                                                        &GimbalSystem.ArmRadsPID.Ele[i]);
        }
        break;

    default:
        break;
    }

    
    BLDCTorqueUpdata();

    
    GimbalSystem.GeoEulerQuat.Yaw = 0.0f;
    Euler2DCM(&GimbalSystem.GeoEulerQuat, &GimbalSystem.CameraDCM);

    
    if (GimbalSystem.JacbInv)
    {
        CalcJacobianInv(&GimbalSystem.ArmAngle, &GimbalSystem.JacbState);
    }
    else
    {
        CalcJacobianTrans(&GimbalSystem.ArmAngle, &GimbalSystem.JacbState);
    }

    
    gimbal_business();
}

static void BLDCUpdata(void)
{
    uint8_t i = 0;

    GimbalSystem.ArmAngle.Data.Latge = (BLDCYaw.BLDCRac) * (GimbalSystem.ArmDir.Data.Latge);
    GimbalSystem.ArmAngle.Data.Middle = (BLDCRoll.BLDCRac) * (GimbalSystem.ArmDir.Data.Middle);
    GimbalSystem.ArmAngle.Data.Small = (BLDCPitch.BLDCRac) * (GimbalSystem.ArmDir.Data.Small);

    for (i = 0; i < 3; i++)
    {
        GimbalSystem.ArmAngle.Ele[i] -= GimbalSystem.ArmAngleOffset.Ele[i];
        
        if ((GimbalSystem.ArmAngle.Ele[i]) < -2.0f * PI)
        {
            GimbalSystem.ArmAngle.Ele[i] += 2.0f * PI;
        }
        else if ((GimbalSystem.ArmAngle.Ele[i]) > 2.0f * PI)
        {
            GimbalSystem.ArmAngle.Ele[i] -= 2.0f * PI;
        }
    }
}
static void IMUUpdata(void)
{
    GimbalSystem.PlaneRadsRaw.Data.X = -Icm206XXData.FGyroZ;
    GimbalSystem.PlaneRadsRaw.Data.Y = -Icm206XXData.FGyroY;
    GimbalSystem.PlaneRadsRaw.Data.Z = -Icm206XXData.FGyroX;

    GimbalSystem.PlaneACCRaw.Data.X = Icm206XXData.FAccZ;
    GimbalSystem.PlaneACCRaw.Data.Y = Icm206XXData.FAccY;
    GimbalSystem.PlaneACCRaw.Data.Z = Icm206XXData.FAccX;
}

static uint8_t CheckBLDCStatus(void)
{
    if ((BLDCYaw.StateCheck) && (BLDCRoll.StateCheck) && (BLDCPitch.StateCheck))
    {
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static void BLDCTorqueUpdata(void)
{

    BLDCYaw.QuadOut = (GimbalSystem.ArmTorqueRef.Data.Latge) * (GimbalSystem.ArmDir.Data.Latge);
    BLDCRoll.QuadOut = (GimbalSystem.ArmTorqueRef.Data.Middle) * (GimbalSystem.ArmDir.Data.Middle);
    BLDCPitch.QuadOut = (GimbalSystem.ArmTorqueRef.Data.Small) * (GimbalSystem.ArmDir.Data.Small);

    
    if (!BLDCYaw.StateCheck)
    {
        BLDCCali(&BLDCYaw);
    }
    else if (!BLDCRoll.StateCheck)
    {
        BLDCCali(&BLDCYaw);
        BLDCCali(&BLDCRoll);
    }
    else
    {
        BLDCCali(&BLDCYaw);
        BLDCCali(&BLDCRoll);
        BLDCCali(&BLDCPitch);
    }
}

static float AngleRadsLimit(float AngleErr, float RadsRef, float MaxACC)
{
    float MaxRads = 0.0f;

    if (AngleErr < 0.0f)
    {
        AngleErr = -AngleErr;
    }
    MaxRads = sqrtf(2.0f * AngleErr * MaxACC);

    if (RadsRef > MaxRads)
    {
        RadsRef = MaxRads;
    }
    else if (RadsRef < -MaxRads)
    {
        RadsRef = -MaxRads;
    }

    return RadsRef;
}
