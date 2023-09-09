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
#include "./MALLOC/malloc.h"

#include "mpu.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "lcd_spi_200.h"
#include "mcu_monitor.h"

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
//	uint8_t key;
//	uint8_t t = 0;
	uint8_t i = 0;
	uint8_t paddr[20];  /* 存放P Addr:+p地址的ASCII值 */
	uint8_t sramx = 0;  /* 默认为内部sram */
	uint8_t *p = 0;
	uint8_t *tp = 0;
	uint16_t memused = 0;
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD屏幕初始化
	my_mem_init(AXI_SRAM123);     /* 初始化内部内存池(AXI) */
	my_mem_init(AHB_SRAM12);      /* 初始化内部内存池(AHB) */
	my_mem_init(SDR_SRAM);        /* 初始化内部内存池(SDR) */
	my_mem_init(SRAM_DTCM);       /* 初始化内部内存池(DTCM) */
	my_mem_init(SRAM_ITCM);       /* 初始化内部内存池(ITCM) */
	
	printf("\r\n STM32H7B0 内存管理实验 \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU");	
	
	p = mymalloc(sramx, 2048);  /* 申请2K字节,并写入内容,显示在lcd屏幕上面 */
	if (p != NULL)
    {
		sprintf((char *)p, "Memory Malloc Test%03d", 20);        /* 向p写入一些内容 */
		LCD_DisplayString( 0, 16, "p:");
        LCD_DisplayString( 16, 16, p);	            
    }
	if (tp != p && p != NULL)
    {
		tp = p;
		sprintf((char *)paddr, "P Addr:0x%08X", (uint32_t)tp);
		LCD_DisplayString( 0, 32, paddr);   	/* 显示p的地址 */
		if(p)
		{
			LCD_DisplayString( 0, 48, "p:");
			LCD_DisplayString( 16, 48, p);  		/* 显示P的内容 */
		}
	}
	
	delay_ms(10);
    i++;
	memused = my_mem_perused(AXI_SRAM123);
	LCD_DisplayString( 0, 64, "AXI SRAM USED:");	
	sprintf((char *)paddr, "%d.%01d%%", memused / 10, memused % 10);
	LCD_DisplayString( 14*8, 64, paddr);   		/* 显示内部内存使用率 */
	
	myfree(sramx, p);   /* 释放内存 */
    p = 0;              /* 指向空地址 */
	while (1)
	{
		
		 LED0_Toggle; /* 红灯闪烁 */
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
