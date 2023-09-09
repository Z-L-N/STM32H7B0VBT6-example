/***
	*************************************************************************************************
	*	@file   	main.c
	*	@version  V1.0
	*  @date    2022-4-25
	*	@author   反客科技
	*	@brief    OV2640图像采集
   ************************************************************************************************
>>>>> 功能说明：
	
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
#include "dht11.h"
/********************************************** 函数声明 *******************************************/

//void SystemClock_Config(void);		// 时钟初始化

/***************************************************************************************************
*	函 数 名: main
*
*	说    明: 主程序
*
****************************************************************************************************/

int main(void)
{
	uint8_t key;
	uint8_t t = 0;
	uint8_t temperature;
	uint8_t humidity;
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD屏幕初始化
	//rtc_init();
	
	printf("\r\n STM32H7B0 RTC 实验 \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0VBT6 DHT11");
	LCD_DisplayString( 0, 40, "DHT11:");
	LCD_DisplayString( 0, 304, "Pin:PD10");
	while (dht11_init()) /* DHT11 初始化 */
	{
		 LCD_DisplayString( 48, 40, "ERROE!!");
		 delay_ms(200);
	}
	LCD_DisplayString( 48, 40, "OK!!");
	LCD_DisplayString( 48, 80, "temp:");
	LCD_DisplayString( 120, 80, "C");
	LCD_DisplayString( 48, 100, "humi:");
	LCD_DisplayString( 120, 100, "%");
	while (1)
	{
		t++;
		if ((t % 10) == 0)  /* 每100ms更新一次显示数据 */
        {
			dht11_read_data(&temperature, &humidity); /* 读取温湿度值 */
			LCD_DisplayNumber( 96, 80, temperature,2);
			LCD_DisplayNumber( 96, 100, humidity,2);
		}
		delay_ms(10);
		if ((t % 20) == 0)
        {
            LED0_Toggle;  /* 每200ms,翻转一次LED0 */
        }	
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
