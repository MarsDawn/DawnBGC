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
#define SMPLRT_DIV                               ((uint16_t)0x19)
#define CONFIG                                   ((uint16_t)0x1a)
#define GYRO_CONFIG                              ((uint16_t)0x1b)
#define ACCEL_CONFIG                             ((uint16_t)0x1c)
#define ACCEL_CONFIG2                            ((uint16_t)0x1d)

#define INT_CFG                                  ((uint16_t)0x37)
#define INT_ENABLE                               ((uint16_t)0x38)

#define ACCEL_XOUT_H                             ((uint16_t)0x3b)
#define ACCEL_XOUT_L                             ((uint16_t)0x3c)
#define ACCEL_YOUT_H                             ((uint16_t)0x3d)
#define ACCEL_YOUT_L                             ((uint16_t)0x3e)
#define ACCEL_ZOUT_H                             ((uint16_t)0x3f)
#define ACCEL_ZOUT_L                             ((uint16_t)0x40)

#define TEMP_OUT_H                               ((uint16_t)0x41)
#define TEMP_OUT_L                               ((uint16_t)0x42)

#define GyroXOUT_H                              ((uint16_t)0x43)
#define GyroXOUT_L                              ((uint16_t)0x44)
#define GyroYOUT_H                              ((uint16_t)0x45)
#define GyroYOUT_L                              ((uint16_t)0x46)
#define GyroZOUT_H                              ((uint16_t)0x47)
#define GyroZOUT_L                              ((uint16_t)0x48)

#define SIGNAL_PATH_RESET                        ((uint16_t)0x68)

#define USER_CTRL                                ((uint16_t)0x6a)
#define PWR_MGMT_1                               ((uint16_t)0x6b)
#define WHOAMI                                 ((uint16_t)0x75)
#define ICM206XX_DATA_AVRG_CNTS                   ((uint8_t)4)
#define ICM206XX_CS_HIGH()                        gpio_bit_set(GPIOD, GPIO_PIN_2)
#define ICM206XX_CS_LOW()                         gpio_bit_reset(GPIOD, GPIO_PIN_2)

Icm206XXDataStruct Icm206XXData;

volatile static uint8_t Icm206XXRegData = 0;

static uint8_t Icm206XXRxBufIndex = 0;
static uint8_t Icm206XXRxBuf[15] = {0};
static uint8_t Icm206XXTxBufGyro[7] =
{
    0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
static uint8_t Icm206XXTxBufGyroTemp[15] =
{
    0xbb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static uint8_t SPI0ReadWriteBytes(uint8_t TxData)
{
    uint16_t i = 0;
    
    while(spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET)
    {
        i++;
        if(i > 5000)
        {
            return 0;
        }
    };
    spi_i2s_data_transmit(SPI0, TxData);
        
    i = 0;
    while(spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET)
    {
        i++;
        if(i > 5000)
        {
            return 0;
        }
    };

    return spi_i2s_data_receive(SPI0);
}

static void Icm206XXWriteByte(uint8_t add, uint8_t TxData)
{
    ICM206XX_CS_LOW();
    SPI0ReadWriteBytes(add);
    SPI0ReadWriteBytes(TxData);
    ICM206XX_CS_HIGH();
}

static uint8_t Icm206XXReadByte(uint8_t add)
{
    uint8_t data = 0;
    
    ICM206XX_CS_LOW();
    SPI0ReadWriteBytes(0x80|add);
    data = SPI0ReadWriteBytes(0xff);
    ICM206XX_CS_HIGH();

    return data;
}

uint8_t icA206xx_debug_data = 0;
void Icm206XXInit(void)
{
    timer_parameter_struct timer_initpara;
    spi_parameter_struct spi_init_struct;
    dma_single_data_parameter_struct dma_init_struct;
    
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_SPI0);
    rcu_periph_clock_enable(RCU_DMA1);
    rcu_periph_clock_enable(RCU_TIMER12);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    
    
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_3|GPIO_PIN_4| GPIO_PIN_5);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3|GPIO_PIN_4| GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_3|GPIO_PIN_4| GPIO_PIN_5);
    
    
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_2);
    gpio_bit_set(GPIOD, GPIO_PIN_2);
                                
    
    gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN0);
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_0);
    nvic_irq_enable(EXTI0_IRQn, ICM206XX_INT_PRE_PRIORITY, ICM206XX_INT_SUB_PRIORITY);
    
    
    dma_deinit(DMA1, DMA_CH3);   
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory0_addr = (uint32_t)Icm206XXTxBufGyro;
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.number = 7;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH3, dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH3, DMA_SUBPERI3);
    
    
    dma_deinit(DMA1, DMA_CH2);   
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory0_addr = (uint32_t)Icm206XXRxBuf;
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.number = 7;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH2, dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH2, DMA_SUBPERI3);
    
    dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_CHXCTL_FTFIE);
    dma_interrupt_enable(DMA1, DMA_CH2, DMA_CHXCTL_FTFIE);
    nvic_irq_enable(DMA1_Channel2_IRQn, DMA_RX_PRE_PRIORITY, DMA_RX_SUB_PRIORITY);

    
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_256;                     
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);
    
    spi_crc_polynomial_set(SPI0,7);
    
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI0, SPI_DMA_RECEIVE);

    spi_enable(SPI0);
    
    
    Icm206XXData.WhoAmI = Icm206XXReadByte(WHOAMI);
    systick_dly_ms(10);
    
    Icm206XXWriteByte(PWR_MGMT_1, 0x80);
    systick_dly_ms(100);
    Icm206XXWriteByte(PWR_MGMT_1, 0x00);
    systick_dly_ms(100);
    

    Icm206XXWriteByte(GYRO_CONFIG, 0x12);      
    systick_dly_ms(100);
    Icm206XXRegData = Icm206XXReadByte(GYRO_CONFIG);
    systick_dly_ms(10);
    
    Icm206XXWriteByte(ACCEL_CONFIG, 0x08);    
    systick_dly_ms(100);
    Icm206XXRegData = Icm206XXReadByte(ACCEL_CONFIG);
    systick_dly_ms(10);
    
    Icm206XXWriteByte(ACCEL_CONFIG2, 0x00);   
    systick_dly_ms(100);
    Icm206XXRegData = Icm206XXReadByte(ACCEL_CONFIG2);
    systick_dly_ms(10);
    
    
    spi_init_struct.prescale = SPI_PSC_8;                     
    spi_init(SPI0, &spi_init_struct);
    
    
    Icm206XXData.AccScale = 1.0f / 8192.0f;
    Icm206XXData.GyroScale = 1.0f / 32.8f * (PI / 180.0f);
    Icm206XXData.TempScale = 1.0f / 333.87f;
    
    
    timer_deinit(TIMER12);
    timer_initpara.prescaler = 0;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = SYSTERM_CLK / 16000 - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER12, &timer_initpara);
    
    nvic_irq_enable(TIMER7_UP_TIMER12_IRQn, ICM206XX_INT_PRE_PRIORITY, ICM206XX_INT_SUB_PRIORITY);
    timer_interrupt_enable(TIMER12, TIMER_INT_UP);
    timer_counter_value_config(TIMER12 , 0);
    timer_enable(TIMER12);
}

void TIMER7_UP_TIMER12_IRQHandler(void)
{
    timer_interrupt_flag_clear(TIMER12, TIMER_INT_UP);
    
    if(DMA_CHCNT(DMA1, DMA_CH2) != DMA_CHCNT(DMA1, DMA_CH3))
    {
        return;
    }
    
    ICM206XX_CS_LOW();

    dma_channel_disable(DMA1, DMA_CH3);
    dma_channel_disable(DMA1, DMA_CH2);

   if(Icm206XXRxBufIndex == (ICM206XX_DATA_AVRG_CNTS - 1))
    {
        DMA_CHM0ADDR(DMA1, DMA_CH2) = (uint32_t)Icm206XXRxBuf;
        DMA_CHCNT(DMA1, DMA_CH2) = 15;

        DMA_CHM0ADDR(DMA1, DMA_CH3) = (uint32_t)Icm206XXTxBufGyroTemp;
        DMA_CHCNT(DMA1, DMA_CH3) = 15;
    }
    else
    {
        DMA_CHM0ADDR(DMA1, DMA_CH2) = (uint32_t)Icm206XXRxBuf;
        DMA_CHCNT(DMA1, DMA_CH2) = 7;

        DMA_CHM0ADDR(DMA1, DMA_CH3) = (uint32_t)Icm206XXTxBufGyro;
        DMA_CHCNT(DMA1, DMA_CH3) = 7;
    }

    dma_channel_enable(DMA1, DMA_CH3);
    dma_channel_enable(DMA1, DMA_CH2);
}

void DMA1_Channel2_IRQHandler(void)
{
    static int32_t GyroXSum = 0, GyroYSum = 0, GyroZSum = 0;
    int16_t GyroX = 0, GyroY = 0, GyroZ = 0;
    
    if(dma_interrupt_flag_get(DMA1, DMA_CH2, DMA_INTF_FTFIF) == SET)
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_INTC_FTFIFC);
        dma_interrupt_flag_clear(DMA1, DMA_CH3, DMA_INTC_FTFIFC);

        Icm206XXRxBufIndex++;
        
        if(Icm206XXRxBufIndex < ICM206XX_DATA_AVRG_CNTS)
        {
            GyroX = (Icm206XXRxBuf[10]<<8) + Icm206XXRxBuf[11];
            GyroY = (Icm206XXRxBuf[12]<<8) + Icm206XXRxBuf[13];
            GyroZ = (Icm206XXRxBuf[14]<<8) + Icm206XXRxBuf[0];
            
            GyroXSum += GyroX;
            GyroYSum += GyroY;
            GyroZSum += GyroZ;
        }
        else
        {
            Icm206XXRxBufIndex = 0;
            
            GyroX = (Icm206XXRxBuf[10]<<8) + Icm206XXRxBuf[11];
            GyroY = (Icm206XXRxBuf[12]<<8) + Icm206XXRxBuf[13];
            GyroZ = (Icm206XXRxBuf[14]<<8) + Icm206XXRxBuf[0];
            
            GyroXSum += GyroX;
            GyroYSum += GyroY;
            GyroZSum += GyroZ;
            
            Icm206XXData.HGyroX = GyroXSum / ICM206XX_DATA_AVRG_CNTS;
            Icm206XXData.HGyroY = GyroYSum / ICM206XX_DATA_AVRG_CNTS;
            Icm206XXData.HGyroZ = GyroZSum / ICM206XX_DATA_AVRG_CNTS;
            
            Icm206XXData.HTemp = (Icm206XXRxBuf[8]<<8) + Icm206XXRxBuf[9];
            
            Icm206XXData.HAccX = (Icm206XXRxBuf[2]<<8) + Icm206XXRxBuf[3];
            Icm206XXData.HAccY = (Icm206XXRxBuf[4]<<8) + Icm206XXRxBuf[5];
            Icm206XXData.HAccZ = (Icm206XXRxBuf[6]<<8) + Icm206XXRxBuf[7];
            
            GyroXSum = 0;
            GyroYSum = 0;
            GyroZSum = 0;
            
            
            Icm206XXData.FAccX = (Icm206XXData.HAccX) * (Icm206XXData.AccScale);
            Icm206XXData.FAccY = (Icm206XXData.HAccY) * (Icm206XXData.AccScale);
            Icm206XXData.FAccZ = (Icm206XXData.HAccZ) * (Icm206XXData.AccScale);
            
            Icm206XXData.FTemp = 21.0f + (Icm206XXData.HTemp) * (Icm206XXData.TempScale);
            
            Icm206XXData.FGyroX = (Icm206XXData.HGyroX) * (Icm206XXData.GyroScale);
            Icm206XXData.FGyroY = (Icm206XXData.HGyroY) * (Icm206XXData.GyroScale);
            Icm206XXData.FGyroZ = (Icm206XXData.HGyroZ) * (Icm206XXData.GyroScale);
            
            exti_software_interrupt_enable(EXTI_0);
        }

        ICM206XX_CS_HIGH();
    }
}

void EXTI0_IRQHandler(void)
{
    exti_interrupt_flag_clear(EXTI_0);

    GimbalWork();
    
    
    
}
