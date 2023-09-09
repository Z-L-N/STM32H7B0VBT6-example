/**
 ****************************************************************************************************
 * @file        adc3.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-28
 * @brief       ADC3(�����ڲ��¶ȴ�����) ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200328
 * ��һ�η���
 ****************************************************************************************************
 */

#ifndef __MCU_MONITOR_H
#define __MCU_MONITOR_H

#include "./SYSTEM/sys/sys.h"


/****************************************************************************************************/
/* ADC3�¶ȴ�����ͨ�� ���� */ 

#define ADC2_TEMPSENSOR_CHX             ADC_CHANNEL_TEMPSENSOR

/****************************************************************************************************/


void adc2_init(void);                                                                       /* ADC3��ʼ�� */
uint32_t adc2_get_result(ADC_HandleTypeDef adc_handle, uint32_t ch);                        /* ���ĳ��ͨ��ֵ  */
uint32_t adc2_get_result_average(ADC_HandleTypeDef adc_handle, uint32_t ch, uint8_t times); /* �õ�ĳ��ͨ����������������ƽ��ֵ */

short adc2_get_temperature(void);                                                           /* ADC3 ��ȡ�ڲ��¶ȴ��������¶�ֵ */

#endif











