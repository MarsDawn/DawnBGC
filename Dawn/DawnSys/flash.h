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


#ifndef __FLASH_H
#define __FLASH_H

#include "gd32f4xx.h"
#define ADDR_FMC_SECTOR_0        ((uint32_t)0x08000000) 
#define ADDR_FMC_SECTOR_1        ((uint32_t)0x08004000) 
#define ADDR_FMC_SECTOR_2        ((uint32_t)0x08008000) 
#define ADDR_FMC_SECTOR_3        ((uint32_t)0x0800C000) 
#define ADDR_FMC_SECTOR_4        ((uint32_t)0x08010000) 
#define ADDR_FMC_SECTOR_5        ((uint32_t)0x08020000) 

#define ADDR_FMC_SECTOR_6        ((uint32_t)0x08040000) 
#define ADDR_FMC_SECTOR_7        ((uint32_t)0x08060000) 

#define ADDR_FMC_SECTOR_8        ((uint32_t)0x08080000) 
#define ADDR_FMC_SECTOR_9        ((uint32_t)0x080A0000) 
#define ADDR_FMC_SECTOR_10       ((uint32_t)0x080C0000) 
#define ADDR_FMC_SECTOR_11       ((uint32_t)0x080E0000) 
#define ADDR_FMC_SECTOR_12       ((uint32_t)0x08100000) 
#define ADDR_FMC_SECTOR_13       ((uint32_t)0x08104000) 
#define ADDR_FMC_SECTOR_14       ((uint32_t)0x08108000) 
#define ADDR_FMC_SECTOR_15       ((uint32_t)0x0810C000) 
#define ADDR_FMC_SECTOR_16       ((uint32_t)0x08110000) 
#define ADDR_FMC_SECTOR_17       ((uint32_t)0x08120000) 
#define ADDR_FMC_SECTOR_18       ((uint32_t)0x08140000) 
#define ADDR_FMC_SECTOR_19       ((uint32_t)0x08160000) 
#define ADDR_FMC_SECTOR_20       ((uint32_t)0x08180000) 
#define ADDR_FMC_SECTOR_21       ((uint32_t)0x081A0000) 
#define ADDR_FMC_SECTOR_22       ((uint32_t)0x081C0000) 
#define ADDR_FMC_SECTOR_23       ((uint32_t)0x081E0000) 
#define ADDR_FMC_SECTOR_24       ((uint32_t)0x08200000) 
#define ADDR_FMC_SECTOR_25       ((uint32_t)0x08240000) 
#define ADDR_FMC_SECTOR_26       ((uint32_t)0x08280000) 
#define ADDR_FMC_SECTOR_27       ((uint32_t)0x082C0000) 
void flash_erase(uint32_t saddr, uint32_t eaddr);
void flash_write_words(uint32_t waddr, uint32_t *pbuf, uint32_t num);
void flash_read_words(uint32_t raddr, uint32_t *pbuf, uint32_t num);

#endif

