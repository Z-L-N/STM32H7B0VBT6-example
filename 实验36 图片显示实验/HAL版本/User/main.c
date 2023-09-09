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


/********************************************** 函数声明 *******************************************/

//void SystemClock_Config(void);		// 时钟初始化
/**
 * @brief       得到path路径下,目标文件的总个数
 * @param       path : 路径
 * @retval      总有效文件数
 */
uint16_t pic_get_tnum(uint8_t *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;           /* 临时目录 */
    FILINFO *tfileinfo; /* 临时文件信息 */
    tfileinfo = (FILINFO *)mymalloc(AXI_SRAM123, sizeof(FILINFO));   /* 申请内存 */
    res = f_opendir(&tdir, (const TCHAR *)path);    /* 打开目录 */

    if (res == FR_OK && tfileinfo)
    {
        while (1)       /* 查询总的有效文件数 */
        {
            res = f_readdir(&tdir, tfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || tfileinfo->fname[0] == 0)break; /* 错误了/到末尾了,退出 */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)   /* 取高四位,看看是不是图片文件 */
            {
                rval++; /* 有效文件数增加1 */
            }
        }
    }

    myfree(AXI_SRAM123, tfileinfo);  /* 释放内存 */
    return rval;
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
	uint16_t temp;
	
	uint32_t total, free;
	uint8_t res;
    DIR picdir;             /* 图片目录 */
    FILINFO *picfileinfo;   /* 文件信息 */
    char *pname;            /* 带路径的文件名 */
    uint16_t totpicnum;     /* 图片文件总数 */
    uint16_t curindex;      /* 图片当前索引 */
	uint32_t *picoffsettbl; /* 图片文件offset索引表 */
	
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
	
	printf("\r\n STM32H7B0 图片显示实验 \r\n");
	LCD_DisplayString( 0, 0, "STM32H7B0 MCU PICTURE TEST");	
	
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
	
	exfuns_init();                  /* 为fatfs相关变量申请内存 */
	f_mount(fs[0], "0:", 1);        /* 挂载SD卡 */
	
	while (exfuns_get_free("0", &total, &free)) /* 得到SD卡的总容量和剩余容量 */
    {
		LCD_DisplayString( 0, 48, "SD Card Fatfs Error!");
        delay_ms(200);
		LCD_Clear();
		delay_ms(200);
        LED0_Toggle; /* 红灯闪烁 */
    }
	LCD_DisplayString( 0, 48, "FATFS OK!");
	LCD_DisplayString( 0, 64, "SD Total Size:      MB");
	LCD_DisplayString( 0, 80, "SD Free Size:       MB");
	LCD_DisplayNumber( 120, 64, total >> 10, 5) ;
	LCD_DisplayNumber( 120, 80, free >> 10, 5) ;
	
	 while (f_opendir(&picdir, "0:/PICTURE"))    /* 打开图片文件夹 */
    {
		LCD_DisplayString( 0, 96, "PICTURE file error!");
		printf("PICTURE file error!\r\n");
        delay_ms(200);        
    }
	printf("PICTURE file ok!\r\n");
	totpicnum = pic_get_tnum((uint8_t *)"0:/PICTURE");  /* 得到总有效文件数 */
	while (totpicnum == NULL)   /* 图片文件为0 */
    {
		LCD_DisplayString( 0, 96, "PICTURE file no have!");
		printf("PICTURE file no have!\r\n");
        delay_ms(200);	
    }
	printf("PICTURE file have!\r\n");
	picfileinfo = (FILINFO *)mymalloc(AXI_SRAM123, sizeof(FILINFO)); /* 申请内存 */
    pname = mymalloc(AXI_SRAM123, FF_MAX_LFN * 2 + 1);   /* 为带路径的文件名分配内存 */
    picoffsettbl = mymalloc(AXI_SRAM123, 4 * totpicnum); /* 申请4*totpicnum个字节的内存,用于存放图片索引 */
	while (!picfileinfo || !pname || !picoffsettbl) /* 内存分配出错 */
    {
		LCD_DisplayString( 0, 96, "mem get error!");
		printf("mem get error!\r\n");
        delay_ms(200);
    }
	printf("mem get ok!\r\n");
	 /* 记录索引 */
    res = f_opendir(&picdir, "0:/PICTURE");         /* 打开目录 */
	if (res == FR_OK)
    {
        curindex = 0;   /* 当前索引为0 */

        while (1)       /* 全部查询一遍 */
        {
            temp = picdir.dptr; /* 记录当前dptr偏移 */
            res = f_readdir(&picdir, picfileinfo);  /* 读取目录下的一个文件 */

            if (res != FR_OK || picfileinfo->fname[0] == 0)break;   /* 错误了/到末尾了,退出 */

            res = exfuns_file_type(picfileinfo->fname);

            if ((res & 0XF0) == 0X50)   /* 取高四位,看看是不是图片文件 */
            {
                picoffsettbl[curindex] = temp;  /* 记录索引 */
                curindex++;
            }
        }
    }
	
	LCD_DisplayString( 0, 112, "picture show begin...");
	delay_ms(1500);
	piclib_init();  /* 初始化画图 */
    curindex = 0;   /* 从0开始显示 */
    res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE");  /* 打开目录 */
	while (res == FR_OK)   /* 打开成功 */
    {
        dir_sdi(&picdir, picoffsettbl[curindex]);   /* 改变当前目录索引 */
        res = f_readdir(&picdir, picfileinfo);      /* 读取目录下的一个文件 */

        if (res != FR_OK || picfileinfo->fname[0] == 0)break;   /* 错误了/到末尾了,退出 */

        strcpy((char *)pname, "0:/PICTURE/");       /* 复制路径(目录) */
        strcat((char *)pname, (const char *)picfileinfo->fname);        /* 将文件名接在后面 */
        LCD_Clear();			 // 清屏函数
        piclib_ai_load_picfile(pname, 0, 0, LCD_Width, LCD_Height, 1);    /* 显示图片 */
        //text_show_string(2, 2, lcddev.width, 16, (char*)pname, 16, 1, RED);     /* 显示图片名字 */
		delay_ms(3000);
        LED0_Toggle; /* 红灯闪烁 */
		curindex++;
		if (curindex >= totpicnum)
			curindex = 0; /* 到末尾的时候,自动从头开始 */
		
	}
	
	myfree(AXI_SRAM123, picfileinfo);    /* 释放内存 */
    myfree(AXI_SRAM123, pname);          /* 释放内存 */
    myfree(AXI_SRAM123, picoffsettbl);   /* 释放内存 */
	
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
