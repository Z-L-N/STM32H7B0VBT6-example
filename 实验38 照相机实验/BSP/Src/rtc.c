/**
 ****************************************************************************************************
 * @file        rtc.c
 * @author      
 * @version     V2.0
 * @date        2023-09-02
 * @brief       RTC ��������
 * @license     
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * �޸�˵��
 * V2.0 20230902
 *
 ****************************************************************************************************
 */

#include "rtc.h"
#include "led.h"
#include "lcd_spi_200.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


RTC_HandleTypeDef g_rtc_handle;  /* RTC��� */


/**
 * @brief       RTCд�������SRAM
 * @param       bkrx : �����Ĵ������,��Χ:0~31
 * @param       data : Ҫд�������,32λ����
 * @retval      ��
 */
void rtc_write_bkr(uint32_t bkrx, uint32_t data)
{
    HAL_PWR_EnableBkUpAccess(); /* ȡ��������д���� */
    HAL_RTCEx_BKUPWrite(&g_rtc_handle, bkrx, data);
}

/**
 * @brief       RTC��ȡ������SRAM
 * @param       bkrx : �����Ĵ������,��Χ:0~31 
 * @retval      ��ȡ����ֵ
 */
uint32_t rtc_read_bkr(uint32_t bkrx)
{
    uint32_t temp = 0;
    temp = HAL_RTCEx_BKUPRead(&g_rtc_handle, bkrx);
    return (uint16_t )temp;     /* ���ض�ȡ����ֵ */
}

/**
 * @brief       RTCʱ������
 * @param       hour,min,sec: Сʱ,����,���� 
 * @param       ampm        : AM/PM, 0=AM/24H; 1=PM/12H;
 * @retval      0,�ɹ�
 *              1,�����ʼ��ģʽʧ��
 */
HAL_StatusTypeDef rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm)
{
    RTC_TimeTypeDef rtc_time;

    rtc_time.Hours = hour;
    rtc_time.Minutes = min;
    rtc_time.Seconds = sec;
    rtc_time.TimeFormat = ampm;
    rtc_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtc_time.StoreOperation = RTC_STOREOPERATION_RESET;
    
    return HAL_RTC_SetTime(&g_rtc_handle, &rtc_time, RTC_FORMAT_BIN);
}

/**
 * @brief       RTC��������
 * @param       year,month,date : ��(0~99),��(1~12),��(0~31)
 * @param       week            : ����(1~7,0,�Ƿ�!)
 * @retval      0,�ɹ�
 *              1,�����ʼ��ģʽʧ��
 */
HAL_StatusTypeDef rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week)
{
    RTC_DateTypeDef rtc_date;

    rtc_date.Date = date;
    rtc_date.Month = month;
    rtc_date.WeekDay = week;
    rtc_date.Year = year;
    
    return HAL_RTC_SetDate(&g_rtc_handle, &rtc_date, RTC_FORMAT_BIN);
}

/**
 * @brief       ��ȡRTCʱ��
 * @param       *hour,*min,*sec : Сʱ,����,����
 * @param       *ampm           : AM/PM,0=AM/24H,1=PM.
 * @retval      ��
 */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm)
{
    RTC_TimeTypeDef rtc_time;

    HAL_RTC_GetTime(&g_rtc_handle, &rtc_time, RTC_FORMAT_BIN);

    *hour = rtc_time.Hours;
    *min = rtc_time.Minutes;
    *sec = rtc_time.Seconds;
    *ampm = rtc_time.TimeFormat;
}

/**
 * @brief       ��ȡRTC����
 * @param       *year,*mon,*date: ��,��,��
 * @param       *week           : ����
 * @retval      ��
 */
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week)
{
    RTC_DateTypeDef rtc_date;

    HAL_RTC_GetDate(&g_rtc_handle, &rtc_date, RTC_FORMAT_BIN);

    *year = rtc_date.Year;
    *month = rtc_date.Month;
    *date = rtc_date.Date;
    *week = rtc_date.WeekDay;
}

/* ���������ݱ� */
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

/**
 * @breif       ������������ڼ�, ���빫�����ڵõ�����(ֻ����1901-2099��)
 * @param       year,month,day������������
 * @retval      ���ں�(1~7,������1~����)
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH, yearL;
    yearH = year / 100;
    yearL = year % 100;

    /*  ���Ϊ21����,�������100 */
    if (yearH > 19)yearL += 100;

    /*  ����������ֻ��1900��֮��� */
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];

    if (yearL % 4 == 0 && month < 3)temp2--;

    temp2 %= 7;

    if (temp2 == 0)temp2 = 7;

    return temp2;
}

/**
 * @brief       RTC��ʼ��
 *   @note 
 *              Ĭ�ϳ���ʹ��LSE,��LSE����ʧ�ܺ�,�л�ΪLSI.
 *              ͨ��BKP�Ĵ���0��ֵ,�����ж�RTCʹ�õ���LSE/LSI:
 *              ��BKP0==0X5050ʱ,ʹ�õ���LSE
 *              ��BKP0==0X5051ʱ,ʹ�õ���LSI
 *              ע��:�л�LSI/LSE������ʱ��/���ڶ�ʧ,�л�������������.
 *
 * @param       ��
 * @retval      0,�ɹ�
 *              1,�����ʼ��ģʽʧ��
 */
uint8_t rtc_init(void)
{
    /* ����ǲ��ǵ�һ������ʱ�� */
    uint16_t bkpflag = 0;
    
    g_rtc_handle.Instance = RTC;
    g_rtc_handle.Init.HourFormat = RTC_HOURFORMAT_24;/* RTC����Ϊ24Сʱ��ʽ */
    g_rtc_handle.Init.AsynchPrediv = 0X7F;           /* RTC�첽��Ƶϵ��(1~0X7F) */
    g_rtc_handle.Init.SynchPrediv = 0XFF;            /* RTCͬ����Ƶϵ��(0~7FFF) */
    g_rtc_handle.Init.OutPut = RTC_OUTPUT_DISABLE;   /* ѡ�����ӵ� RTC_ALARM ����ı�־ */  
    g_rtc_handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;  /* ���� RTC_ALARM ��������� */
    g_rtc_handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;     /* ���� RTC_ALARM ���������Ϊ��©�������������� */
    
    bkpflag = rtc_read_bkr(0);  /* ��ȡBKP0��ֵ */
    LCD_DisplayString( 10, 224, "RTC BKP0:");
	LCD_DisplayNumber( 90, 224, bkpflag,5) ;
    if (HAL_RTC_Init(&g_rtc_handle) != HAL_OK)
    {
        return 1;
    }
    
    if ((bkpflag != 0X5050) && (bkpflag != 0x5051))     /* ֮ǰδ��ʼ�������������� */
    {
        rtc_set_time(16, 5, 56, RTC_HOURFORMAT12_AM);  /* ����ʱ�� ,����ʵ��ʱ���޸� */
        rtc_set_date(23, 9, 2, 6);                     /* �������� */
    }

    return 0;
}

/**
 * @brief       RTC�ײ�������ʱ������
 * @param       hrtc:RTC���
 * @note        �˺����ᱻHAL_RTC_Init()����
 * @retval      ��
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    uint16_t retry = 200;
    
    RCC_OscInitTypeDef rcc_osc_init;
    RCC_PeriphCLKInitTypeDef rcc_periphclk_init;
    
    __HAL_RCC_RTC_CLK_ENABLE();     /* ʹ��RTCʱ�� */
    HAL_PWR_EnableBkUpAccess();     /* ȡ����������д���� */
    __HAL_RCC_RTC_ENABLE();         /* RTCʱ��ʹ�� */
    
    /* ʹ�üĴ����ķ�ʽȥ���LSE�Ƿ������������ */
    RCC->BDCR |= 1 << 0;    /* �����ⲿ��������LSE */
    
    while (retry && ((RCC->BDCR & 0X02) == 0))  /* �ȴ�LSE׼���� */
    {
        retry--;
        delay_ms(5);
    }
    
    if (retry == 0)     /* LSE����ʧ�ܣ�ʹ��LSI */
    {
        rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSI;           /* ѡ��Ҫ���õ����� */
        rcc_osc_init.LSEState = RCC_LSI_ON;                             /* LSI״̬������ */
        rcc_osc_init.PLL.PLLState = RCC_PLL_NONE;                       /* PLL������ */
        HAL_RCC_OscConfig(&rcc_osc_init);                               /* �������õ�rcc_oscinitstruct */

        rcc_periphclk_init.PeriphClockSelection = RCC_PERIPHCLK_RTC;    /* ѡ��Ҫ���õ����� RTC */
        rcc_periphclk_init.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;    /* RTCʱ��Դѡ�� LSI */
        HAL_RCCEx_PeriphCLKConfig(&rcc_periphclk_init);                 /* �������õ�rcc_periphClkInitStruct */
        rtc_write_bkr(0, 0X5051);
    }
    else
    {
        rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;           /* ѡ��Ҫ���õ����� */
        rcc_osc_init.PLL.PLLState = RCC_PLL_NONE;                       /* PLL������ */
        rcc_osc_init.LSEState = RCC_LSE_ON;                             /* LSE״̬������ */
        HAL_RCC_OscConfig(&rcc_osc_init);                               /* �������õ�rcc_oscinitstruct */

        rcc_periphclk_init.PeriphClockSelection = RCC_PERIPHCLK_RTC;    /* ѡ��Ҫ�������� RTC */
        rcc_periphclk_init.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;    /* RTCʱ��ԴΪLSE */
        HAL_RCCEx_PeriphCLKConfig(&rcc_periphclk_init);                 /* �������õ�rcc_periphclkinitstruct */
        rtc_write_bkr(0, 0X5050);
    }
}

/**
 * @breif       ��������ʱ��(����������,24Сʱ��)
 * @param       week        : ���ڼ�(1~7) 
 * @param       hour,min,sec: Сʱ,����,����
 * @retval      ��
 */
void rtc_set_alarma(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
    RTC_AlarmTypeDef rtc_alarm;
    
    rtc_alarm.AlarmTime.Hours = hour;                                /* Сʱ */
    rtc_alarm.AlarmTime.Minutes = min;                               /* ���� */
    rtc_alarm.AlarmTime.Seconds = sec;                               /* �� */
    rtc_alarm.AlarmTime.SubSeconds = 0;
    rtc_alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    
    rtc_alarm.AlarmMask = RTC_ALARMMASK_NONE;                        /* ��ȷƥ�����ڣ�ʱ���� */
    rtc_alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
    rtc_alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY; /* ������ */
    rtc_alarm.AlarmDateWeekDay = week;                               /* ���� */
    rtc_alarm.Alarm = RTC_ALARM_A;                                   /* ����A */
    HAL_RTC_SetAlarm_IT(&g_rtc_handle, &rtc_alarm, RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 2);   /* ��ռ���ȼ�1,�����ȼ�2 */
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
/**
 * @breif       �����Ի��Ѷ�ʱ������
 * @param       wksel
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
 *   @arg       RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
 *   @arg       RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
 *   @arg       RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
 * @note        000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
 * @note        ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
 * @param       cnt: �Զ���װ��ֵ.����0,�����ж�.
 * @retval      ��
 */
void rtc_set_wakeup(uint8_t wksel, uint16_t cnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&g_rtc_handle, RTC_FLAG_WUTF);  /* ���RTC WAKE UP�ı�־ */

    HAL_RTCEx_SetWakeUpTimer_IT(&g_rtc_handle, cnt, wksel);          /* ������װ��ֵ��ʱ�� */

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 2, 2);                       /* ��ռ���ȼ�2,�����ȼ�2 */
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

/**
 * @breif       RTC�����жϷ�����
 * @param       ��
 * @retval      ��
 */
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&g_rtc_handle);
}

/**
 * @breif       RTC����A�жϴ���ص�����
 * @param       hrtc:RTC���
 * @retval      ��
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("ALARM A!\r\n");
}

/**
 * @breif       RTC WAKE UP�жϷ�����
 * @param       ��
 * @retval      ��
 */
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&g_rtc_handle); 
}

/**
 * @breif       RTC WAKE UP�жϴ�����ص�����
 * @param       hrtc:RTC���
 * @retval      ��
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    LED0_Toggle;
}







