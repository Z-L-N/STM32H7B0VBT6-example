/**
 ****************************************************************************************************
 * @file        adc3.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-28
 * @brief       ADC3(开启内部温度传感器) 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200328
 * 第一次发布
 ****************************************************************************************************
 */

#ifndef __MCU_MONITOR_H
#define __MCU_MONITOR_H

#include "./SYSTEM/sys/sys.h"


/****************************************************************************************************/
/* ADC3温度传感器通道 定义 */ 

#define ADC2_TEMPSENSOR_CHX             ADC_CHANNEL_TEMPSENSOR

/****************************************************************************************************/


void adc2_init(void);                                                                       /* ADC3初始化 */
uint32_t adc2_get_result(ADC_HandleTypeDef adc_handle, uint32_t ch);                        /* 获得某个通道值  */
uint32_t adc2_get_result_average(ADC_HandleTypeDef adc_handle, uint32_t ch, uint8_t times); /* 得到某个通道给定次数采样的平均值 */

short adc2_get_temperature(void);                                                           /* ADC3 获取内部温度传感器的温度值 */

#endif











