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
	uint8_t x = 0;
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	
	printf("\r\n STM32H7B0 SPI LCD ʵ�� \r\n");
	
	while (1)
	{
		switch (x)
		 {
		 case 0: LCD_SetBackColor(LCD_RED); break;
		 case 1: LCD_SetBackColor(LCD_GREEN); break;
		 case 2: LCD_SetBackColor(LCD_BLUE); break;
		 case 3: LCD_SetBackColor(LCD_CYAN); break;
		 case 4: LCD_SetBackColor(LCD_MAGENTA); break;
		 case 5: LCD_SetBackColor(LCD_YELLOW); break;
		 case 6: LCD_SetBackColor(LCD_GREY); break;
		 case 7: LCD_SetBackColor(LIGHT_BLUE); break;
		 case 8: LCD_SetBackColor(LIGHT_GREEN); break;
		 case 9: LCD_SetBackColor(LIGHT_RED); break;
		 case 10: LCD_SetBackColor(DARK_CYAN); break;
		 case 11: LCD_SetBackColor(DARK_YELLOW); break;
		 }
		 LCD_Clear();	
		 LCD_DisplayString( 10, 0, "STM32");
		 LCD_DisplayString( 10, 20, "TFTLCD TEST");
		 x++;
		 if (x == 12)x = 0;
		 LED0_Toggle; /* �����˸ */
		 delay_ms(1000);
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
