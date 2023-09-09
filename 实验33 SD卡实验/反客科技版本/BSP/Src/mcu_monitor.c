/**
 ****************************************************************************************************
 * @file        adc3.c
 * @author      
 * @version     V2.0
 * @date        2023-09-02
 * @brief       ADC3(�����ڲ��¶ȴ�����) ��������
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 
 * �޸�˵��
 * V2.0 20230902
 * ��һ�η���
 ****************************************************************************************************
 */

#include "mcu_monitor.h"
#include "./SYSTEM/delay/delay.h"
#include "lcd_spi_200.h"

ADC_HandleTypeDef g_adc2_handle;  /* ADC��� */

/**
 * @brief       ADC2��ʼ������
 *   @note      ������ר����֧��ADC3, ��ADC1/2���ֿ���, ����ʹ��
 *              ����ʹ��16λ����, ADC����ʱ��=32M, ת��ʱ��Ϊ:�������� + 8.5��ADC����
 *              ��������������: 810.5, ��ת��ʱ�� = 819��ADC���� = 25.6us
 * @param       ��
 * @retval      ��
 */
void adc2_init(void)
{
    g_adc2_handle.Instance = ADC2;                                                   /* ѡ���ĸ�ADC */
    g_adc2_handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;                        /* ����ʱ��2��Ƶ,��adc_ker_ck=per_ck/2=32Mhz */
    g_adc2_handle.Init.Resolution = ADC_RESOLUTION_16B;                              /* 16λģʽ  */
    g_adc2_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;                              /* ��ɨ��ģʽ�����õ�һ��ͨ�� */
    g_adc2_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                           /* �ر�EOC�ж� */
    g_adc2_handle.Init.LowPowerAutoWait = DISABLE;                                   /* �Զ��͹��Ĺر� */
    g_adc2_handle.Init.ContinuousConvMode = DISABLE;                                 /* �ر�����ת��ģʽ */
    g_adc2_handle.Init.NbrOfConversion = 1;                                          /* ��ֵ��Χ��1~16����ʵ���õ�1��ͨ�� */
    g_adc2_handle.Init.DiscontinuousConvMode = DISABLE;                              /* ��ֹ����ת���鲻��������ģʽ */
    g_adc2_handle.Init.NbrOfDiscConversion = 0;                                      /* ���ò���������ģʽ��ͨ��������ֹ����ת���鲻��������ģʽ�󣬴˲������� */
    g_adc2_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                        /* ������� */
    g_adc2_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;         /* ������������Ļ�����λ���� */
    g_adc2_handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;             /* ����ͨ�������ݽ���������DR�Ĵ������� */
    g_adc2_handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                           /* ���µ����ݺ�ֱ�Ӹ��ǵ������� */
    g_adc2_handle.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;                         /* ����ADCת�����������λ�� */
    g_adc2_handle.Init.OversamplingMode = DISABLE;                                   /* �رչ����� */
    HAL_ADC_Init(&g_adc2_handle);                                                    /* ��ʼ�� */

    HAL_ADCEx_Calibration_Start(&g_adc2_handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED); /* ADCУ׼ */
}

/**
 * @brief       ADC�ײ�����,ʱ��ʹ��
 * @param       hadc:ADC���
 * @note        �˺����ᱻHAL_ADC_Init()����
 * @retval      ��
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    RCC_PeriphCLKInitTypeDef rcc_periph_clk_struct = {0};

    __HAL_RCC_ADC12_CLK_ENABLE();            /* ʹ��ADC3ʱ�� */

    rcc_periph_clk_struct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    rcc_periph_clk_struct.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periph_clk_struct);
}

/**
 * @brief       ���ADCת����Ľ��
 * @param       ch: ͨ����, ADC_CHANNEL_0~ADC_CHANNEL_19
 * @retval      ��
 */
uint32_t adc2_get_result(ADC_HandleTypeDef adc_handle, uint32_t ch)
{
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    adc_ch_conf.Channel = ch;                              /* ͨ�� */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                 /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_810CYCLES_5; /* ����ʱ�� */
    adc_ch_conf.SingleDiff = ADC_SINGLE_ENDED;             /* ���߲ɼ� */
    adc_ch_conf.OffsetNumber = ADC_OFFSET_NONE;            /* ��ʹ��ƫ������ͨ�� */
    adc_ch_conf.Offset=0;                                  /* ƫ����Ϊ0 */
    HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);      /* ͨ������ */

    HAL_ADC_Start(&adc_handle);                            /* ����ADC */

    HAL_ADC_PollForConversion(&adc_handle, 10);            /* ��ѯת�� */
    return HAL_ADC_GetValue(&adc_handle);                  /* �������һ��ADC�������ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~19
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc2_get_result_average(ADC_HandleTypeDef adc_handle, uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)   /* ��ȡtimes������ */
    {
        temp_val += adc2_get_result(adc_handle, ch);
        delay_ms(5);
    }

    return temp_val / times;      /* ����ƽ��ֵ */
}

/**
 * @brief       ��ȡ�ڲ��¶ȴ������¶�ֵ
 * @param       ��
 * @retval      �¶�ֵ(������100��,��λ:��.)
 */
short adc2_get_temperature(void)
{
    uint32_t adcx;
    short result;
    double temperature;
    float temp = 0;
    uint32_t ts_cal1, ts_cal2;
    
    ts_cal1 = *(volatile uint16_t *)(0X08FFF814);          /* ��ȡTS_CAL1 */
    ts_cal2 = *(volatile uint16_t *)(0X08FFF818);          /* ��ȡTS_CAL2 */
    temp = (float)((110.0 - 30.0) / (ts_cal2 - ts_cal1));  /* ��ȡ�������� */

    adcx = adc2_get_result_average(g_adc2_handle, ADC2_TEMPSENSOR_CHX, 10); /* ��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ�� */
    temperature = (float)(temp * (adcx - ts_cal1) + 30);   /* �����¶� */
	
	LCD_DisplayString( 10, 32, "ts_cal1:");
	LCD_DisplayNumber( 80, 32, ts_cal1, 4);
	LCD_DisplayString( 10, 48, "ts_cal2:");
	LCD_DisplayNumber( 80, 48, ts_cal2, 4);
	LCD_DisplayString( 10, 72, "ADC:");
	LCD_DisplayNumber( 80, 72, adcx, 5);
	
	LCD_DisplayString( 184, 0, "T:");
	LCD_DisplayDecimals( 200, 0, temperature, 5, 2);
	
//    result = temperature *= 100;                           /* ����100��. */
//    return result;
}
















