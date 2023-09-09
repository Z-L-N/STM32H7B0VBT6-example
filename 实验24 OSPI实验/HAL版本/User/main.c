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
#include "norflash.h"
#include "norflash_ex.h"

#if !(__ARMCC_VERSION >= 6010050)   /* ����AC6����������ʹ��AC5������ʱ */
uint8_t mpudata[128] __attribute__((at(0X20002000)));  /* ����һ������ */
#else
uint8_t mpudata[128] __attribute__((section(".bss.ARM.__at_0X20002000"))); /* ����һ������ */
#endif

/* Ҫд�뵽FLASH���ַ������� */
const uint8_t g_text_buf[] = {"MiniPRO STM32H7 QSPI TEST"};

#define TEXT_SIZE       sizeof(g_text_buf)  /* TEXT�ַ������� */

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
	uint8_t i = 0;
	uint8_t datatemp[TEXT_SIZE];
	uint32_t flashsize;
    uint16_t id = 0;
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	LED_Init();
	key_init();   
	SPI_LCD_Init();      // Һ�����Լ�SPI��ʼ�� 
	
	printf("\r\n STM32H7B0 OSPI FLASHʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32 QSPI TEST");
	
	id = norflash_ex_read_id(); /* ��ȡFLASH ID */
	while ((id == 0) || (id == 0XFFFF)) /* ��ⲻ��FLASHоƬ */
    {
        LCD_DisplayString( 0, 16, "FLASH Check Failed!");
        delay_ms(500);
        LCD_DisplayString( 0, 32, "Please Check!");
        delay_ms(500);
        LED0_Toggle;      /* LED0��˸ */
    }
	
	LCD_DisplayString( 0, 32, "QSPI FLASH Ready!");
	LCD_DisplayString( 0, 48, "FLASH ID:");
	LCD_DisplayNumber( 80, 48, id,5);
	
	while (1)
	{
		LED0_Toggle;   /* LED0ȡ�� */
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
