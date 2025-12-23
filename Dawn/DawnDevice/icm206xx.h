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


#ifndef __ICM206XX_H
#define __ICM206XX_H

#include "gd32f4xx.h"

#define ICM206XX_INT_PRE_PRIORITY                 ((uint8_t)1)
#define ICM206XX_INT_SUB_PRIORITY                 ((uint8_t)0)

#define DMA_RX_PRE_PRIORITY                      ((uint8_t)1)
#define DMA_RX_SUB_PRIORITY                      ((uint8_t)0)
typedef struct
{
    int16_t HAccX;                        
    int16_t HAccY;
    int16_t HAccZ;
    int16_t HTemp;
    int16_t HGyroX;
    int16_t HGyroY;
    int16_t HGyroZ;
    
    float FAccX;                          
    float FAccY;
    float FAccZ;
    float FTemp;                           
    float FGyroX;                         
    float FGyroY;
    float FGyroZ;
    
    float AccScale;
    float GyroScale;
    float TempScale;
    
    uint8_t WhoAmI;                       
}Icm206XXDataStruct;

extern Icm206XXDataStruct Icm206XXData;
void Icm206XXInit(void);

#endif

