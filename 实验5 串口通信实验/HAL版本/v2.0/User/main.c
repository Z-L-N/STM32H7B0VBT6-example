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

#include "led.h"
#include "beep.h"
#include "key.h"

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
	uint8_t key;
	uint8_t len;
	uint8_t str1[4]="led";
	uint8_t str2[5]="beep";
	uint16_t times=0;
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	LED_Init();
	key_init();   
	
	printf("\r\n STM32H7B0 串口实验 \r\n");
	printf("\r\n printf函数测试 \r\n");	
	
	while (1)
	{
		if (g_usart1_rx_sta & 0x8000) /* 接收到了数据 */
		{
			 len = g_usart1_rx_sta & 0x3fff; /* 得到此次接收到的数据长度 */
			 printf("\r\n 您发送的消息为:\r\n");
			/*发送接收到的数据*/
			if(NULL != strstr(g_usart1_rx_buf, str2))
			{
				LED0_Toggle;
			}
			if(NULL != strstr(g_usart1_rx_buf, str1))
			{
				BEEP_TOGGLE();
			}
			 HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart1_rx_buf,len,1000);
			/*等待发送结束*/
			 while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET); 
			 printf("\r\n\r\n"); /* 插入换行 */
			 memset(g_usart1_rx_buf, 0 ,sizeof(g_usart1_rx_buf));
			 g_usart1_rx_sta = 0;
		}
		else
        {
            times++;
            if (times % 200 == 0) 
				printf("请输入数据,以回车键结束\r\n");

            if (times % 30  == 0) 
				LED0_Toggle; /* 闪烁LED,提示系统正在运行. */

            delay_ms(10);
        }
		
//		LED0_Toggle;
//		delay_ms(2000);
		
//		key = key_scan(0); /* 得到键值 */
//		if (key)
//		{
//			 switch (key)
//			 {
//			 case WKUP_PRES: /* 控制 LED0(RED)翻转 */
//						LED0_Toggle; /* LED0 状态取反 */
//			 break;
//			 case KEY1_PRES: /* 控制 LED1(GREEN)翻转 */
//						BEEP_TOGGLE(); /* LED1 状态取反 */
//			 break;
//			 case KEY0_PRES: /* 控制 LED2(BLUE)翻转 */
//						LED0_Toggle; /* LED2 状态取反 */
//			 break;
//			 }
//	  }
//		else
//		{
//			 delay_ms(10);
//		}
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
