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
#include "math.h"

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./MALLOC/malloc.h"
#include "./PICTURE/piclib.h"
#include "./FATFS/exfuns/exfuns.h"

#include "mpu.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "lcd_spi_200.h"
#include "mcu_monitor.h"
#include "sdmmc_sdcard.h"


/********************************************** �������� *******************************************/

//void SystemClock_Config(void);		// ʱ�ӳ�ʼ��
/**
 * @brief       �õ�path·����,Ŀ���ļ����ܸ���
 * @param       path : ·��
 * @retval      ����Ч�ļ���
 */
uint16_t pic_get_tnum(uint8_t *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;           /* ��ʱĿ¼ */
    FILINFO *tfileinfo; /* ��ʱ�ļ���Ϣ */
    tfileinfo = (FILINFO *)mymalloc(AXI_SRAM123, sizeof(FILINFO));   /* �����ڴ� */
    res = f_opendir(&tdir, (const TCHAR *)path);    /* ��Ŀ¼ */

    if (res == FR_OK && tfileinfo)
    {
        while (1)       /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || tfileinfo->fname[0] == 0)break; /* ������/��ĩβ��,�˳� */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)   /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                rval++; /* ��Ч�ļ�������1 */
            }
        }
    }

    myfree(AXI_SRAM123, tfileinfo);  /* �ͷ��ڴ� */
    return rval;
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
	uint16_t temp;
	
	uint32_t total, free;
	uint8_t res;
    DIR picdir;             /* ͼƬĿ¼ */
    FILINFO *picfileinfo;   /* �ļ���Ϣ */
    char *pname;            /* ��·�����ļ��� */
    uint16_t totpicnum;     /* ͼƬ�ļ����� */
    uint16_t curindex;      /* ͼƬ��ǰ���� */
	uint32_t *picoffsettbl; /* ͼƬ�ļ�offset������ */
	
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
	
	printf("\r\n STM32H7B0 ͼƬ��ʾʵ�� \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU PICTURE TEST");	
	
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
	
	 while (f_opendir(&picdir, "0:/PICTURE"))    /* ��ͼƬ�ļ��� */
    {
		LCD_DisplayString( 0, 96, "PICTURE file error!");
		printf("PICTURE file error!\r\n");
        delay_ms(200);        
    }
	printf("PICTURE file ok!\r\n");
	totpicnum = pic_get_tnum((uint8_t *)"0:/PICTURE");  /* �õ�����Ч�ļ��� */
	while (totpicnum == NULL)   /* ͼƬ�ļ�Ϊ0 */
    {
		LCD_DisplayString( 0, 96, "PICTURE file no have!");
		printf("PICTURE file no have!\r\n");
        delay_ms(200);	
    }
	printf("PICTURE file have!\r\n");
	picfileinfo = (FILINFO *)mymalloc(AXI_SRAM123, sizeof(FILINFO)); /* �����ڴ� */
    pname = mymalloc(AXI_SRAM123, FF_MAX_LFN * 2 + 1);   /* Ϊ��·�����ļ��������ڴ� */
    picoffsettbl = mymalloc(AXI_SRAM123, 4 * totpicnum); /* ����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ���� */
	while (!picfileinfo || !pname || !picoffsettbl) /* �ڴ������� */
    {
		LCD_DisplayString( 0, 96, "mem get error!");
		printf("mem get error!\r\n");
        delay_ms(200);
    }
	printf("mem get ok!\r\n");
	 /* ��¼���� */
    res = f_opendir(&picdir, "0:/PICTURE");         /* ��Ŀ¼ */
	if (res == FR_OK)
    {
        curindex = 0;   /* ��ǰ����Ϊ0 */

        while (1)       /* ȫ����ѯһ�� */
        {
            temp = picdir.dptr; /* ��¼��ǰdptrƫ�� */
            res = f_readdir(&picdir, picfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || picfileinfo->fname[0] == 0)break;   /* ������/��ĩβ��,�˳� */

            res = exfuns_file_type(picfileinfo->fname);

            if ((res & 0XF0) == 0X50)   /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                picoffsettbl[curindex] = temp;  /* ��¼���� */
                curindex++;
            }
        }
    }
	
	LCD_DisplayString( 0, 112, "picture show begin...");
	delay_ms(1500);
	piclib_init();  /* ��ʼ����ͼ */
    curindex = 0;   /* ��0��ʼ��ʾ */
    res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE");  /* ��Ŀ¼ */
	while (res == FR_OK)   /* �򿪳ɹ� */
    {
        dir_sdi(&picdir, picoffsettbl[curindex]);   /* �ı䵱ǰĿ¼���� */
        res = f_readdir(&picdir, picfileinfo);      /* ��ȡĿ¼�µ�һ���ļ� */

        if (res != FR_OK || picfileinfo->fname[0] == 0)break;   /* ������/��ĩβ��,�˳� */

        strcpy((char *)pname, "0:/PICTURE/");       /* ����·��(Ŀ¼) */
        strcat((char *)pname, (const char *)picfileinfo->fname);        /* ���ļ������ں��� */
        LCD_Clear();			 // ��������
        piclib_ai_load_picfile(pname, 0, 0, LCD_Width, LCD_Height, 1);    /* ��ʾͼƬ */
        //text_show_string(2, 2, lcddev.width, 16, (char*)pname, 16, 1, RED);     /* ��ʾͼƬ���� */
		delay_ms(3000);
        LED0_Toggle; /* �����˸ */
		curindex++;
		if (curindex >= totpicnum)
			curindex = 0; /* ��ĩβ��ʱ��,�Զ���ͷ��ʼ */
		
	}
	
	myfree(AXI_SRAM123, picfileinfo);    /* �ͷ��ڴ� */
    myfree(AXI_SRAM123, pname);          /* �ͷ��ڴ� */
    myfree(AXI_SRAM123, picoffsettbl);   /* �ͷ��ڴ� */
	
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
