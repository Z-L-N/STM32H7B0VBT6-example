/**
 ****************************************************************************************************
 * @file        adc.h
 * @author      
 * @version     V2.0
 * @date        2023-09-03
 * @brief       ADC ��������
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨: 
 * �޸�˵��
 * V2.0 20230903
 *
 ****************************************************************************************************
 */

#include "adc.h"
#include "./SYSTEM/delay/delay.h"


ADC_HandleTypeDef g_adc_handle;   /* ADC��� */

/**
 * @brief       ADC��ʼ������
 *   @note      ������֧��ADC1/ADC2����ͨ��,���ǲ�֧��ADC3
 *              ����ʹ��16λ����, ADC����ʱ��=32M, ת��ʱ��Ϊ:�������� + 8.5��ADC����
 *              ��������������: 810.5, ��ת��ʱ�� = 819��ADC���� = 25.6us
 * @param       ��
 * @retval      ��
 */
void adc_init(void)
{
    g_adc_handle.Instance = ADC_ADCX;                                               /* ѡ���ĸ�ADC */
    g_adc_handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;                        /* ����ʱ��2��Ƶ,��adc_ker_ck=per_ck/2=32Mhz */
    g_adc_handle.Init.Resolution = ADC_RESOLUTION_16B;                              /* 16λģʽ  */
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;                              /* ��ɨ��ģʽ�����õ�һ��ͨ�� */
    g_adc_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                           /* �ر�EOC�ж� */
    g_adc_handle.Init.LowPowerAutoWait = DISABLE;                                   /* �Զ��͹��Ĺر� */
    g_adc_handle.Init.ContinuousConvMode = DISABLE;                                 /* �ر�����ת��ģʽ */
    g_adc_handle.Init.NbrOfConversion = 1;                                          /* ��ֵ��Χ��1~16����ʵ���õ�1������ͨ�� */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;                              /* ��ֹ����ת���鲻��������ģʽ */
    g_adc_handle.Init.NbrOfDiscConversion = 0;                                      /* ���ò���������ģʽ��ͨ��������ֹ����ת���鲻��������ģʽ�󣬴˲������� */
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                        /* ������� */
    g_adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;         /* ������������Ļ�����λ���� */
    g_adc_handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;             /* ����ͨ�������ݽ���������DR�Ĵ������� */
    g_adc_handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;                           /* ���µ����ݺ�ֱ�Ӹ��ǵ������� */
    g_adc_handle.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;                         /* ����ADCת�����������λ�� */
    g_adc_handle.Init.OversamplingMode = DISABLE;                                   /* �رչ����� */
    HAL_ADC_Init(&g_adc_handle);                                                    /* ��ʼ�� */

    HAL_ADCEx_Calibration_Start(&g_adc_handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED); /* ADCУ׼ */
}

/**
 * @brief       ADC�ײ��������������ã�ʱ��ʹ��
                �˺����ᱻHAL_ADC_Init()����
 * @param       hadc:ADC���
 * @retval      ��
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC_ADCX)
    {
        GPIO_InitTypeDef gpio_init_struct;

        ADC_ADCX_CHY_CLK_ENABLE();                      /* ʹ��ADC1/2ʱ�� */
        ADC_ADCX_CHY_GPIO_CLK_ENABLE();                 /* ����ADCͨ��IO����ʱ�� */

        __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);    /* ADC����ʱ��ѡ�� */

        gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;   /* ADCͨ��IO���� */
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;       /* ģ�� */
        HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);
    }
}

/**
 * @brief       ���ADCת����Ľ�� 
 * @param       ch: ͨ��ֵ 0~19��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_19
 * @retval      ����ֵ:ת�����
 */
uint32_t adc_get_result(uint32_t ch)
{
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    adc_ch_conf.Channel = ch;                              /* ͨ�� */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                 /* ���� */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_810CYCLES_5; /* ����ʱ�䣬��������������: 810.5��ADC���� */
    adc_ch_conf.SingleDiff = ADC_SINGLE_ENDED;             /* ���߲ɼ� */
    adc_ch_conf.OffsetNumber = ADC_OFFSET_NONE;            /* ��ʹ��ƫ������ͨ�� */
    adc_ch_conf.Offset = 0;                                /* ƫ����Ϊ0 */
    HAL_ADC_ConfigChannel(&g_adc_handle, &adc_ch_conf);    /* ͨ������ */

    HAL_ADC_Start(&g_adc_handle);                          /* ����ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);          /* ��ѯת�� */
    return HAL_ADC_GetValue(&g_adc_handle);                /* �������һ��ADC1�������ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~19
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++) /* ��ȡtimes������ */
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;    /* ����ƽ��ֵ */
}










