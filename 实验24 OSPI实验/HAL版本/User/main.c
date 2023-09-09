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
#include "norflash.h"
#include "norflash_ex.h"

#if !(__ARMCC_VERSION >= 6010050)   /* 不是AC6编译器，即使用AC5编译器时 */
uint8_t mpudata[128] __attribute__((at(0X20002000)));  /* 定义一个数组 */
#else
uint8_t mpudata[128] __attribute__((section(".bss.ARM.__at_0X20002000"))); /* 定义一个数组 */
#endif

/* 要写入到FLASH的字符串数组 */
const uint8_t g_text_buf[] = {"MiniPRO STM32H7 QSPI TEST"};

#define TEXT_SIZE       sizeof(g_text_buf)  /* TEXT字符串长度 */

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
	uint8_t i = 0;
	uint8_t datatemp[TEXT_SIZE];
	uint32_t flashsize;
    uint16_t id = 0;
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	LED_Init();
	key_init();   
	SPI_LCD_Init();      // 液晶屏以及SPI初始化 
	
	printf("\r\n STM32H7B0 OSPI FLASH实验 \r\n");
	LCD_DisplayString( 0, 0, "STM32 QSPI TEST");
	
	id = norflash_ex_read_id(); /* 读取FLASH ID */
	while ((id == 0) || (id == 0XFFFF)) /* 检测不到FLASH芯片 */
    {
        LCD_DisplayString( 0, 16, "FLASH Check Failed!");
        delay_ms(500);
        LCD_DisplayString( 0, 32, "Please Check!");
        delay_ms(500);
        LED0_Toggle;      /* LED0闪烁 */
    }
	
	LCD_DisplayString( 0, 32, "QSPI FLASH Ready!");
	LCD_DisplayString( 0, 48, "FLASH ID:");
	LCD_DisplayNumber( 80, 48, id,5);
	
	while (1)
	{
		LED0_Toggle;   /* LED0取反 */
		delay_ms(500);
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
