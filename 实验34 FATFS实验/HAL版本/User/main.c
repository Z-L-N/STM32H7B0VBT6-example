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
#include "sdmmc_sdcard.h"
#include "./FATFS/exfuns/exfuns.h"

/********************************************** �������� *******************************************/

//void SystemClock_Config(void);		// ʱ�ӳ�ʼ��
/**
 * @brief       ͨ�����ڴ�ӡSD�������Ϣ
 * @param       ��
 * @retval      ��
 */
void show_sdcard_info(void)
{
    uint64_t card_capacity;       /* SD������ */
    HAL_SD_CardCIDTypeDef sd_card_cid;

    HAL_SD_GetCardCID(&g_sd_handle, &sd_card_cid);       /* ��ȡCID */
    get_sd_card_info(&g_sd_card_info_handle);            /* ��ȡSD����Ϣ */

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

    card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* ����SD������ */
    printf("Card ManufacturerID:%d\r\n", sd_card_cid.ManufacturerID);                /* ������ID */
	LCD_DisplayString( 0, 80, "Card ManufacturerID:");	
    LCD_DisplayNumber( 160, 80, sd_card_cid.ManufacturerID, 4) ;
	
	printf("Card RCA:%d\r\n", g_sd_card_info_handle.RelCardAdd);                     /* ����Ե�ַ */
	LCD_DisplayString( 0, 96, "Card RCA:");
    LCD_DisplayNumber( 160, 96, g_sd_card_info_handle.RelCardAdd, 4) ;
	
	printf("LogBlockNbr:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));    /* ��ʾ�߼������� */
    LCD_DisplayString( 0, 112, "LogBlockNbr:");
	LCD_DisplayNumber( 160, 112, (uint32_t)(g_sd_card_info_handle.LogBlockNbr), 4) ;
	
	printf("LogBlockSize:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));  /* ��ʾ�߼����С */
    LCD_DisplayString( 0, 128, "LogBlockSize:");
	LCD_DisplayNumber( 160, 128, (uint32_t)(g_sd_card_info_handle.LogBlockSize), 4) ;
	
	printf("Card Capacity:%d MB\r\n", (uint32_t)(card_capacity >> 20));              /* ��ʾ���� */
    LCD_DisplayString( 0, 144, "Card Capacity:");
	card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize);
	LCD_DisplayNumber( 160, 144, (uint32_t)(card_capacity >> 20) , 4) ;
	
	printf("Card BlockSize:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);            /* ��ʾ���С */
	LCD_DisplayString( 0, 160, "Card BlockSize:");
	LCD_DisplayNumber( 160, 160, g_sd_card_info_handle.BlockSize, 4) ;
}
/**
 * @brief       ����SD���Ķ�ȡ
 *   @note      ��secaddr��ַ��ʼ,��ȡseccnt������������
 * @param       secaddr : ������ַ
 * @param       seccnt  : ������
 * @retval      ��
 */
void sd_test_read(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;
    buf = mymalloc(AXI_SRAM123, seccnt * 512);       /* �����ڴ�,��SDRAM�����ڴ� */
    sta = sd_read_disk(buf, secaddr, seccnt);   /* ��ȡsecaddr������ʼ������ */

    if (sta == 0)
    {
        printf("SECTOR %d DATA:\r\n", secaddr);

        for (i = 0; i < seccnt * 512; i++)
        {
            printf("%x ", buf[i]);  /* ��ӡsecaddr��ʼ���������� */
        }
        
        printf("\r\nDATA ENDED\r\n");
    }
    else printf("err:%d\r\n", sta);

    myfree(AXI_SRAM123, buf);    /* �ͷ��ڴ� */
}

/**
 * @brief       ����SD����д��
 *   @note      ��secaddr��ַ��ʼ,д��seccnt������������
 *              ����!! ���дȫ��0XFF������,���������SD��.
 *
 * @param       secaddr : ������ַ
 * @param       seccnt  : ������
 * @retval      ��
 */
void sd_test_write(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;
    buf = mymalloc(AXI_SRAM123, seccnt * 512);   /* ��SDRAM�����ڴ� */

    for (i = 0; i < seccnt * 512; i++)      /* ��ʼ��д�������,��3�ı���. */
    {
        buf[i] = i * 3;
    }

    sta = sd_write_disk(buf, secaddr, seccnt); /* ��secaddr������ʼд��seccnt���������� */

    if (sta == 0)
    {
        printf("Write over!\r\n");
    }
    else printf("err:%d\r\n", sta);

    myfree(AXI_SRAM123, buf);    /* �ͷ��ڴ� */
}

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
    uint32_t sd_size;
	uint32_t total, free;
	
	sys_cache_enable();  // ʹ��ICache��ʹ��DCache
	HAL_Init();					 // ��ʼ��HAL��
	sys_stm32_clock_init(112, 5, 2, 2); /* ����ϵͳʱ�ӣ���Ƶ280MHz */
	delay_init(280); /* ��ʼ���ӳٺ��� */
	USART1_Init(115200);
	
	LED_Init();
	key_init();   
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
	
	my_mem_init(AXI_SRAM123);                /* ��ʼ���ڲ��ڴ��(AXI) */
    my_mem_init(AHB_SRAM12);                /* ��ʼ��SRAM12�ڴ��(SRAM1+SRAM2) */
    my_mem_init(SDR_SRAM);                 /* ��ʼ��SRAM4�ڴ��(SRAM4) */
    my_mem_init(SRAM_DTCM);              /* ��ʼ��DTCM�ڴ��(DTCM) */
    my_mem_init(SRAM_ITCM);              /* ��ʼ��ITCM�ڴ��(ITCM) */
	
	printf("\r\n STM32H7B0 SD��FATFSʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU SD FATFS TEST");	
	while (sd_init())    /* ��ⲻ��SD�� */
    {
        LCD_DisplayString( 0, 16, "SD Card Error!");	
        delay_ms(500);
        LCD_DisplayString( 0, 16, "Please Check!");	
        delay_ms(500);
        LED0_Toggle;  /* �����˸ */
    }
	 /* ���SD���ɹ� */
	LCD_DisplayString( 0, 16, "SD Card OK");	
	LCD_DisplayString( 0, 32, "SD Card Size:      MB");	
	card_capacity = (uint64_t)(g_sd_card_info_handle.LogBlockNbr) * (uint64_t)(g_sd_card_info_handle.LogBlockSize); /* ����SD������ */
	LCD_DisplayNumber( 108, 32, card_capacity >> 20, 4) ;
	 
	exfuns_init();                  /* Ϊfatfs��ر��������ڴ� */
	f_mount(fs[0], "0:", 1);        /* ����SD�� */
	
	while (exfuns_get_free("0", &total, &free)) /* �õ�SD������������ʣ������ */
    {
		LCD_DisplayString( 0, 48, "SD Card Fatfs Error!");
        delay_ms(200);
		LCD_Clear();
		delay_ms(200);
        LED0_Toggle; /* �����˸ */
    }
	LCD_DisplayString( 0, 48, "FATFS OK!");
	LCD_DisplayString( 0, 64, "SD Total Size:      MB");
	LCD_DisplayString( 0, 80, "SD Free Size:       MB");
	LCD_DisplayNumber( 120, 64, total >> 10, 5) ;
	LCD_DisplayNumber( 120, 80, free >> 10, 5) ;
	
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
