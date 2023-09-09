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
#include "mcu_monitor.h"
#include "sdmmc_sd.h"
/********************************************** 函数声明 *******************************************/

//void SystemClock_Config(void);		// 时钟初始化

/***************************************************************************************************
*	函 数 名: main
*
*	说    明: 主程序
*
****************************************************************************************************/
int32_t SD_Status ; 		 //SD卡检测标志位

int main(void)
{
	uint8_t key;
	uint8_t t = 0;
	uint8_t x = 0;
	uint64_t card_capacity;       		/* SD卡容量 */
    HAL_SD_CardCIDTypeDef sd_card_cid;
	SD_HandleTypeDef g_sd_handle; 		/* SD卡句柄 */
	HAL_SD_CardInfoTypeDef g_sd_card_info_handle;    /* SD卡信息结构体 */
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	adc2_init();
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD屏幕初始化
	
	printf("\r\n STM32H7B0 SD卡读写实验 \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU");	
	
	SD_Status = BSP_SD_Init(SD_Instance);	//SD卡初始化
	while(SD_Status!=BSP_ERROR_NONE)
	{
		//printf("检测不到SD卡，ERROR: %d\r\n",SD_Status);
		LCD_DisplayString( 0, 16, "SD card error!!");
		delay_ms(500);
		LCD_DisplayString( 0, 16, "Please Check!!");
		delay_ms(500);
		LED0_Toggle; /* 红灯闪烁 */
	}
	
	
	if( SD_Status == BSP_ERROR_NONE )	//检测是否初始化成功
	{		
		printf("SD卡初始化成功 \r\n");	
		LCD_DisplayString( 0, 16, "SD card init ok!!");
		HAL_SD_GetCardCID(&g_sd_handle, &sd_card_cid);       /* 获取CID */
		BSP_SD_GetCardInfo(SD_Instance, &g_sd_card_info_handle);
		switch (g_sd_card_info_handle.CardType)
		{
			case CARD_SDSC:
			{
				if (g_sd_card_info_handle.CardVersion == CARD_V1_X)
				{
					printf("Card Type:SDSC V1\r\n");
					LCD_DisplayString( 0, 32, "Card Type:SDSC V1");
				}
				else if (g_sd_card_info_handle.CardVersion == CARD_V2_X)
				{
					printf("Card Type:SDSC V2\r\n");
					LCD_DisplayString( 0, 32, "Card Type:SDSC V2");
				}
			}
				break;
			
			case CARD_SDHC_SDXC:
			{
				printf("Card Type:SDHC\r\n");
				LCD_DisplayString( 0, 32, "Card Type:SDHC");
			}
				break;
		}
		card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* 计算SD卡容量 */
		printf("Card ManufacturerID:%d\r\n", sd_card_cid.ManufacturerID);                /* 制造商ID */
		LCD_DisplayString( 0, 48, "Card ManufacturerID:");
		LCD_DisplayNumber( 168, 48, sd_card_cid.ManufacturerID,5) ;
		printf("Card RCA:%d\r\n", g_sd_card_info_handle.RelCardAdd);                     /* 卡相对地址 */
		printf("LogBlockNbr:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));    /* 显示逻辑块数量 */
		printf("LogBlockSize:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));  /* 显示逻辑块大小 */
		printf("Card Capacity:%d MB\r\n", (uint32_t)(card_capacity >> 20));              /* 显示容量 */
		LCD_DisplayString( 0, 64, "Card Capacity:");
		card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* 计算SD卡容量 */
		LCD_DisplayNumber( 120, 64, (uint32_t)(card_capacity >> 20),5) ;
		printf("Card BlockSize:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);            /* 显示块大小 */
	}
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
