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

void iwdg_init(uint8_t prer, uint16_t rlr)
{
    
    fwdgt_config(rlr, prer);
    
    
    fwdgt_counter_reload();
    
    dbg_periph_disable(DBG_FWDGT_HOLD);

    fwdgt_enable();
}

void iwdg_feed(void)
{
    fwdgt_counter_reload();
}
