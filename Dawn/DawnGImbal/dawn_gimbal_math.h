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


#ifndef __DWAN_GIMBAL_MATH_H
#define __DWAN_GIMBAL_MATH_H

#include "gd32f4xx.h"
typedef union
{
    float Ele[3];
    struct
    {
        float X;
        float Y;
        float Z;
    }Data;
}CartesianUnion;
typedef union
{
    float Ele[3];
    struct
    {
        float Middle;
        float Latge;
        float Small;
    }Data;
}ArmUnion;
typedef enum
{
    EULER_ORDER_ZXY = 0,
    EULER_ORDER_ZYX,
}EulerOrderEnum;
typedef struct
{
    EulerOrderEnum Order;                      
    float Pitch;
    float Roll;
    float Yaw;
}EulerStruct;
typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
}QuaternionStruct;
typedef union
{
    float Ele[9];
    struct
    {
        float A00;
        float A01;
        float A02;
        float A10;
        float A11;
        float A12;
        float A20;
        float A21;
        float A22;
    }Data;
}Matrix3X3Union;
typedef struct
{
    float Kp;
    float Ki;
    float Integral;
    float IntegralMin;
    float IntegralMax;
    float OutLimitMin;
    float OutLimitMax;
    float Kd;
    float OldError;                   
    float FliterCoef;            
}PidStruct;
void PidSetKp(float Kp, PidStruct *Pid);
void PidSetKi(float Ki, PidStruct *Pid);
void PidSetIntegral(float Integral, PidStruct *Pid);
void PidSetIntegralLimit(float LimitMin, float LimitMax, PidStruct *Pid);
void PidSetKd(float Kd, PidStruct *Pid);
void PidSetKdFilterCoef(float FliterCoef, PidStruct *Pid);
void PidSetOldError(float OldError, PidStruct *Pid);
void PidSetOutLimit(float LimitMin, float LimitMax, PidStruct *Pid);
float PidControl(float LimitMax, PidStruct *Pid);
void ACC2Euler(CartesianUnion *a, EulerStruct *e);
void Euler2Quat(EulerStruct *e, QuaternionStruct *q);
void Euler2DCM(EulerStruct *e, Matrix3X3Union *DCM );

void Quat2Euler(QuaternionStruct *q, EulerStruct *e);
void QuatNormalize(QuaternionStruct *q, QuaternionStruct *QHat);

void CalcJacobianInv(ArmUnion *Link, Matrix3X3Union *j);
void CalcJacobianTrans(ArmUnion *Link, Matrix3X3Union *j);

void MatrixAdd(uint8_t ABRow, uint8_t ABCol, float *m_a, float *m_b, float *m_c);
void MatrixSub(uint8_t ABRow, uint8_t ABCol, float *m_a, float *m_b, float *m_c);
void MatrixMultiply(uint8_t ARow, uint8_t AColBRow, uint8_t BCol, float *m_a, float *m_b, float *m_c);

void AttitudeUpdata(void);

#endif

