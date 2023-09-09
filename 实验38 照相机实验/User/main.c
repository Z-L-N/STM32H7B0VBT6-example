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
#include "dcmi_ov2640.h"

#define Camera_Buffer	0x24000000    // ����ͷͼ�񻺳���

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
	
	mpu_set_protection(0x24000000, MPU_REGION_SIZE_512KB,MPU_REGION_NUMBER0, MPU_INSTRUCTION_ACCESS_ENABLE,
                       MPU_REGION_FULL_ACCESS, MPU_ACCESS_SHAREABLE, MPU_ACCESS_CACHEABLE, MPU_ACCESS_BUFFERABLE);
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	DCMI_OV2640_Init();     // DCMI�Լ�OV2640��ʼ��
	
	printf("\r\n STM32H7B0 OV2640 ʵ�� \r\n");
	OV2640_DMA_Transmit_Continuous(Camera_Buffer, OV2640_BufferSize);  // ����DMA��������
//	LCD_DisplayString( 0, 0, "STM32H7B0VBT6 RTC");
//	LCD_DisplayString( 0, 40, "RTC:");
	
	while (1)
	{
		if (DCMI_FrameState == 1)	// �ɼ�����һ֡ͼ��
		{		
  			DCMI_FrameState = 0;		// �����־λ

			LCD_CopyBuffer(0,0,Display_Width,Display_Height, (uint16_t *)Camera_Buffer);	// ��ͼ�����ݸ��Ƶ���Ļ
			
			LCD_DisplayString( 180 ,0,"FPS:");
			LCD_DisplayNumber( 216,0, OV2640_FPS,2) ;	// ��ʾ֡��	
			LED0_Toggle;				
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
