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
#include "./MALLOC/malloc.h"

#include "mpu.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "lcd_spi_200.h"
#include "mcu_monitor.h"
#include "sdmmc_sdcard.h"

/********************************************** 函数声明 *******************************************/

//void SystemClock_Config(void);		// 时钟初始化
/**
 * @brief       通过串口打印SD卡相关信息
 * @param       无
 * @retval      无
 */
void show_sdcard_info(void)
{
    uint64_t card_capacity;       /* SD卡容量 */
    HAL_SD_CardCIDTypeDef sd_card_cid;

    HAL_SD_GetCardCID(&g_sd_handle, &sd_card_cid);       /* 获取CID */
    get_sd_card_info(&g_sd_card_info_handle);            /* 获取SD卡信息 */

    switch (g_sd_card_info_handle.CardType)
    {
        case CARD_SDSC:
        {
            if (g_sd_card_info_handle.CardVersion == CARD_V1_X)
            {
                printf("Card Type:SDSC V1\r\n");
				LCD_DisplayString( 0, 48, "Card Type:SDSC V1");
            }
            else if (g_sd_card_info_handle.CardVersion == CARD_V2_X)
            {
                printf("Card Type:SDSC V2\r\n");
				LCD_DisplayString( 0, 48, "Card Type:SDSC V2");
            }
        }
            break;
        
        case CARD_SDHC_SDXC:
		{
            printf("Card Type:SDHC\r\n");
			LCD_DisplayString( 0, 48, "Card Type:SDHC");
		}
            break;
    }

    card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* 计算SD卡容量 */
    printf("Card ManufacturerID:%d\r\n", sd_card_cid.ManufacturerID);                /* 制造商ID */
	LCD_DisplayString( 0, 80, "Card ManufacturerID:");	
    LCD_DisplayNumber( 160, 80, sd_card_cid.ManufacturerID, 4) ;
	
	printf("Card RCA:%d\r\n", g_sd_card_info_handle.RelCardAdd);                     /* 卡相对地址 */
	LCD_DisplayString( 0, 96, "Card RCA:");
    LCD_DisplayNumber( 160, 96, g_sd_card_info_handle.RelCardAdd, 4) ;
	
	printf("LogBlockNbr:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));    /* 显示逻辑块数量 */
    LCD_DisplayString( 0, 112, "LogBlockNbr:");
	LCD_DisplayNumber( 160, 112, (uint32_t)(g_sd_card_info_handle.LogBlockNbr), 4) ;
	
	printf("LogBlockSize:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));  /* 显示逻辑块大小 */
    LCD_DisplayString( 0, 128, "LogBlockSize:");
	LCD_DisplayNumber( 160, 128, (uint32_t)(g_sd_card_info_handle.LogBlockSize), 4) ;
	
	printf("Card Capacity:%d MB\r\n", (uint32_t)(card_capacity >> 20));              /* 显示容量 */
    LCD_DisplayString( 0, 144, "Card Capacity:");
	card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize);
	LCD_DisplayNumber( 160, 144, (uint32_t)(card_capacity >> 20) , 4) ;
	
	printf("Card BlockSize:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);            /* 显示块大小 */
	LCD_DisplayString( 0, 160, "Card BlockSize:");
	LCD_DisplayNumber( 160, 160, g_sd_card_info_handle.BlockSize, 4) ;
}
/**
 * @brief       测试SD卡的读取
 *   @note      从secaddr地址开始,读取seccnt个扇区的数据
 * @param       secaddr : 扇区地址
 * @param       seccnt  : 扇区数
 * @retval      无
 */
void sd_test_read(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;
    buf = mymalloc(AXI_SRAM123, seccnt * 512);       /* 申请内存,从SDRAM申请内存 */
    sta = sd_read_disk(buf, secaddr, seccnt);   /* 读取secaddr扇区开始的内容 */

    if (sta == 0)
    {
        printf("SECTOR %d DATA:\r\n", secaddr);

        for (i = 0; i < seccnt * 512; i++)
        {
            printf("%x ", buf[i]);  /* 打印secaddr开始的扇区数据 */
        }
        
        printf("\r\nDATA ENDED\r\n");
    }
    else printf("err:%d\r\n", sta);

    myfree(AXI_SRAM123, buf);    /* 释放内存 */
}

/**
 * @brief       测试SD卡的写入
 *   @note      从secaddr地址开始,写入seccnt个扇区的数据
 *              慎用!! 最好写全是0XFF的扇区,否则可能损坏SD卡.
 *
 * @param       secaddr : 扇区地址
 * @param       seccnt  : 扇区数
 * @retval      无
 */
void sd_test_write(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;
    buf = mymalloc(AXI_SRAM123, seccnt * 512);   /* 从SDRAM申请内存 */

    for (i = 0; i < seccnt * 512; i++)      /* 初始化写入的数据,是3的倍数. */
    {
        buf[i] = i * 3;
    }

    sta = sd_write_disk(buf, secaddr, seccnt); /* 从secaddr扇区开始写入seccnt个扇区内容 */

    if (sta == 0)
    {
        printf("Write over!\r\n");
    }
    else printf("err:%d\r\n", sta);

    myfree(AXI_SRAM123, buf);    /* 释放内存 */
}

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
    uint32_t sd_size;
	uint8_t *buf;
	
	sys_cache_enable();  // 使能ICache、使能DCache
	HAL_Init();					 // 初始化HAL库
	sys_stm32_clock_init(112, 5, 2, 2); /* 配置系统时钟，主频280MHz */
	delay_init(280); /* 初始化延迟函数 */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD屏幕初始化
	
	my_mem_init(AXI_SRAM123);                /* 初始化内部内存池(AXI) */
    my_mem_init(AHB_SRAM12);                /* 初始化SRAM12内存池(SRAM1+SRAM2) */
    my_mem_init(SDR_SRAM);                 /* 初始化SRAM4内存池(SRAM4) */
    my_mem_init(SRAM_DTCM);              /* 初始化DTCM内存池(DTCM) */
    my_mem_init(SRAM_ITCM);              /* 初始化ITCM内存池(ITCM) */
	
	printf("\r\n STM32H7B0 SD卡读写实验 \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU");	
	LCD_DisplayString( 0, 16, "SD TEST");	
	while (sd_init())    /* 检测不到SD卡 */
    {
        LCD_DisplayString( 0, 16, "SD Card Error!");	
        delay_ms(500);
        LCD_DisplayString( 0, 16, "Please Check!");	
        delay_ms(500);
        LED0_Toggle;  /* 红灯闪烁 */
    }
	 /* 检测SD卡成功 */
	LCD_DisplayString( 0, 16, "SD Card OK");	
	LCD_DisplayString( 0, 32, "SD Card Size:      MB");	
	card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* 计算SD卡容量 */
	LCD_DisplayNumber( 108, 32, card_capacity >> 20, 4) ;
	/* 打印SD卡相关信息 */
    show_sdcard_info(); 
	
	buf = mymalloc(0, 512); /* 申请内存 */
	key=sd_read_disk(buf, 0, 1);

    if (key == 0)           /* 读取0扇区的内容 */
    {
		LCD_DisplayString( 0, 180, "USART1 Sending Data...");
        printf("SECTOR 0 DATA:\r\n");
		for (sd_size = 0; sd_size < 512; sd_size++)
        {
			printf("%x ", buf[sd_size]);    /* 打印0扇区数据 */
        }
		printf("\r\nDATA ENDED\r\n");
		LCD_DisplayString( 0, 196, "USART1 Send Data Over!");
    }
    else 
		printf("err:%d\r\n", key);
	myfree(0, buf); /* 释放内存 */
	
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
