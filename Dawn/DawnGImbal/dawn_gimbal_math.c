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

void ACC2Euler(CartesianUnion *a, EulerStruct *e)
{
    float NormDiv = 0.0f;
    float ax = 0.0f, ay = 0.0f, az = 0.0f;
    
    
    NormDiv = 1.0f / sqrtf((a->Data.X) * (a->Data.X) + (a->Data.Y) * (a->Data.Y) + (a->Data.Z) * (a->Data.Z));
    ax = (a->Data.X) * NormDiv;
    ay = (a->Data.Y) * NormDiv;
    az = (a->Data.Z) * NormDiv;
    
    
    switch(e->Order)
    {
        case EULER_ORDER_ZXY:
            
            if(ay > 0.9999f)
            {
                e->Roll = PI * 0.5f;
                e->Pitch = 0.0f;
            }
            else if(ay < -0.9999f)
            {
                e->Roll = -PI * 0.5f;
                e->Pitch = 0.0f;
            }
            else
            {
                e->Roll = asinf(ay);
                e->Pitch = atan2f(-ax, az);
            }
            break;
    }
    
    
    e->Yaw = 0.0f;
}

void Euler2Quat(EulerStruct *e, QuaternionStruct *q)
{
    float Pitch = 0.0f, Roll = 0.0f, Yaw = 0.0f;
    float sp = 0.0f, cp = 0.0f, sr = 0.0f, cr = 0.0f, sy = 0.0f, cy = 0.0f;

    Pitch = (e->Pitch) * 0.5f;
    Roll = (e->Roll) * 0.5f;
    Yaw = (e->Yaw) * 0.5f;
    
    sp = sinf(Pitch);
    cp = cosf(Pitch);
    sr = sinf(Roll);
    cr = cosf(Roll);
    sy = sinf(Yaw);
    cy = cosf(Yaw);
    
    switch(e->Order)
    {
        case EULER_ORDER_ZXY:
            q->q0 = cp * cr * cy - sp * sr * sy;
            q->q1 = cp * sr * cy - cr * sp * sy;
            q->q2 = cr * sp * cy + cp * sr * sy;
            q->q3 = cp * cr * sy + sp * sr * cy;
            break;
        
        default:
            break;
    }
}

void Euler2DCM(EulerStruct *e, Matrix3X3Union *DCM )
{
    float sp = 0.0f, cp = 0.0f, sr = 0.0f, cr = 0.0f, sy = 0.0f, cy = 0.0f;

    sp = sinf(e->Pitch);
    cp = cosf(e->Pitch);
    sr = sinf(e->Roll);
    cr = cosf(e->Roll);
    sy = sinf(e->Yaw);
    cy = cosf(e->Yaw);
    
    switch(e->Order)
    {
        case EULER_ORDER_ZXY:
            
            DCM ->Data.A00 = cp * cy - sp * sr * sy;
            DCM ->Data.A01 = cp * sy + cy * sp * sr;
            DCM ->Data.A02 = -cr * sp;
        
            
            DCM ->Data.A10 = -cr * sy;
            DCM ->Data.A11 = cr * cy;
            DCM ->Data.A12 = sr;
        
            
            DCM ->Data.A20 = cy * sp + cp * sr * sy;
            DCM ->Data.A21 = sp * sy - cp * cy * sr;
            DCM ->Data.A22 = cp * cr;
            break;
        
        default:
            break;
    }
}
        
void Quat2Euler(QuaternionStruct *q, EulerStruct *e)
{
    float A00 = 0.0f, A01 = 0.0f, A02 = 0.0f;
    float A10 = 0.0f, A11 = 0.0f, A12 = 0.0f;
    float A20 = 0.0f, A21 = 0.0f, A22 = 0.0f;
    
    switch(e->Order)
    {
        case EULER_ORDER_ZXY:
            
            A12 = 2.0f * ((q->q2) * (q->q3) + (q->q0) * (q->q1));
            if(A12 > 0.9999f)
            {
                A00 = 0.5f - ((q->q2) * (q->q2) + (q->q3) * (q->q3));
                A01 = ((q->q1) * (q->q2) + (q->q0) * (q->q3));
                
                e->Roll = PI * 0.5f;
                e->Pitch = 0.0f;
                e->Yaw = atan2f(A01, A00);
            }
            else if(A12 < -0.9999f)
            {
                A00 = 0.5f - ((q->q2) * (q->q2) + (q->q3) * (q->q3));
                A01 = ((q->q1) * (q->q2) + (q->q0) * (q->q3));
                
                e->Roll = -PI * 0.5f;
                e->Pitch = 0.0f;
                e->Yaw = atan2f(A01, A00);
            }
            else
            {
                A02 = (q->q1) * (q->q3) - (q->q0) * (q->q2);
                A10 = (q->q1) * (q->q2) - (q->q0) * (q->q3);
                A11 = 0.5f - ((q->q1) * (q->q1) + (q->q3) * (q->q3));
                A22 = 0.5f - ((q->q1) * (q->q1) + (q->q2) * (q->q2));
                
                e->Roll = asinf(A12);
                e->Pitch = atan2f(-A02, A22);
                e->Yaw = atan2f(-A10, A11);
            }
            break;
        
        default:
            break;
    }
}

void QuatNormalize(QuaternionStruct *q, QuaternionStruct *QHat)
{
    float NormDiv = 0.0f;
    
    NormDiv = sqrtf((q->q0) * (q->q0) + (q->q1) * (q->q1) + (q->q2) * (q->q2) + (q->q3) * (q->q3));
    NormDiv = 1.0f / NormDiv;
    
    QHat->q0 = (q->q0) * NormDiv;
    QHat->q1 = (q->q1) * NormDiv;
    QHat->q2 = (q->q2) * NormDiv;
    QHat->q3 = (q->q3) * NormDiv;
}

void CalcJacobianInv(ArmUnion *Link, Matrix3X3Union *j)
{
    float cm_div = 0;
    float sm = 0.0f, cm = 0.0f, si = 0.0f, ci = 0.0f;
    
    sm = sinf(Link->Data.Middle);
    cm = cosf(Link->Data.Middle);
    si = sinf(Link->Data.Small);
    ci = cosf(Link->Data.Small);
    
    
    cm_div = 1.0f / cm;

    
    j->Data.A00 = ci * cm_div;
    j->Data.A01 = 0.0f;
    j->Data.A02 = si * cm_div;
    
    
    j->Data.A10 = -si;
    j->Data.A11 = 0.0f;
    j->Data.A12 = ci;
    
    
    j->Data.A20 = ci * sm * cm_div;
    j->Data.A21 = 1.0f;
    j->Data.A22 = si * sm * cm_div;
}

void CalcJacobianTrans(ArmUnion *Link, Matrix3X3Union *j)
{
    float sm = 0.0f, cm = 0.0f, si = 0.0f, ci = 0.0f;
    
    sm = sinf(Link->Data.Middle);
    cm = cosf(Link->Data.Middle);
    si = sinf(Link->Data.Small);
    ci = cosf(Link->Data.Small);

    
    j->Data.A00 = ci * cm;
    j->Data.A01 = -sm;
    j->Data.A02 = cm * si;
    
    
    j->Data.A10 = -si;
    j->Data.A11 = 0.0f;
    j->Data.A12 = ci;
    
    
    j->Data.A20 = 0.0f;
    j->Data.A21 = 1.0f;
    j->Data.A22 = 0.0f;
}
    
    
    
void MatrixAdd(uint8_t ABRow, uint8_t ABCol, float *m_a, float *m_b, float *m_c)
{
    uint8_t i = 0, j = 0;
    uint8_t Index = 0;

    for(i = 0; i < ABRow; i++)
    {
        for(j = 0; j < ABCol; j++)
        {
            Index = i * ABCol + j;
            m_c[Index] = m_a[Index] + m_b[Index];
        }
    }
}

void MatrixSub(uint8_t ABRow, uint8_t ABCol, float *m_a, float *m_b, float *m_c)
{
    uint8_t i = 0, j = 0;
    uint8_t Index = 0;

    for(i = 0; i < ABRow; i++)
    {
        for(j = 0; j < ABCol; j++)
        {
            Index = i * ABCol + j;
            m_c[Index] = m_a[Index] - m_b[Index];
        }
    }
}

void MatrixMultiply(uint8_t ARow, uint8_t AColBRow, uint8_t BCol, float *m_a, float *m_b, float *m_c)
{
    uint8_t i = 0, j = 0, k = 0;

    memset((uint8_t*)m_c, 0, ARow * BCol * 4);
    for(i = 0; i < ARow; i++)
    {
        for(j = 0; j < AColBRow; j++)
        {
            for(k = 0; k < BCol; k++)
            {
                m_c[i * BCol + k] += m_a[i * AColBRow + j] * m_b[j * BCol + k];
            }
        }
    }
}
void AttitudeUpdata(void)
{
    float DeltaQ0 = 0.0f, DeltaQ1 = 0.0f, DeltaQ2 = 0.0f, DeltaQ3 = 0.0f;
    float half_dt = 0.5f / CLOSELOOP_EXECUTE_FREQ;

    
    DeltaQ0 = (-(GimbalSystem.QuaternionPlane.q1) * (GimbalSystem.PlaneRadsCali.Data.X) - (GimbalSystem.QuaternionPlane.q2) * (GimbalSystem.PlaneRadsCali.Data.Y) - (GimbalSystem.QuaternionPlane.q3) * (GimbalSystem.PlaneRadsCali.Data.Z)) * half_dt;
    DeltaQ1 = ((GimbalSystem.QuaternionPlane.q0) * (GimbalSystem.PlaneRadsCali.Data.X) + (GimbalSystem.QuaternionPlane.q2) * (GimbalSystem.PlaneRadsCali.Data.Z) - (GimbalSystem.QuaternionPlane.q3) * (GimbalSystem.PlaneRadsCali.Data.Y)) * half_dt;
    DeltaQ2 = ((GimbalSystem.QuaternionPlane.q0) * (GimbalSystem.PlaneRadsCali.Data.Y) - (GimbalSystem.QuaternionPlane.q1) * (GimbalSystem.PlaneRadsCali.Data.Z) + (GimbalSystem.QuaternionPlane.q3) * (GimbalSystem.PlaneRadsCali.Data.X)) * half_dt;
    DeltaQ3 = ((GimbalSystem.QuaternionPlane.q0) * (GimbalSystem.PlaneRadsCali.Data.Z) + (GimbalSystem.QuaternionPlane.q1) * (GimbalSystem.PlaneRadsCali.Data.Y) - (GimbalSystem.QuaternionPlane.q2) * (GimbalSystem.PlaneRadsCali.Data.X)) * half_dt;

    GimbalSystem.QuaternionPlane.q0 += DeltaQ0;
    GimbalSystem.QuaternionPlane.q1 += DeltaQ1;
    GimbalSystem.QuaternionPlane.q2 += DeltaQ2;
    GimbalSystem.QuaternionPlane.q3 += DeltaQ3;

    
    QuatNormalize(&GimbalSystem.QuaternionPlane, &GimbalSystem.QuaternionPlane);

    
    Quat2Euler(&GimbalSystem.QuaternionPlane, &GimbalSystem.GeoEulerQuat);
}

void PidSetKp(float Kp, PidStruct *Pid)
{
    Pid->Kp = Kp;
}

void PidSetKi(float Ki, PidStruct *Pid)
{
    Pid->Ki = Ki;
}

void PidSetIntegral(float Integral, PidStruct *Pid)
{
    Pid->Integral = Integral;
}

void PidSetIntegralLimit(float LimitMin, float LimitMax, PidStruct *Pid)
{
    Pid->IntegralMin = LimitMin;
    Pid->IntegralMax = LimitMax;
}

void PidSetKd(float Kd, PidStruct *Pid)
{
    Pid->Kd = Kd;
}

void PidSetKdFilterCoef(float FliterCoef, PidStruct *Pid)
{
    Pid->FliterCoef = FliterCoef;
}

void PidSetOldError(float OldError, PidStruct *Pid)
{
    Pid->OldError = OldError;
}

void PidSetOutLimit(float LimitMin, float LimitMax, PidStruct *Pid)
{
    Pid->OutLimitMin = LimitMin;
    Pid->OutLimitMax = LimitMax;
}

float PidControl(float LimitMax, PidStruct *Pid)
{
    float ProportionalTerm = 0.0f, IntegralTerm = 0.0f, IntegralSumTemp = 0.0f, Out = 0.0f;
    float FiffTerm = 0.0f, DeltaLimitMax = 0.0f;

    
    ProportionalTerm = (Pid->Kp) * LimitMax;

    
    if((Pid->Ki) == 0)
    {
        Pid->Integral = 0;
    }
    else
    {
        IntegralTerm = (Pid->Ki) * LimitMax;
        IntegralSumTemp = (Pid->Integral) + IntegralTerm;

        
        if(IntegralSumTemp > (Pid->IntegralMax))
        {
            Pid->Integral = Pid->IntegralMax;
        }
        else if (IntegralSumTemp < (Pid->IntegralMin))
        {
            Pid->Integral = Pid->IntegralMin;
        }
        else
        {
            Pid->Integral = IntegralSumTemp;
        }
    }

    
    LimitMax = ((Pid->FliterCoef) * (Pid->OldError) + (1.0f - (Pid->FliterCoef)) * LimitMax);
    DeltaLimitMax = LimitMax - (Pid->OldError);
    FiffTerm = (Pid->Kd) * DeltaLimitMax;
    Pid->OldError = LimitMax;

    
    Out = ProportionalTerm + (Pid->Integral) + FiffTerm;

    
    if(Out > (Pid->OutLimitMax))
    {
        Out = Pid->OutLimitMax;
    }
    else if(Out < (Pid->OutLimitMin))
    {
        Out = Pid->OutLimitMin;
    }

    return Out;
}

