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

#if !(__ARMCC_VERSION >= 6010050)   /* 不是AC6编译器，即使用AC5编译器时 */
uint8_t mpudata[128] __attribute__((at(0X20002000)));  /* 定义一个数组 */
#else
uint8_t mpudata[128] __attribute__((section(".bss.ARM.__at_0X20002000"))); /* 定义一个数组 */
#endif

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
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	LED_Init();
	key_init();   
	
	printf("\r\n\r\nMPU closed!\r\n");  /* 提示MPU关闭 */
	printf("\r\n STM32H7B0 保护内存（MPU）实验 \r\n");
	
	while (1)
	{
		key = key_scan(0);

        if (key == WKUP_PRES)           /* 使能MPU保护数组 mpudata */
        {
            mpu_set_protection(0X20002000, MPU_REGION_SIZE_128B, MPU_REGION_NUMBER0, MPU_REGION_FULL_ACCESS, MPU_REGION_PRIV_RO_URO,
                               MPU_ACCESS_NOT_SHAREABLE, MPU_ACCESS_NOT_CACHEABLE, MPU_ACCESS_BUFFERABLE);  /* 只读,禁止共用,禁止catch,允许缓冲 */
            printf("MPU open!\r\n");    /* 提示MPU打开 */
        }
        else if (key == KEY0_PRES)      /* 向数组中写入数据，如果开启了MPU保护的话会进入内存访问错误！ */
        {
            printf("Start Writing data...\r\n");
            sprintf((char *)mpudata, "MPU test array %d", t);
            printf("Data Write finshed!\r\n");
        }
		else if (key == KEY1_PRES)      /* 从数组中读取数据，不管有没有开启MPU保护都不会进入内存访问错误！ */
        {
            printf("Array data is:%s\r\n", mpudata);
        }
        else 
        {
            delay_ms(10);
        }

        t++;

        if ((t % 50) == 0) 
			LED0_Toggle;   /* LED0取反 */
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
