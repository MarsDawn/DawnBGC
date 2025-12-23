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


#ifndef GD32F4XX_LIBOPT_H
#define GD32F4XX_LIBOPT_H

#if defined (GD32F450) || defined (GD32F405) || defined (GD32F407)
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_adc.h"
#include "gd32f4xx_can.h"
#include "gd32f4xx_crc.h"
#include "gd32f4xx_ctc.h"
#include "gd32f4xx_dac.h"
#include "gd32f4xx_dbg.h"
#include "gd32f4xx_dci.h"
#include "gd32f4xx_dma.h"
#include "gd32f4xx_exti.h"
#include "gd32f4xx_fmc.h"
#include "gd32f4xx_fwdgt.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_syscfg.h"
#include "gd32f4xx_i2c.h"
#include "gd32f4xx_iref.h"
#include "gd32f4xx_pmu.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_rtc.h"
#include "gd32f4xx_sdio.h"
#include "gd32f4xx_spi.h"
#include "gd32f4xx_timer.h"
#include "gd32f4xx_trng.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_wwdgt.h"
#include "gd32f4xx_misc.h"
#endif

#if defined (GD32F450)
#include "gd32f4xx_enet.h"
#include "gd32f4xx_exmc.h"
#include "gd32f4xx_ipa.h"
#include "gd32f4xx_tli.h"
#endif

#if defined (GD32F407)
#include "gd32f4xx_enet.h"
#include "gd32f4xx_exmc.h"
#endif

#endif 
