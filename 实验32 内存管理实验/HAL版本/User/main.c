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

#include "mpu.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "lcd_spi_200.h"
#include "mcu_monitor.h"

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
//	uint8_t key;
//	uint8_t t = 0;
	uint8_t i = 0;
	uint8_t paddr[20];  /* ���P Addr:+p��ַ��ASCIIֵ */
	uint8_t sramx = 0;  /* Ĭ��Ϊ�ڲ�sram */
	uint8_t *p = 0;
	uint8_t *tp = 0;
	uint16_t memused = 0;
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	my_mem_init(AXI_SRAM123);     /* ��ʼ���ڲ��ڴ��(AXI) */
	my_mem_init(AHB_SRAM12);      /* ��ʼ���ڲ��ڴ��(AHB) */
	my_mem_init(SDR_SRAM);        /* ��ʼ���ڲ��ڴ��(SDR) */
	my_mem_init(SRAM_DTCM);       /* ��ʼ���ڲ��ڴ��(DTCM) */
	my_mem_init(SRAM_ITCM);       /* ��ʼ���ڲ��ڴ��(ITCM) */
	
	printf("\r\n STM32H7B0 �ڴ����ʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU");	
	
	p = mymalloc(sramx, 2048);  /* ����2K�ֽ�,��д������,��ʾ��lcd��Ļ���� */
	if (p != NULL)
    {
		sprintf((char *)p, "Memory Malloc Test%03d", 20);        /* ��pд��һЩ���� */
		LCD_DisplayString( 0, 16, "p:");
        LCD_DisplayString( 16, 16, p);	            
    }
	if (tp != p && p != NULL)
    {
		tp = p;
		sprintf((char *)paddr, "P Addr:0x%08X", (uint32_t)tp);
		LCD_DisplayString( 0, 32, paddr);   	/* ��ʾp�ĵ�ַ */
		if(p)
		{
			LCD_DisplayString( 0, 48, "p:");
			LCD_DisplayString( 16, 48, p);  		/* ��ʾP������ */
		}
	}
	
	delay_ms(10);
    i++;
	memused = my_mem_perused(AXI_SRAM123);
	LCD_DisplayString( 0, 64, "AXI SRAM USED:");	
	sprintf((char *)paddr, "%d.%01d%%", memused / 10, memused % 10);
	LCD_DisplayString( 14*8, 64, paddr);   		/* ��ʾ�ڲ��ڴ�ʹ���� */
	
	myfree(sramx, p);   /* �ͷ��ڴ� */
    p = 0;              /* ָ��յ�ַ */
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
