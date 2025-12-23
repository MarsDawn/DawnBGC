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


#ifndef __DWAN_GIMBAL_SYSTEM_H
#define __DWAN_GIMBAL_SYSTEM_H

#include "gd32f4xx.h"
#include "dawn_gimbal_math.h"

#define ANGLE_MODE ((bool)0)    
#define ATTITUDE_MODE ((bool)1) 
#define CLOSELOOP_EXECUTE_FREQ ((uint16_t)4000)
#define MAX_ANGLE_ERR ((float)0.26f)
typedef enum
{
    IMU_STATUS = 0, 
    BLDC_STATUS,    
    ATTITUDE_STATUS,                  
    SYSTEM_STATUS,                   
} GimbalStateEnum;
typedef struct
{
    float PlaneKpValue;
    float PlaneKiValue;
    float AccThresholdCoeA;
    float AccThresholdCoeB;
    float Kp;
    float Ki;
    float ErrX;
    float ErrY;
    float ErrZ;
    float IntegralX;
    float IntegralY;
    float IntegralZ;
    float OutX;
    float OutY;
    float OutZ;
} PlaneAttitudeFilterStruct;
typedef struct
{
    float ArmKp;
    float ArmKi;
    float FilterCoefLarge;
    float FilterCoefMiddle;
    float FilterCoefSmall;
} ArmAngleFilterStruct;
typedef union
{
    PidStruct Ele[3];
    struct
    {
        PidStruct Middle;
        PidStruct Latge;
        PidStruct Small;
    } Data;
} PIDArmUnion;
typedef struct
{

    GimbalStateEnum GimbalState; 
    
    
    
    EulerStruct CtrlMode;
    uint8_t RestartFlag;                        
    uint8_t JacbInv;                            
    float PlaneAccMod;                          
    uint8_t PositionLoopFlag;                   
    PlaneAttitudeFilterStruct PlaneAttitudePID; 
    ArmAngleFilterStruct ArmAngleCompPID;           

    EulerStruct GeoEulerACC;          
    EulerStruct GeoEulerQuat;         
    EulerStruct GeoEulerTraget;       
    EulerStruct GeoEulerRefOffset;    
    EulerStruct GeoEulerBase;         
    QuaternionStruct QuaternionPlane; 

    CartesianUnion GeoEulerErr;   
    CartesianUnion PlaneEulerErr; 
    CartesianUnion PlaneACCRaw;   
    CartesianUnion PlaneACC;      
    CartesianUnion PlaneACCBias;  

    CartesianUnion PlaneRadsRaw;  
    CartesianUnion PlaneRadsBias; 
    CartesianUnion PlaneRads;     
    CartesianUnion PlaneRadsCali; 

    ArmUnion ArmDir;              
    ArmUnion ArmAngleErr;         
    ArmUnion ArmAngle;         
    ArmUnion ArmAngleOffset;  
    ArmUnion ArmAngleEst;     
    ArmUnion ArmAngleRef;     
    ArmUnion ArmAngleRefMin; 
    ArmUnion ArmAngleRefMax; 
    ArmUnion ArmRadsEst;         
    ArmUnion ArmRadsRef;         
    ArmUnion ArmRadsRefACCMax; 
    ArmUnion ArmRadsRaw;         
    ArmUnion ArmRads;             
    ArmUnion ArmRadsBias;        
    ArmUnion ArmTorqueRef;    

    Matrix3X3Union BaseDCM;   
    Matrix3X3Union CameraDCM; 
    Matrix3X3Union ArmDCM;  
    Matrix3X3Union JacbState;  

    ArmUnion ArmAnglePIDKpAttitude;
    ArmUnion ArmAnglePIDKpAngle;
    
    ArmUnion BLDCAnglePID;

    PIDArmUnion ArmAnglePID; 
    PIDArmUnion ArmRadsPID;    
    PIDArmUnion BLDCRadsPID;     

} GimbalSystemStruct;

extern GimbalSystemStruct GimbalSystem;

void GimbalInit(void);
void GimbalWork(void);

#endif
