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
float CAPTURE_PWM_TIME_UP=0;
float CAPTURE_PWM_TIME_ALL=0;
float CAPTURE_PWM_TIME_LAST=0;
uint32_t AdcDate [5] = {0};

void BLDCGpioInit(void)
{
    dma_single_data_parameter_struct DmaSingleDataParameter;
    timer_parameter_struct TimerInitpara;
    timer_oc_parameter_struct TimerTcintpara;
    timer_break_parameter_struct TimerTreakpara;
    
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_SYSCFG);
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_periph_clock_enable(RCU_ADC1);
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
    rcu_periph_clock_enable(RCU_DMA1);
    
    rcu_periph_clock_enable(RCU_TIMER0);
    rcu_periph_clock_enable(RCU_TIMER1);
    rcu_periph_clock_enable(RCU_TIMER7);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_15);

    
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_8);          
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);          
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);         
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_8);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_9);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_10);
    
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_8);          
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_9);          
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_10);         
    
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_9);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_10);
    
    gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_6);          
    gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_7);          
    gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_8);          
    
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_8);
        
    
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);         
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);         
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_2);         
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3);         
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);         
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5);         

	
    gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);         
    gpio_mode_set(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);         
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_6);         
   
    
    dma_deinit(DMA1,DMA_CH0);
    DmaSingleDataParameter.periph_addr = (uint32_t)(&ADC_SYNCDATA);
    DmaSingleDataParameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    DmaSingleDataParameter.memory0_addr = (uint32_t)(&AdcDate );
    DmaSingleDataParameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    DmaSingleDataParameter.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    DmaSingleDataParameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
    DmaSingleDataParameter.direction = DMA_PERIPH_TO_MEMORY;
    DmaSingleDataParameter.number = 5;
    DmaSingleDataParameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH0, DmaSingleDataParameter);
    dma_channel_enable(DMA1, DMA_CH0);

    
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 5);
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 5);
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_3, ADC_SAMPLETIME_84);
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_2, ADC_SAMPLETIME_84);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_1, ADC_SAMPLETIME_84);
    adc_regular_channel_config(ADC1, 1, ADC_CHANNEL_0, ADC_SAMPLETIME_84);
    adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_5, ADC_SAMPLETIME_84);
    adc_regular_channel_config(ADC1, 2, ADC_CHANNEL_4, ADC_SAMPLETIME_84);
	
	adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_8, ADC_SAMPLETIME_84);
    adc_regular_channel_config(ADC1, 3, ADC_CHANNEL_9, ADC_SAMPLETIME_84);
	adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_6, ADC_SAMPLETIME_84);

    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE);
    adc_sync_mode_config(ADC_DAUL_REGULAL_PARALLEL);
    adc_sync_delay_config(ADC_SYNC_DELAY_5CYCLE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    adc_enable(ADC0);
    adc_enable(ADC1);
    adc_calibration_enable(ADC0);
    adc_calibration_enable(ADC1);
    
    adc_dma_mode_enable(ADC0);
    adc_dma_mode_enable(ADC1);
    adc_sync_dma_config(ADC_SYNC_DMA_MODE1);    
    adc_sync_dma_request_after_last_enable();
    
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    
    
    timer_deinit(TIMER0);
    timer_deinit(TIMER1);
    timer_deinit(TIMER7);
    TimerInitpara.prescaler = 0;
    TimerInitpara.alignedmode = TIMER_COUNTER_CENTER_DOWN;
    TimerInitpara.counterdirection = TIMER_COUNTER_UP;
    TimerInitpara.period = HALF_PWM_PERIOD - 1;
    TimerInitpara.clockdivision = TIMER_CKDIV_DIV2;       
    TimerInitpara.repetitioncounter = 0;
    timer_init(TIMER0, &TimerInitpara);
    timer_init(TIMER1, &TimerInitpara);
    timer_init(TIMER7, &TimerInitpara);
    
    TimerTcintpara.outputstate = TIMER_CCX_ENABLE;
    TimerTcintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    TimerTcintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    TimerTcintpara.outputnstate = TIMER_CCX_DISABLE;
    TimerTcintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    TimerTcintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    
    timer_channel_output_config(TIMER0, TIMER_CH_0, &TimerTcintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_1, &TimerTcintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_2, &TimerTcintpara);
    timer_channel_output_config(TIMER1, TIMER_CH_0, &TimerTcintpara);
    timer_channel_output_config(TIMER1, TIMER_CH_1, &TimerTcintpara);
    timer_channel_output_config(TIMER1, TIMER_CH_2, &TimerTcintpara);
    timer_channel_output_config(TIMER7, TIMER_CH_0, &TimerTcintpara);
    timer_channel_output_config(TIMER7, TIMER_CH_1, &TimerTcintpara);
    timer_channel_output_config(TIMER7, TIMER_CH_2, &TimerTcintpara);
    
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_0, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_1, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_1, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, HALF_PWM_PERIOD * 0.5f);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_2, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);
    
    timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER1, TIMER_CH_0, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER1, TIMER_CH_1, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER1, TIMER_CH_2, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER7, TIMER_CH_0, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER7, TIMER_CH_1, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER7, TIMER_CH_2, TIMER_CCX_DISABLE);
    
    TimerTreakpara.runoffstate = TIMER_ROS_STATE_ENABLE;
    TimerTreakpara.ideloffstate = TIMER_IOS_STATE_ENABLE ;
    TimerTreakpara.deadtime = 0;
    TimerTreakpara.breakpolarity = TIMER_BREAK_POLARITY_LOW;
    TimerTreakpara.outputautostate = TIMER_OUTAUTO_DISABLE;
    TimerTreakpara.protectmode = TIMER_CCHP_PROT_0;
    TimerTreakpara.breakstate = TIMER_BREAK_DISABLE;
    timer_break_config(TIMER0, &TimerTreakpara);
	timer_break_config(TIMER7, &TimerTreakpara);
    timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_REPETITIVE);
    timer_auto_reload_shadow_enable(TIMER0);
    timer_single_pulse_mode_config(TIMER1, TIMER_SP_MODE_REPETITIVE);
    timer_auto_reload_shadow_enable(TIMER1);
    timer_single_pulse_mode_config(TIMER7, TIMER_SP_MODE_REPETITIVE);
    timer_auto_reload_shadow_enable(TIMER7);
    
    timer_primary_output_config(TIMER0, ENABLE);
	timer_primary_output_config(TIMER7, ENABLE);
    timer_counter_value_config(TIMER0, 0);
    timer_counter_value_config(TIMER1, 0);
    timer_counter_value_config(TIMER7, 0);
    timer_enable(TIMER0);
    timer_enable(TIMER1);
    timer_enable(TIMER7);
}
