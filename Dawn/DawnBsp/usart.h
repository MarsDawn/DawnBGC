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


#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"
#define DEBUG_USART_PRE_PRIORITY                 ((uint8_t)2)
#define DEBUG_USART_SUB_PRIORITY                 ((uint8_t)1)

#define DEBUG_USART_RX_DMA_BUFFER_SIZE           ((uint8_t)64)

#define DEBUG_USART_RX_BUFF_SIZE                 ((uint16_t)512)
#define DEBUG_USART_TX_BUFF_SIZE                 ((uint16_t)1024)
void DebugUsartInit(uint32_t baudrate);
void debug_usart_trans_enable(void);
void debug_usart_put_byte(uint8_t *pbuf, uint16_t len);
uint16_t debug_usart_get_byte(uint8_t *pbuf);
void debug_usart_rx_buff_reset(void);
#endif

