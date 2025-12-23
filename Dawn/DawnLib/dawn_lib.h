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


#ifndef __DAWB_LIB_H
#define __DAWM_LIB_H

#include "dawn_bldc_math.h"

typedef struct
{

    uint8_t BLDCId;
    uint8_t PoleDouble;

    int16_t HallAdcX; 
    int16_t HallAdcY;
    int16_t HallOffsetX; 
    int16_t HallOffsetY;
    int16_t HallAdcMinX; 
    int16_t HallAdcMaxX; 
    int16_t HallAdcMinY;
    int16_t HallAdcMaxY;
    float HallDir; 

    uint8_t StateCheck;
    HallStruct HallStruct;
    ParkStruct ParkStruct;
    ParkStruct VoltStrict;
    QuadDirectStruct QuadDirectStruct;

    uint8_t DynamicHoming; 
    uint8_t HomingMaxFlag;           
    float HomingCwQuad;                
    float HomingCCwQuad;
    int32_t StallTime;                
    uint32_t StallWaitTime;     
    float StallSpeed;          
    uint8_t StallFlag;               
    uint8_t BangCwFlag; 
    uint8_t BangCCwFlag;
    float BangHomingAddCOE; 
    float BangHomingCOE;     
    float ArmCwRad;   
    float ArmCCwRad;
    float ArmCwERad; 
    float ArmCCwERad;
    float HomingCwRad; 
    float HomingCcwRad;

    float DirectOut; 
    float QuadOut; 

    float BLDCRac;                
    float BLDCRacOffset;         
    float BLDCRacOffsetCoe; 
    float Speed;                          
    float ERadOffset;            
    float ERad;                   

    float CalcAnglesP;        
    float CalcAnglesI;        
    float CalcAnglesIntegral;  
    float CalcAnglesRadRes; 
    float CalcAnglesSpeedAccRes; 
} BLDCStruct;

extern BLDCStruct BLDCYaw;
extern BLDCStruct BLDCRoll;
extern BLDCStruct BLDCPitch;

extern void BLDCInit(void);
extern void BLDCCali(BLDCStruct *BLDCStruct);
extern void BLDCSvpwm(BLDCStruct *BLDCStruct);
extern void TurnOnPWM(BLDCStruct *BLDCStruct);
extern void ShutOffPWM(BLDCStruct *BLDCStruct);
extern void HallDataRead(BLDCStruct *BLDCStruct);

extern void BLDCEAngle(BLDCStruct *BLDCStruct);

void AttitudeCali(void);

#endif
