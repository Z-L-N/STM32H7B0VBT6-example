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
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"

#include "led.h"
//#include "usart.h"
//#include "lcd_spi_200.h"
//#include "dcmi_ov2640.h"  

/********************************************** 函数声明 *******************************************/

//void SystemClock_Config(void);		// 时钟初始化
void MPU_Config(void);					// MPU配置

/***************************************************************************************************
*	函 数 名: main
*
*	说    明: 主程序
*
****************************************************************************************************/

int main(void)
{
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
  sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	LED_Init();
	
	while (1)
	{
		LED0_Toggle;
		delay_ms(500);
	}
}


/***************************************** SPI4内核时钟设置 **************************************************/
/*** FANKE ***
>>>>> 配置说明：
	*
	*	1.	SPI6 所允许的最大驱动时钟为75M，详情可以查阅：730数据手册 第6.3.36.3小节  SPI interface characteristics
   *
	*	2. 
   *
	*	3. 这里将 pll2_q_ck 设置为 140M 作为 SPI4 的内核时钟，然后再经过2分频得到 70M 的SCK驱动时钟
   *
*** FANKE ***/  
  
//	配置MPU
//
void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	HAL_MPU_Disable();		// 先禁止MPU

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

	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);	// 使能MPU
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
