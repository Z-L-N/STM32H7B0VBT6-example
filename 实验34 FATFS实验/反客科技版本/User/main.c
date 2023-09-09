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
#include "./MALLOC/malloc.h"
#include "./FATFS/exfuns/exfuns.h"
#include "sdmmc_sd.h"

#include "mpu.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "lcd_spi_200.h"
#include "mcu_monitor.h"
#include "sdmmc_sd.h"

/********************************************** �������� *******************************************/

//void SystemClock_Config(void);		// ʱ�ӳ�ʼ��

/***************************************************************************************************
*	�� �� ��: main
*
*	˵    ��: ������
*
****************************************************************************************************/
int32_t SD_Status ; 		 //SD������־λ

int main(void)
{
	uint8_t key;
	uint8_t t = 0;
	uint8_t x = 0;
	uint64_t card_capacity;       		/* SD������ */
    HAL_SD_CardCIDTypeDef sd_card_cid;
	SD_HandleTypeDef g_sd_handle; 		/* SD����� */
	HAL_SD_CardInfoTypeDef g_sd_card_info_handle;    /* SD����Ϣ�ṹ�� */
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	adc2_init();
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	
	my_mem_init(AXI_SRAM123);     /* ��ʼ���ڲ��ڴ��(AXI) */
	my_mem_init(AHB_SRAM12);      /* ��ʼ���ڲ��ڴ��(AHB) */
	my_mem_init(SDR_SRAM);        /* ��ʼ���ڲ��ڴ��(SDR) */
	my_mem_init(SRAM_DTCM);       /* ��ʼ���ڲ��ڴ��(DTCM) */
	my_mem_init(SRAM_ITCM);       /* ��ʼ���ڲ��ڴ��(ITCM) */
	
	printf("\r\n STM32H7B0 SD����дʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU");	
	
	SD_Status = BSP_SD_Init(SD_Instance);	//SD����ʼ��
	while(SD_Status!=BSP_ERROR_NONE)
	{
		//printf("��ⲻ��SD����ERROR: %d\r\n",SD_Status);
		LCD_DisplayString( 0, 16, "SD card error!!");
		delay_ms(500);
		LCD_DisplayString( 0, 16, "Please Check!!");
		delay_ms(500);
		LED0_Toggle; /* �����˸ */
	}
	
	
	if( SD_Status == BSP_ERROR_NONE )	//����Ƿ��ʼ���ɹ�
	{		
		printf("SD����ʼ���ɹ� \r\n");	
		LCD_DisplayString( 0, 16, "SD card init ok!!");
		HAL_SD_GetCardCID(&g_sd_handle, &sd_card_cid);       /* ��ȡCID */
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
		card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* ����SD������ */
		printf("Card ManufacturerID:%d\r\n", sd_card_cid.ManufacturerID);                /* ������ID */
		LCD_DisplayString( 0, 48, "Card ManufacturerID:");
		LCD_DisplayNumber( 168, 48, sd_card_cid.ManufacturerID,5) ;
		printf("Card RCA:%d\r\n", g_sd_card_info_handle.RelCardAdd);                     /* ����Ե�ַ */
		printf("LogBlockNbr:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));    /* ��ʾ�߼������� */
		printf("LogBlockSize:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));  /* ��ʾ�߼����С */
		printf("Card Capacity:%d MB\r\n", (uint32_t)(card_capacity >> 20));              /* ��ʾ���� */
		LCD_DisplayString( 0, 64, "Card Capacity:");
		card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* ����SD������ */
		LCD_DisplayNumber( 120, 64, (uint32_t)(card_capacity >> 20),5) ;
		printf("Card BlockSize:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);            /* ��ʾ���С */
	}
	while (1)
	{
		
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
