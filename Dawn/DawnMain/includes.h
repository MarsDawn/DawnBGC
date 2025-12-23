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


#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdint.h>   
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "gd32f4xx.h"

#ifndef __INLINE
  #ifdef __STATIC_INLINE
    #define __INLINE __STATIC_INLINE
  #else
    #define __INLINE inline
  #endif
#endif

#include "arm_math.h"

#include "dawn_lib.h"
#include "dawn_main.h"



#include "usart.h"
#include "timebase.h"

#include "crc.h"
#include "unpack.h"
#include "icm206xx.h"
#include "dawn_function.h"
#include "dawn_gimbal_system.h"
#include "dawn_gimbal_math.h"

#include "dawn_bldc.h"
#include "gpio.h"
#include "wdg.h"
#include "systick.h"
#include "flash.h"
#endif /* __INCLUDES_H */
