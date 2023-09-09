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

#if !(__ARMCC_VERSION >= 6010050)   /* ����AC6����������ʹ��AC5������ʱ */
uint8_t mpudata[128] __attribute__((at(0X20002000)));  /* ����һ������ */
#else
uint8_t mpudata[128] __attribute__((section(".bss.ARM.__at_0X20002000"))); /* ����һ������ */
#endif

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
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	LED_Init();
	key_init();   
	
	printf("\r\n\r\nMPU closed!\r\n");  /* ��ʾMPU�ر� */
	printf("\r\n STM32H7B0 �����ڴ棨MPU��ʵ�� \r\n");
	
	while (1)
	{
		key = key_scan(0);

        if (key == WKUP_PRES)           /* ʹ��MPU�������� mpudata */
        {
            mpu_set_protection(0X20002000, MPU_REGION_SIZE_128B, MPU_REGION_NUMBER0, MPU_REGION_FULL_ACCESS, MPU_REGION_PRIV_RO_URO,
                               MPU_ACCESS_NOT_SHAREABLE, MPU_ACCESS_NOT_CACHEABLE, MPU_ACCESS_BUFFERABLE);  /* ֻ��,��ֹ����,��ֹcatch,������ */
            printf("MPU open!\r\n");    /* ��ʾMPU�� */
        }
        else if (key == KEY0_PRES)      /* ��������д�����ݣ����������MPU�����Ļ�������ڴ���ʴ��� */
        {
            printf("Start Writing data...\r\n");
            sprintf((char *)mpudata, "MPU test array %d", t);
            printf("Data Write finshed!\r\n");
        }
		else if (key == KEY1_PRES)      /* �������ж�ȡ���ݣ�������û�п���MPU��������������ڴ���ʴ��� */
        {
            printf("Array data is:%s\r\n", mpudata);
        }
        else 
        {
            delay_ms(10);
        }

        t++;

        if ((t % 50) == 0) 
			LED0_Toggle;   /* LED0ȡ�� */
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
