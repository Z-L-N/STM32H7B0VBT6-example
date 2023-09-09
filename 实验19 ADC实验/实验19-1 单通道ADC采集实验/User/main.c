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
#include "adc.h"

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
	float t;
	uint16_t adcx;
	char str_buf[24];
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	adc_init(); /* ��ʼ�� ADC */
	
	printf("\r\n STM32H7B0 ��ͨ��ADC�ɼ�ʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0VBT6 ADC 1");
	LCD_DisplayString( 0, 20, "ADC:");
	LCD_DisplayString( 0, 40, "ADC1_CH19_VAL:");
	LCD_DisplayString( 210, 40, "%");
	sprintf(str_buf,"%x",&t);
	LCD_DisplayString( 0, 80, str_buf);
	while (1)
	{
		adcx = adc_get_result_average(ADC_ADCX_CHY, 10);
		t=adcx/65536;
		LCD_DisplayNumber( 120, 40, adcx,5);	
		LCD_DisplayDecimals( 162, 40, t,5,2);	
		
        LED0_Toggle;  /* ÿ200ms,��תһ��LED0 */ 
        delay_ms(100);	
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
