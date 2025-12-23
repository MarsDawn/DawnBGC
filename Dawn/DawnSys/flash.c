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

static uint32_t flash_sector_get(uint32_t addr)
{
    uint32_t sector = 0;
    
    if((addr < ADDR_FMC_SECTOR_1) && (addr >= ADDR_FMC_SECTOR_0))
    {
        sector = CTL_SECTOR_NUMBER_0;
    }
    else if((addr < ADDR_FMC_SECTOR_2) && (addr >= ADDR_FMC_SECTOR_1))
    {
        sector = CTL_SECTOR_NUMBER_1;
    }
    else if((addr < ADDR_FMC_SECTOR_3) && (addr >= ADDR_FMC_SECTOR_2))
    {
        sector = CTL_SECTOR_NUMBER_2;
    }
    else if((addr < ADDR_FMC_SECTOR_4) && (addr >= ADDR_FMC_SECTOR_3))
    {
        sector = CTL_SECTOR_NUMBER_3;  
    }
    else if((addr < ADDR_FMC_SECTOR_5) && (addr >= ADDR_FMC_SECTOR_4))
    {
        sector = CTL_SECTOR_NUMBER_4;
    }
    else if((addr < ADDR_FMC_SECTOR_6) && (addr >= ADDR_FMC_SECTOR_5))
    {
        sector = CTL_SECTOR_NUMBER_5;
    }
    else if((addr < ADDR_FMC_SECTOR_7) && (addr >= ADDR_FMC_SECTOR_6))
    {
        sector = CTL_SECTOR_NUMBER_6;
    }
    else if((addr < ADDR_FMC_SECTOR_8) && (addr >= ADDR_FMC_SECTOR_7))
    {
        sector = CTL_SECTOR_NUMBER_7;  
    }
    else if((addr < ADDR_FMC_SECTOR_9) && (addr >= ADDR_FMC_SECTOR_8))
    {
        sector = CTL_SECTOR_NUMBER_8;
    }
    else if((addr < ADDR_FMC_SECTOR_10) && (addr >= ADDR_FMC_SECTOR_9))
    {
        sector = CTL_SECTOR_NUMBER_9;
    }
    else if((addr < ADDR_FMC_SECTOR_11) && (addr >= ADDR_FMC_SECTOR_10))
    {
        sector = CTL_SECTOR_NUMBER_10;
    }
    else if((addr < ADDR_FMC_SECTOR_12) && (addr >= ADDR_FMC_SECTOR_11))
    {
        sector = CTL_SECTOR_NUMBER_11;
    }
    else if((addr < ADDR_FMC_SECTOR_13) && (addr >= ADDR_FMC_SECTOR_12))
    {
        sector = CTL_SECTOR_NUMBER_12;
    }
    else if((addr < ADDR_FMC_SECTOR_14) && (addr >= ADDR_FMC_SECTOR_13))
    {
        sector = CTL_SECTOR_NUMBER_13;
    }
    else if((addr < ADDR_FMC_SECTOR_15) && (addr >= ADDR_FMC_SECTOR_14))
    {
        sector = CTL_SECTOR_NUMBER_14;
    }
    else if((addr < ADDR_FMC_SECTOR_16) && (addr >= ADDR_FMC_SECTOR_15))
    {
        sector = CTL_SECTOR_NUMBER_15;
    }
    else if((addr < ADDR_FMC_SECTOR_17) && (addr >= ADDR_FMC_SECTOR_16))
    {
        sector = CTL_SECTOR_NUMBER_16;
    }
    else if((addr < ADDR_FMC_SECTOR_18) && (addr >= ADDR_FMC_SECTOR_17))
    {
        sector = CTL_SECTOR_NUMBER_17;
    }
    else if((addr < ADDR_FMC_SECTOR_19) && (addr >= ADDR_FMC_SECTOR_18))
    {
        sector = CTL_SECTOR_NUMBER_18;
    }
    else if((addr < ADDR_FMC_SECTOR_20) && (addr >= ADDR_FMC_SECTOR_19))
    {
        sector = CTL_SECTOR_NUMBER_19;
    }
    else if((addr < ADDR_FMC_SECTOR_21) && (addr >= ADDR_FMC_SECTOR_20))
    {
        sector = CTL_SECTOR_NUMBER_20;
    }
    else if((addr < ADDR_FMC_SECTOR_22) && (addr >= ADDR_FMC_SECTOR_21))
    {
        sector = CTL_SECTOR_NUMBER_21;
    }
    else if((addr < ADDR_FMC_SECTOR_23) && (addr >= ADDR_FMC_SECTOR_22))
    {
        sector = CTL_SECTOR_NUMBER_22;
    }
    else if((addr < ADDR_FMC_SECTOR_24) && (addr >= ADDR_FMC_SECTOR_23))
    {
        sector = CTL_SECTOR_NUMBER_23;
    }
    else if((addr < ADDR_FMC_SECTOR_25) && (addr >= ADDR_FMC_SECTOR_24))
    {
        sector = CTL_SECTOR_NUMBER_24;
    }
    else if((addr < ADDR_FMC_SECTOR_26) && (addr >= ADDR_FMC_SECTOR_25))
    {
        sector = CTL_SECTOR_NUMBER_25;
    }
    else if((addr < ADDR_FMC_SECTOR_27) && (addr >= ADDR_FMC_SECTOR_26))
    {
        sector = CTL_SECTOR_NUMBER_26;
    }
    else
    {
        sector = CTL_SECTOR_NUMBER_27;
    }
    
    return sector;
}

void flash_erase(uint32_t saddr, uint32_t eaddr)
{
    uint32_t i = 0, start_page = 0, end_page = 0;

    start_page = flash_sector_get(saddr);
    end_page = flash_sector_get(eaddr);

    
    fmc_unlock();

    
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    
    for(i = start_page; i < end_page; i += 8)
    {
        
        iwdg_feed();
        
        fmc_sector_erase(i);
    }

    
    fmc_lock();
}

void flash_write_words(uint32_t waddr, uint32_t *pbuf, uint32_t num)
{
    uint32_t i = 0;
    
    
    fmc_unlock();

    
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    for(i=0; i < num; i++)
    {
        
        iwdg_feed();
        
        fmc_word_program(waddr, pbuf[i]);
        waddr += 4;
    }

    
    fmc_lock();
}

void flash_read_words(uint32_t raddr, uint32_t *pbuf, uint32_t num)
{
    uint32_t i;

    for(i=0; i < num; i++)
    {
        
        pbuf[i] = *(uint32_t*)(raddr);
        
        raddr += 4;
    }
}
