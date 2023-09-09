/***
	*************************************************************************************************
	*	@file   	main.c
	*	@version  V1.0
	*  @date    2022-4-25
	*	@author   ���ͿƼ�
	*	@brief    OV2640ͼ��ɼ�
   ************************************************************************************************
>>>>> ����˵����
	
	************************************************************************************************
***/
#include "stdio.h"
#include "string.h"

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

#include "mpu.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "lcd_spi_200.h"
#include "rtc.h"

/********************************************** �������� *******************************************/

//void SystemClock_Config(void);		// ʱ�ӳ�ʼ��

/***************************************************************************************************
*	�� �� ��: main
*
*	˵    ��: ������
*
****************************************************************************************************/

int main(void)
{
	uint8_t key;
	uint8_t t = 0;
	uint8_t hour, min, sec, ampm;
	uint8_t year, month, date, week;
	uint8_t tbuf[40];
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	rtc_init();
	
	printf("\r\n STM32H7B0 RTC ʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0VBT6 RTC");
	LCD_DisplayString( 0, 40, "RTC:");
	
	while (1)
	{
		t++;
		if ((t % 10) == 0)  /* ÿ100ms����һ����ʾ���� */
        {
            rtc_get_time(&hour, &min, &sec, &ampm);
			sprintf((char *)tbuf, "Time:%02d:%02d:%02d", hour, min, sec);
			LCD_DisplayString( 0, 60, tbuf);
			rtc_get_date(&year, &month, &date, &week);
			sprintf((char *)tbuf, "Date:20%02d-%02d-%02d", year, month, date);
			LCD_DisplayString( 0, 80, tbuf);
			sprintf((char *)tbuf, "Week:%d", week);
			LCD_DisplayString( 0, 100, tbuf);
		}
		 if ((t % 20) == 0)
        {
            LED0_Toggle;  /* ÿ200ms,��תһ��LED0 */
        }
        delay_ms(10);	
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
