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

void BLDCInit(void)
{
    BLDCGpioInit();

    TurnOnPWM(&BLDCYaw);
    TurnOnPWM(&BLDCRoll);
    TurnOnPWM(&BLDCPitch);
}


uint8_t BLDCStallState(BLDCStruct *BLDCStruct)
{
    if ((fabs(BLDCStruct->Speed) < (BLDCStruct->StallSpeed)) && (BLDCStruct->HomingMaxFlag) || (fabs(BLDCStruct->Speed) < (BLDCStruct->StallSpeed)) && (BLDCStruct->HomingMaxFlag))
    {
        
        if ((BLDCStruct->StallTime) < (BLDCStruct->StallWaitTime))
        {
            BLDCStruct->StallTime++;
            BLDCStruct->StallFlag = FALSE;
        }
        else
        {
            BLDCStruct->StallFlag = TRUE;
        }
    }
    else
    {
        
        BLDCStruct->StallTime--;
        if ((BLDCStruct->StallTime) < 0)
        {
            BLDCStruct->StallTime = 0;
        }

        BLDCStruct->StallFlag = FALSE;
    }

    return BLDCStruct->StallFlag;
}

void ArmQuadCcw(BLDCStruct *BLDCStruct)
{
    BLDCStruct->QuadDirectStruct.Direct= 0.0f;
    if ((BLDCStruct->QuadDirectStruct.Quad) > (BLDCStruct->HomingCCwQuad))
    {
        if ((BLDCStruct->QuadDirectStruct.Quad) > (BLDCStruct->HomingCCwQuad) * (BLDCStruct->BangHomingCOE))
        {
            
            BLDCStruct->QuadDirectStruct.Quad = (BLDCStruct->HomingCCwQuad) * (BLDCStruct->BangHomingCOE);
        }
        BLDCStruct->HomingMaxFlag = FALSE;
        BLDCStruct->QuadDirectStruct.Quad -= 3;
        ;
    }
    else
    {
        BLDCStruct->HomingMaxFlag = TRUE;
        BLDCStruct->QuadDirectStruct.Quad = BLDCStruct->HomingCCwQuad;
    }
}

void ArmQuadCw(BLDCStruct *BLDCStruct)
{
    BLDCStruct->QuadDirectStruct.Direct= 0.0f;
    if ((BLDCStruct->QuadDirectStruct.Quad) < (BLDCStruct->HomingCwQuad))
    {
        if ((BLDCStruct->QuadDirectStruct.Quad) < (BLDCStruct->HomingCwQuad) * (BLDCStruct->BangHomingCOE))
        {
            
            BLDCStruct->QuadDirectStruct.Quad = (BLDCStruct->HomingCwQuad) * (BLDCStruct->BangHomingCOE);
        }
        BLDCStruct->HomingMaxFlag = FALSE;

        BLDCStruct->QuadDirectStruct.Quad += 3;
        ;
    }
    else
    {
        BLDCStruct->HomingMaxFlag = TRUE;
        BLDCStruct->QuadDirectStruct.Quad = BLDCStruct->HomingCwQuad;
    }
}
void CleanBLDCAngle(BLDCStruct *BLDCStruct)
{
    BLDCStruct->CalcAnglesRadRes = fmodf((BLDCStruct->CalcAnglesRadRes), 2.0f * PI);
    if ((BLDCStruct->CalcAnglesRadRes) < 0.0f)
    {
        BLDCStruct->CalcAnglesRadRes += 2.0f * PI;
    }
    BLDCStruct->HomingCcwRad = (BLDCStruct->CalcAnglesRadRes) / (BLDCStruct->PoleDouble);
    
    BLDCStruct->ArmCCwERad = fmodf((BLDCStruct->CalcAnglesRadRes), 2.0f * PI);

    BLDCStruct->BangCCwFlag = TRUE;
    BLDCStruct->QuadDirectStruct.Quad = (BLDCStruct->HomingCwQuad) * 0.5f;
}
void BLDCAngleCal(BLDCStruct *BLDCStruct)
{
    
    BLDCStruct->HomingCwRad = (BLDCStruct->CalcAnglesRadRes) / (BLDCStruct->PoleDouble);
    
    BLDCStruct->ArmCwERad = fmodf((BLDCStruct->CalcAnglesRadRes), 2.0f * PI);

    
    BLDCStruct->BLDCRacOffsetCoe = (BLDCStruct->HomingCcwRad) + ((BLDCStruct->HomingCwRad) - (BLDCStruct->HomingCcwRad)) * fabs(BLDCStruct->ArmCCwRad) / ((BLDCStruct->ArmCwRad) - (BLDCStruct->ArmCCwRad));
    BLDCStruct->BLDCRacOffset = BLDCStruct->BLDCRacOffsetCoe;
    BLDCStruct->QuadDirectStruct.Quad = 0.0f;
    BLDCStruct->StateCheck = 1;

    BLDCStruct->BangCCwFlag = FALSE;
    BLDCStruct->BangCwFlag = FALSE;
}

void BLDCCali(BLDCStruct *BLDCStruct)
{
    static uint8_t StannTimes = 0;
    HallDataRead(BLDCStruct);
    BLDCStruct->ParkStruct = BLDCClarke(BLDCStruct->HallStruct);
    BLDCEAngle(BLDCStruct);

    if (!BLDCStruct->StateCheck)
    {
    

        
        if (BLDCStruct->BangCCwFlag == FALSE)
        {
            
            ArmQuadCcw(BLDCStruct);

            if (BLDCStallState(BLDCStruct))
            {
                
                BLDCStruct->StallTime = 0;
                
                
                
                StannTimes++;
                if (StannTimes > 6)
                {
                    ShutOffPWM(&BLDCYaw);
                    ShutOffPWM(&BLDCRoll);
                    ShutOffPWM(&BLDCPitch);
                }

                
                CleanBLDCAngle(BLDCStruct);
            }
        }
        else
        {
            
            ArmQuadCw(BLDCStruct);
            if (BLDCStallState(BLDCStruct))
            {
                
                BLDCStruct->StallTime = 0;

                BLDCStruct->BangCwFlag = TRUE;
                BLDCAngleCal(BLDCStruct);
            }
        }
        BLDCStruct->VoltStrict = BLDCRevPark(BLDCStruct->QuadDirectStruct, BLDCStruct->ERad);
    }
    else if (BLDCStruct->StateCheck)
    {

        BLDCStruct->QuadDirectStruct.Direct= BLDCStruct->DirectOut;
        BLDCStruct->QuadDirectStruct.Quad = BLDCStruct->QuadOut;
        BLDCStruct->VoltStrict = BLDCRevPark(BLDCStruct->QuadDirectStruct, BLDCStruct->ERad);

    }

    BLDCSvpwm(BLDCStruct);
}
void BLDCSvpwm(BLDCStruct *BLDCStruct)
{
    uint8_t Sector = 0;
    int16_t X = 0, Y = 0, Z = 0, A = 0, B  = 0, t = 0;
    uint16_t PhaseA = 0, PhaseB = 0, PhaseC = 0;
    
    A = ((int32_t)(BLDCStruct->VoltStrict.A) * T_SQRT3 ) >> 16;
    B  = ((int32_t)(BLDCStruct->VoltStrict.B) * T) >> 16;

    X = B ;
    Y = (B  + A) >> 1;
    Z = (B  - A) >> 1;

    if(B  >= 0)
    {
        Sector += 1;
    }

    if((A - B ) >= 0)
    {
        Sector += 2;
    }

    if((A + B ) < 0)
    {
        Sector += 4;
    }
    
    switch(Sector)
    {
        case SECTOR_1:
            t = Z + Y;
            if(t > T_MAX_LIMIT)
            {
                Z = (int32_t)Z * (int32_t)T_MAX_LIMIT / (int32_t)t;
                Y = (int32_t)Y * (int32_t)T_MAX_LIMIT / (int32_t)t;
            }
            PhaseB = (T_QUARTER) - (Z + Y) * 0.5f;
            PhaseA = PhaseB + Z;
            PhaseC = PhaseA + Y;
            break;

         case SECTOR_2:
            t = Y + (-X);
            if(t > T_MAX_LIMIT)
            {
                Y = (int32_t)Y * (int32_t)T_MAX_LIMIT / (int32_t)t;
                X = (int32_t)X * (int32_t)T_MAX_LIMIT / (int32_t)t;
            }
            PhaseA = (T_QUARTER) - (Y + (-X)) * 0.5f;
            PhaseC = PhaseA + Y;
            PhaseB = PhaseC + (-X);
            break;

         case SECTOR_3:
            t = (-Z) + X;
            if(t > T_MAX_LIMIT)
            {
                Z = (int32_t)Z * (int32_t)T_MAX_LIMIT / (int32_t)t;
                X = (int32_t)X * (int32_t)T_MAX_LIMIT / (int32_t)t;
            }
            PhaseA = (T_QUARTER) - ((-Z) + X) * 0.5f;
            PhaseB = PhaseA + (-Z);
            PhaseC = PhaseB + X;
            break;

         case SECTOR_4:
            t = (-X) + Z;
            if(t > T_MAX_LIMIT)
            {
                X = (int32_t)X * (int32_t)T_MAX_LIMIT / (int32_t)t;
                Z= (int32_t)Z * (int32_t)T_MAX_LIMIT / (int32_t)t;
            }
            PhaseC = (T_QUARTER) - ((-X) + Z) * 0.5f;
            PhaseB = PhaseC + (-X);
            PhaseA = PhaseB + Z;
            break;

         case SECTOR_5:
            t = X + (-Y);
            if(t > T_MAX_LIMIT)
            {
                X = (int32_t)X * (int32_t)T_MAX_LIMIT / (int32_t)t;
                Y = (int32_t)Y * (int32_t)T_MAX_LIMIT / (int32_t)t;
            }
            PhaseB = (T_QUARTER) - (X + (-Y)) * 0.5f;
            PhaseC = PhaseB + X;
            PhaseA = PhaseC + (-Y);
            break;

         case SECTOR_6:
            t = (-Y) + (-Z);
            if(t > T_MAX_LIMIT)
            {
                Y = (int32_t)Y * (int32_t)T_MAX_LIMIT / (int32_t)t;
                Z = (int32_t)Z * (int32_t)T_MAX_LIMIT / (int32_t)t;
            }
            PhaseC = (T_QUARTER) - ((-Y) + (-Z)) * 0.5f;
            PhaseA = PhaseC + (-Y);
            PhaseB = PhaseA + (-Z);
            break;

        default:
            break;
    }
    
    switch(BLDCStruct->BLDCId)
    {
        case BLDC_ID_YAW:
            
						TIMER_CH0CV(TIMER0) = PhaseB;
            TIMER_CH1CV(TIMER0) = PhaseC;
            TIMER_CH2CV(TIMER0) = PhaseA;    
            break;					
				
        case BLDC_ID_ROLL:
            TIMER_CH0CV(TIMER1) = PhaseA;
            TIMER_CH1CV(TIMER1) = PhaseC;
            TIMER_CH2CV(TIMER1) = PhaseB;
            break;
        
        case BLDC_ID_PITCH:
            TIMER_CH0CV(TIMER7) = PhaseC;
            TIMER_CH1CV(TIMER7) = PhaseB;
            TIMER_CH2CV(TIMER7) = PhaseA;
            break;
    }
}

void TurnOnPWM(BLDCStruct *BLDCStruct)
{
    switch(BLDCStruct->BLDCId)
    {
        case BLDC_ID_YAW:
            TIMER_CH0CV(TIMER0) = HALF_PWM_PERIOD * 0.5f;
            TIMER_CH1CV(TIMER0) = HALF_PWM_PERIOD * 0.5f;
            TIMER_CH2CV(TIMER0) = HALF_PWM_PERIOD * 0.5f;
            
            timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_ENABLE);
            timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_ENABLE);
            timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_ENABLE);
            break;
        
        case BLDC_ID_ROLL:

            TIMER_CH0CV(TIMER1) = HALF_PWM_PERIOD * 0.5f;
            TIMER_CH1CV(TIMER1) = HALF_PWM_PERIOD * 0.5f;
            TIMER_CH2CV(TIMER1) = HALF_PWM_PERIOD * 0.5f;
            
            timer_channel_output_state_config(TIMER1, TIMER_CH_0, TIMER_CCX_ENABLE);
            timer_channel_output_state_config(TIMER1, TIMER_CH_1, TIMER_CCX_ENABLE);
            timer_channel_output_state_config(TIMER1, TIMER_CH_2, TIMER_CCX_ENABLE);
            break;
        
        case BLDC_ID_PITCH:
            TIMER_CH0CV(TIMER7) = HALF_PWM_PERIOD * 0.5f;
            TIMER_CH1CV(TIMER7) = HALF_PWM_PERIOD * 0.5f;
            TIMER_CH2CV(TIMER7) = HALF_PWM_PERIOD * 0.5f;
            
            timer_channel_output_state_config(TIMER7, TIMER_CH_0, TIMER_CCX_ENABLE);
            timer_channel_output_state_config(TIMER7, TIMER_CH_1, TIMER_CCX_ENABLE);
            timer_channel_output_state_config(TIMER7, TIMER_CH_2, TIMER_CCX_ENABLE);
            break;
    }
}

void ShutOffPWM(BLDCStruct *BLDCStruct)
{
    switch(BLDCStruct->BLDCId)
    {
        case BLDC_ID_YAW:
            timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_DISABLE);
            timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_DISABLE);
            timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_DISABLE);
            break;
        
        case BLDC_ID_ROLL:
            timer_channel_output_state_config(TIMER1, TIMER_CH_0, TIMER_CCX_DISABLE);
            timer_channel_output_state_config(TIMER1, TIMER_CH_1, TIMER_CCX_DISABLE);
            timer_channel_output_state_config(TIMER1, TIMER_CH_2, TIMER_CCX_DISABLE);
            break;
        
        case BLDC_ID_PITCH:
            timer_channel_output_state_config(TIMER7, TIMER_CH_0, TIMER_CCX_DISABLE);
            timer_channel_output_state_config(TIMER7, TIMER_CH_1, TIMER_CCX_DISABLE);
            timer_channel_output_state_config(TIMER7, TIMER_CH_2, TIMER_CCX_DISABLE);
            break;
    }
}

void HallDataRead(BLDCStruct *BLDCStruct)
{
    switch(BLDCStruct->BLDCId)
    {
        case BLDC_ID_YAW:
            BLDCStruct->HallAdcY = (uint16_t)AdcDate[1];
            BLDCStruct->HallAdcX = (AdcDate[1]) >> 16;
            break;
        
        case BLDC_ID_ROLL:
            BLDCStruct->HallAdcY = (uint16_t)AdcDate[0];
            BLDCStruct->HallAdcX = (AdcDate[0]) >> 16;
            break;
        
        case BLDC_ID_PITCH:
            BLDCStruct->HallAdcX = (uint16_t)AdcDate[2];
            BLDCStruct->HallAdcY = (AdcDate[2]) >> 16;
            break;
    }
    
    BLDCStruct->HallStruct.X = (BLDCStruct->HallAdcX) - (BLDCStruct->HallOffsetX);
    BLDCStruct->HallStruct.Y = ((BLDCStruct->HallAdcY) - (BLDCStruct->HallOffsetY)) * (BLDCStruct->HallDir);
	
}

