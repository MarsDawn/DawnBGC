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
#include "usart.h"

static bool is_debug_usart_dma_trans_completed = TRUE;

static uint8_t debug_usart_rx_buf[DEBUG_USART_RX_BUFF_SIZE] = {0};
static uint8_t debug_usart_tx_buf[DEBUG_USART_TX_BUFF_SIZE] = {0};

static uint16_t debug_usart_rx_buff_head = 0;
static uint16_t debug_usart_rx_buff_tail = 0;
static uint16_t debug_usart_rx_buff_tail_last = 0;
static uint16_t debug_usart_rx_dma_cnt = 0;

static uint16_t debug_usart_tx_buff_head = 0;
static uint16_t debug_usart_tx_buff_tail = 0;

void DebugUsartInit(uint32_t baudrate)
{
    dma_single_data_parameter_struct dma_init_struct;
    
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_DMA1);
    
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);  
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);  
    
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    
    
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudrate);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    
    nvic_irq_enable(USART0_IRQn, DEBUG_USART_PRE_PRIORITY, DEBUG_USART_SUB_PRIORITY);
    
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);
    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);
    
    usart_flag_clear(USART0, USART_FLAG_TC);
    usart_interrupt_enable(USART0, USART_INTEN_IDLEIE);    
    
    dma_deinit(DMA1, DMA_CH5);   
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.memory0_addr = (uint32_t)&debug_usart_rx_buf[0];
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.number = DEBUG_USART_RX_DMA_BUFFER_SIZE;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH5, dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH5, DMA_SUBPERI4);
    
    debug_usart_rx_dma_cnt = DEBUG_USART_RX_DMA_BUFFER_SIZE;
    
    nvic_irq_enable(DMA1_Channel5_IRQn, DEBUG_USART_PRE_PRIORITY, DEBUG_USART_SUB_PRIORITY);
    
    dma_deinit(DMA1, DMA_CH7);   
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.memory0_addr = (uint32_t)&debug_usart_tx_buf[0];
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.number = DEBUG_USART_TX_BUFF_SIZE;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);

    nvic_irq_enable(DMA1_Channel7_IRQn, DEBUG_USART_PRE_PRIORITY, DEBUG_USART_SUB_PRIORITY);

    dma_channel_enable(DMA1, DMA_CH5);
    dma_channel_disable(DMA1, DMA_CH7);          
    
    dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INTC_FTFIFC);
    dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INTC_FTFIFC);
    
    dma_interrupt_enable(DMA1, DMA_CH5, DMA_CHXCTL_FTFIE);
    dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
        
    usart_enable(USART0);
}

void debug_usart_trans_enable(void)
{
    
    if(is_debug_usart_dma_trans_completed == TRUE)
    {
        usart_interrupt_enable(USART0, USART_INTEN_TBEIE);   
    }
}

void debug_usart_put_byte(uint8_t *pbuf, uint16_t len)
{
    uint16_t next_head = 0;          
    uint16_t len1 = 0, len2 = 0;

    next_head = debug_usart_tx_buff_head + len;
    if(next_head <= DEBUG_USART_TX_BUFF_SIZE)
    {
        memcpy(&debug_usart_tx_buf[debug_usart_tx_buff_head], pbuf, len);
    }
    else
    {
        len1 = DEBUG_USART_TX_BUFF_SIZE - debug_usart_tx_buff_head;
        len2 = len - len1;
        memcpy(&debug_usart_tx_buf[debug_usart_tx_buff_head], pbuf, len1);
        memcpy(&debug_usart_tx_buf[0], &pbuf[len1], len2);
    }
    debug_usart_tx_buff_head = next_head % DEBUG_USART_TX_BUFF_SIZE;
}

uint16_t debug_usart_get_byte(uint8_t *pbuf)
{
    uint16_t len = 0, local_rx_head = 0;
    
    
    local_rx_head = debug_usart_rx_buff_head;
    if(debug_usart_rx_buff_tail < local_rx_head)
    {
        len = local_rx_head - debug_usart_rx_buff_tail;
        memcpy(pbuf, &debug_usart_rx_buf[debug_usart_rx_buff_tail], len);
        debug_usart_rx_buff_tail = local_rx_head;
    }
    else if(debug_usart_rx_buff_tail > local_rx_head)
    {
        len = DEBUG_USART_RX_BUFF_SIZE - debug_usart_rx_buff_tail;
        memcpy(pbuf, &debug_usart_rx_buf[debug_usart_rx_buff_tail], len);
        debug_usart_rx_buff_tail = 0;
    }

    return len;
}

void debug_usart_rx_buff_reset(void)
{
    debug_usart_rx_buff_head = 0;
    debug_usart_rx_buff_tail = 0;
    debug_usart_rx_buff_tail_last = 0;
    debug_usart_rx_dma_cnt = DEBUG_USART_RX_DMA_BUFFER_SIZE;
    
    dma_interrupt_disable(DMA1, DMA_CH5, DMA_CHXCTL_FTFIE);
    dma_channel_disable(DMA1, DMA_CH5);
    DMA_CHM0ADDR(DMA1, DMA_CH5) = (uint32_t)&debug_usart_rx_buf[debug_usart_rx_buff_head];
    DMA_CHCNT(DMA1, DMA_CH5) = debug_usart_rx_dma_cnt;
    
    dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC);
    dma_channel_enable(DMA1, DMA_CH5);
    dma_interrupt_enable(DMA1, DMA_CH5, DMA_CHXCTL_FTFIE);
}

void DMA1_Channel5_IRQHandler(void)
{
    
    if(dma_interrupt_flag_get(DMA1, DMA_CH5, DMA_INTF_FTFIF) == SET)
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC);
        
        if(debug_usart_rx_buff_head >= debug_usart_rx_buff_tail_last)   
        {
            debug_usart_rx_buff_head += debug_usart_rx_dma_cnt;
            debug_usart_rx_buff_head %= DEBUG_USART_RX_BUFF_SIZE;
                    
            if(debug_usart_rx_buff_head > debug_usart_rx_buff_tail_last)
            {
                debug_usart_rx_dma_cnt = DEBUG_USART_RX_BUFF_SIZE - debug_usart_rx_buff_head;
                debug_usart_rx_buff_tail_last = debug_usart_rx_buff_tail;
            }
            else
            {
                debug_usart_rx_buff_tail_last = debug_usart_rx_buff_tail;
                debug_usart_rx_dma_cnt = debug_usart_rx_buff_tail_last;
            }
        }
        else if(debug_usart_rx_buff_head < debug_usart_rx_buff_tail_last)
        {
            debug_usart_rx_buff_head += debug_usart_rx_dma_cnt;
            debug_usart_rx_buff_head %= DEBUG_USART_RX_BUFF_SIZE;
            
            debug_usart_rx_buff_tail_last = debug_usart_rx_buff_tail;
            
            debug_usart_rx_dma_cnt = debug_usart_rx_buff_tail_last - debug_usart_rx_buff_head;
        }
        
        if(debug_usart_rx_dma_cnt > DEBUG_USART_RX_DMA_BUFFER_SIZE)
        {
            debug_usart_rx_dma_cnt = DEBUG_USART_RX_DMA_BUFFER_SIZE;
        }

        dma_channel_disable(DMA1, DMA_CH5);
        DMA_CHM0ADDR(DMA1, DMA_CH5) = (uint32_t)&debug_usart_rx_buf[debug_usart_rx_buff_head];
        DMA_CHCNT(DMA1, DMA_CH5) = debug_usart_rx_dma_cnt;
        dma_channel_enable(DMA1, DMA_CH5);
    }
}

void DMA1_Channel7_IRQHandler(void)
{
    
    if(dma_interrupt_flag_get(DMA1, DMA_CH7, DMA_INTF_FTFIF) == SET)
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC);

        is_debug_usart_dma_trans_completed = TRUE;
        
        debug_usart_trans_enable();   
    }
}

void USART0_IRQHandler(void)
{
    uint16_t len = 0;
    
    
    if(usart_interrupt_flag_get(USART0, USART_INT_IDLEIE) == SET)   
    {
        
        usart_data_receive(USART0);
        
        len = debug_usart_rx_dma_cnt - dma_transfer_number_get(DMA1, DMA_CH5);  
        debug_usart_rx_buff_head += len;
        
        debug_usart_rx_buff_tail_last = debug_usart_rx_buff_tail;
        
        if(debug_usart_rx_buff_head >= debug_usart_rx_buff_tail_last)   
        {
            debug_usart_rx_dma_cnt = DEBUG_USART_RX_BUFF_SIZE - debug_usart_rx_buff_head;
        }
        else if(debug_usart_rx_buff_head < debug_usart_rx_buff_tail_last)
        {
            debug_usart_rx_dma_cnt = debug_usart_rx_buff_tail_last - debug_usart_rx_buff_head;
        }
        
        if(debug_usart_rx_dma_cnt > DEBUG_USART_RX_DMA_BUFFER_SIZE)
        {
            debug_usart_rx_dma_cnt = DEBUG_USART_RX_DMA_BUFFER_SIZE;
        }

        dma_interrupt_disable(DMA1, DMA_CH5, DMA_CHXCTL_FTFIE);
        dma_channel_disable(DMA1, DMA_CH5);
        DMA_CHM0ADDR(DMA1, DMA_CH5) = (uint32_t)&debug_usart_rx_buf[debug_usart_rx_buff_head];
        DMA_CHCNT(DMA1, DMA_CH5) = debug_usart_rx_dma_cnt;
        
        dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC);
        dma_channel_enable(DMA1, DMA_CH5);
        dma_interrupt_enable(DMA1, DMA_CH5, DMA_CHXCTL_FTFIE);
    }
    
    
    if(usart_interrupt_flag_get(USART0, USART_INT_TBEIE) == SET)
    {
        if(debug_usart_tx_buff_head > debug_usart_tx_buff_tail)       
        {
            
            if(is_debug_usart_dma_trans_completed == TRUE)
            {
                is_debug_usart_dma_trans_completed = FALSE;
                
                dma_interrupt_disable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
                dma_channel_disable(DMA1, DMA_CH7);
                DMA_CHM0ADDR(DMA1, DMA_CH7) = (uint32_t)&debug_usart_tx_buf[debug_usart_tx_buff_tail];
                DMA_CHCNT(DMA1, DMA_CH7) = debug_usart_tx_buff_head - debug_usart_tx_buff_tail;
                
                dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC);
                dma_channel_enable(DMA1, DMA_CH7);
                dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
                
                debug_usart_tx_buff_tail = debug_usart_tx_buff_head;
            }
        }
        else if(debug_usart_tx_buff_head < debug_usart_tx_buff_tail)
        {
            
            if(is_debug_usart_dma_trans_completed == TRUE)
            {
                is_debug_usart_dma_trans_completed = FALSE;
                
                dma_channel_disable(DMA1, DMA_CH7);
                DMA_CHM0ADDR(DMA1, DMA_CH7) = (uint32_t)&debug_usart_tx_buf[debug_usart_tx_buff_tail];
                DMA_CHCNT(DMA1, DMA_CH7) = DEBUG_USART_TX_BUFF_SIZE - debug_usart_tx_buff_tail;
                
                dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INTC_FTFIFC | DMA_INTC_HTFIFC);
                dma_channel_enable(DMA1, DMA_CH7);
                dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
                
                debug_usart_tx_buff_tail = 0;
            }
        }
        usart_interrupt_disable(USART0, USART_INTEN_TBEIE);
    }
    
    
    if(usart_flag_get(USART0, USART_FLAG_ORERR) == SET)
    {
        usart_data_receive(USART0);
        usart_flag_clear(USART0, USART_FLAG_ORERR);
    }
}
