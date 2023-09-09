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

#include "led.h"
#include "beep.h"
#include "key.h"

//#include "usart.h"
//#include "lcd_spi_200.h"
//#include "dcmi_ov2640.h"  

/********************************************** �������� *******************************************/

//void SystemClock_Config(void);		// ʱ�ӳ�ʼ��
void MPU_Config(void);					// MPU����

/***************************************************************************************************
*	�� �� ��: main
*
*	˵    ��: ������
*
****************************************************************************************************/

int main(void)
{
	uint8_t key;
	uint8_t len;
	uint8_t str1[4]="led";
	uint8_t str2[5]="beep";
	uint16_t times=0;
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	LED_Init();
	key_init();   
	
	printf("\r\n STM32H7B0 ����ʵ�� \r\n");
	printf("\r\n printf�������� \r\n");	
	
	while (1)
	{
		if (g_usart1_rx_sta & 0x8000) /* ���յ������� */
		{
			 len = g_usart1_rx_sta & 0x3fff; /* �õ��˴ν��յ������ݳ��� */
			 printf("\r\n �����͵���ϢΪ:\r\n");
			/*���ͽ��յ�������*/
			if(NULL != strstr(g_usart1_rx_buf, str2))
			{
				LED0_Toggle;
			}
			if(NULL != strstr(g_usart1_rx_buf, str1))
			{
				BEEP_TOGGLE();
			}
			 HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart1_rx_buf,len,1000);
			/*�ȴ����ͽ���*/
			 while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET); 
			 printf("\r\n\r\n"); /* ���뻻�� */
			 memset(g_usart1_rx_buf, 0 ,sizeof(g_usart1_rx_buf));
			 g_usart1_rx_sta = 0;
		}
		else
        {
            times++;
            if (times % 200 == 0) 
				printf("����������,�Իس�������\r\n");

            if (times % 30  == 0) 
				LED0_Toggle; /* ��˸LED,��ʾϵͳ��������. */

            delay_ms(10);
        }
		
//		LED0_Toggle;
//		delay_ms(2000);
		
//		key = key_scan(0); /* �õ���ֵ */
//		if (key)
//		{
//			 switch (key)
//			 {
//			 case WKUP_PRES: /* ���� LED0(RED)��ת */
//						LED0_Toggle; /* LED0 ״̬ȡ�� */
//			 break;
//			 case KEY1_PRES: /* ���� LED1(GREEN)��ת */
//						BEEP_TOGGLE(); /* LED1 ״̬ȡ�� */
//			 break;
//			 case KEY0_PRES: /* ���� LED2(BLUE)��ת */
//						LED0_Toggle; /* LED2 ״̬ȡ�� */
//			 break;
//			 }
//	  }
//		else
//		{
//			 delay_ms(10);
//		}
	}
}

//	����MPU
//
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	HAL_MPU_Disable();		// �Ƚ�ֹMPU

	MPU_InitStruct.Enable 				= MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress 		= 0x24000000;
	MPU_InitStruct.Size 					= MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission 	= MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable 		= MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable 		= MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable 		= MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number 				= MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField 		= MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable 	= 0x00;
	MPU_InitStruct.DisableExec 		= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);	

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);	// ʹ��MPU
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
