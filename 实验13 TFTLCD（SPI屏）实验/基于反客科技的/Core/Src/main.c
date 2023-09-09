/***
	*************************************************************************************************
	*	@file  	main.c
	*	@version V1.0
	*  @date    2022-4-20
	*	@author  ���ͿƼ�
	*	@brief   SPI������Ļ
   ************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨������STM32H7B0VBT6���İ� ���ͺţ�FK7B0M2-VBT6��+ 2.00��Һ��ģ�飨�ͺţ�SPI200M1-240*320��
	*	�Ա���ַ��https://shop212360197.taobao.com
	*	QQ����Ⱥ��536665479
	*
>>>>> ����˵����
	*
	*	SPI����LCD���м򵥵���ʾ
	*
	************************************************************************************************
***/

#include "main.h"
#include "led.h"
#include "usart.h"
#include "lcd_spi_200.h"

/********************************************** �������� *******************************************/

void SystemClock_Config(void);		// ʱ�ӳ�ʼ��

// LCD���Ժ��������������ڵײ�
void 	LCD_Test_Clear(void);			// ��������
void 	LCD_Test_Text(void);			   // �ı�����
void 	LCD_Test_Variable (void);	   // ������ʾ������������С��
void 	LCD_Test_Color(void);			// ����������
void 	LCD_Test_Grahic(void);		   // 2Dͼ�λ���
void 	LCD_Test_Image(void);			// ͼƬ��ʾ
void  LCD_Test_Direction(void);	   // ������ʾ����

unsigned char sec=0,min=0,hour=0;

void timeClock_display(void)
{
	sec++;
	if(sec>=60)
	{
		sec=0;
		min++;
		if(min>=60)
		{
			min=0;
			hour++;
			if(hour>=24)
			{
				hour=0;
			}
		}
	}
	LCD_DisplayNumber( 256, 0, hour,2);
	LCD_DisplayChar(256+2*8, 0,':');
	LCD_DisplayNumber( 256+3*8, 0, min,2);
	LCD_DisplayChar(256+5*8, 0,':');
	LCD_DisplayNumber( 256+6*8, 0, sec,2);			
//	HAL_Delay(1000);
}

/***************************************************************************************************
*	�� �� ��: main
*
*	˵    ��: ��
*
****************************************************************************************************/

int main(void)
{
	SCB_EnableICache();		// ʹ��ICache
	SCB_EnableDCache();		// ʹ��DCache
	HAL_Init();					// ��ʼ��HAL��
	SystemClock_Config();	// ����ϵͳʱ�ӣ���Ƶ280MHz
	LED_Init();					// ��ʼ��LED����
	USART1_Init();				// USART1��ʼ��	
	
	SPI_LCD_Init();			// SPI LCD��Ļ��ʼ��
//	LCD_Clear();		// ����
	
//	LCD_SetBackColor(LCD_BLACK);
//	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
//	LCD_SetColor(LCD_GREEN);				// ���û�����ɫ
	LCD_DisplayText(0,0,"\\");
	LCD_DisplayText(8,16,"\\");
	LCD_DisplayText(16,32,"ST");
	LCD_DisplayText(32,48,"\\");
	LCD_DisplayText(40,64,"\\");
	
	LCD_DisplayText(40,0,"/");
	LCD_DisplayText(32,16,"/");
	LCD_DisplayText(8,48,"/");
	LCD_DisplayText(0.,64,"/");
	
	
	
	LCD_DisplayText(0,80,"st@H7:");
	while(1)
	{
		LCD_DisplayText(6*8,80,"|");
		HAL_Delay(500);
		LCD_DisplayText(6*8,80," ");
		HAL_Delay(500);
		timeClock_display();
	}
//	while (1)
//	{
//		LCD_Test_Clear(); 		// ��������
//		LCD_Test_Text();			// �ı�����
//		LCD_Test_Variable ();	// ������ʾ������������С��
//		LCD_Test_Color();			// ��ɫ����
//		LCD_Test_Grahic();		// 2Dͼ�λ���	
//		LCD_Test_Image();			// ͼƬ��ʾ
//		LCD_Test_Direction();	// ������ʾ����
//	}
}

/****************************************************************************************************/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1 (AHB  Clock  280MHz)
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 112
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  
  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 112;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  

  
/***************************************** SPI4�ں�ʱ������ **************************************************/
/*** FANKE ***
>>>>> ����˵����
	*
	*	1.	SPI4 ��������������ʱ��Ϊ75M��������Բ��ģ�7B0�����ֲ� ��6.3.36.3С��  SPI interface characteristics
   *
	*	2. Ϊ�˷������ú��û���ֲ�����ｫ2��Ƶ��� pclk��140M�� ��Ϊ SPI4 ���ں�ʱ�ӣ�Ȼ����SPI������������Ϊ2��Ƶ�õ� 70M ��SCK����ʱ��
   *
   *	3. ��Ȼ��Ļ������ST7789��������������ʱ��Ϊ62.5M����ʵ�ʲ����У���ʹ����Ϊ70MҲ�ǳ��ȶ������û�
  *		��SPIʱ�����ϸ�Ҫ�󣬿����޸�SPI�ں�ʱ����ʵ��
   *
*** FANKE ***/  
  
  	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI4;				// ѡ��SPI4ʱ��
	PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;	// 
  
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
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

/*************************************************************************************************
*	�� �� ��:	LCD_Test_Clear
*
*	��������:	��������
*************************************************************************************************/
void LCD_Test_Clear(void)
{
	uint8_t	i = 0;			// ��������
			
	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
	LCD_SetColor(LCD_BLACK);				// ���û�����ɫ

	for(i=0;i<8;i++)
	{
		switch (i)		// �л�����ɫ
		{
			case 0: LCD_SetBackColor(LIGHT_RED); 		break;	
			case 1: LCD_SetBackColor(LIGHT_GREEN); 	break;				
			case 2: LCD_SetBackColor(LIGHT_BLUE); 		break;
			case 3: LCD_SetBackColor(LIGHT_YELLOW); 	break;
			case 4: LCD_SetBackColor(LIGHT_CYAN);		break;
			case 5: LCD_SetBackColor(LIGHT_GREY); 		break;
			case 6: LCD_SetBackColor(LIGHT_MAGENTA); 	break;
			case 7: LCD_SetBackColor(LCD_WHITE); 		break;			
			default:	break;			
		}
		LCD_Clear();		// ����
		LCD_DisplayText(13, 70,"STM32H7 ˢ������");
		LCD_DisplayText(13,106,"��Ļ�ֱ���:240*320");
		LCD_DisplayText(13,142,"������:ST7789");	
		HAL_Delay(1000);	// ��ʱ
	}
}

/*************************************************************************************************
*	�� �� ��:	LCD_Test_Text
*
*	��������:	�ı���ʾ����
*
*	˵    ��:	��ʾ�ı����������������С�����ĺ�ASCII�ַ� 
*************************************************************************************************/
void LCD_Test_Text(void)
{
	LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
	LCD_Clear(); 								// ����
	
	LCD_SetColor(LCD_WHITE);
	LCD_SetAsciiFont(&ASCII_Font32); LCD_DisplayString(0, 0,"!#$'()*+,-.0123"); 						    		
	LCD_SetAsciiFont(&ASCII_Font24); LCD_DisplayString(0,32,"!#$'()*+,-.012345678"); 				   
	LCD_SetAsciiFont(&ASCII_Font20); LCD_DisplayString(0,56,"!#$'()*+,-.0123456789:;<");      	
	LCD_SetAsciiFont(&ASCII_Font16); LCD_DisplayString(0,76,"!#$'()*+,-.0123456789:;<=>?@AB"); 	
	LCD_SetAsciiFont(&ASCII_Font12); LCD_DisplayString(0,92,"!#$'()*+,-.0123456789:;<=>?@ABCDEFGHIJKL"); 	
																																		
	LCD_SetColor(LCD_CYAN);                                                                             
	LCD_SetAsciiFont(&ASCII_Font12); LCD_DisplayString(0,104,"!#&'()*+,-.0123456789:;<=>?@ABCDEFGHIJKL"); 	
	LCD_SetAsciiFont(&ASCII_Font16); LCD_DisplayString(0,116,"!#&'()*+,-.0123456789:;<=>?@AB"); 	
	LCD_SetAsciiFont(&ASCII_Font20); LCD_DisplayString(0,132,"!#&'()*+,-.0123456789:;<");		  	
	LCD_SetAsciiFont(&ASCII_Font24); LCD_DisplayString(0,152,"!#&'()*+,-.012345678"); 				  	
	LCD_SetAsciiFont(&ASCII_Font32); LCD_DisplayString(0,176,"!#&'()*+,-.0123"); 							  		

	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
	LCD_SetColor(LCD_YELLOW);				// ���û��ʣ���ɫ
	LCD_DisplayText(0, 216,"ASCII�ַ���");

	HAL_Delay(2000);	// ��ʱ�ȴ�
	LCD_Clear(); 								// ����

	LCD_SetTextFont(&CH_Font12);			// ����1212��������,ASCII�����ӦΪ1206
	LCD_SetColor(0X8AC6D1);					// ���û���
	LCD_DisplayText(14, 10,"1212:���ͿƼ�");	
	
	LCD_SetTextFont(&CH_Font16);			// ����1616��������,ASCII�����ӦΪ1608
	LCD_SetColor(0XC5E1A5);					// ���û���
	LCD_DisplayText(14, 30,"1616:���ͿƼ�");		
	
	LCD_SetTextFont(&CH_Font20);			// ����2020��������,ASCII�����ӦΪ2010
	LCD_SetColor(0XFFB549);					// ���û���
	LCD_DisplayText(14, 60,"2020:���ͿƼ�");		

	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
	LCD_SetColor(0XFF585D);					// ���û���
	LCD_DisplayText(14, 90,"2424:���ͿƼ�");		

	LCD_SetTextFont(&CH_Font32);			// ����3232��������,ASCII�����ӦΪ3216
	LCD_SetColor(0xFFB6B9);					// ���û���
	LCD_DisplayText(14, 130,"3232:���ͿƼ�");		

	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
	LCD_SetColor(LCD_WHITE);
 	LCD_DisplayText(14, 180,"������ʾ");	  

	HAL_Delay(2000);	// ��ʱ�ȴ�
}
/************************************************************************************************
*	�� �� ��:	LCD_Test_Variable
*
*	��������:	������ʾ������������С��
*************************************************************************************************/
void LCD_Test_Variable (void)
{
	uint16_t i;					// ��������
	int32_t	a = 0;			// �����������������ڲ���
	int32_t	b = 0;			// �����������������ڲ���
	int32_t	c = 0;			// �����������������ڲ���

	double p = 3.1415926;	// ���帡�������������ڲ���
	double f = -1234.1234;	// ���帡�������������ڲ���
	
	LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
	LCD_Clear(); 								// ����
	
   LCD_SetTextFont(&CH_Font24);     
	LCD_SetColor(LIGHT_CYAN);					// ���û��ʣ�����ɫ		
	LCD_DisplayText(0,10,"����:");				
	LCD_DisplayText(0,40,"����:");					
				
	LCD_SetColor(LIGHT_YELLOW);				// ���û��ʣ�����ɫ		
	LCD_DisplayText(0, 80,"���ո�:");	
	LCD_DisplayText(0,110,"���0:");	
	
	LCD_SetColor(LIGHT_RED);					// ���û���	������ɫ		
	LCD_DisplayText(0, 150,"��С��:");	
	LCD_DisplayText(0, 180,"��С��:");		
	
	for(i=0;i<100;i++)
   {
		LCD_SetColor(LIGHT_CYAN);								// ���û���	������ɫ	
		LCD_ShowNumMode(Fill_Space);							// ����λ���ո�
		LCD_DisplayNumber( 80,10, b+i*10, 4) ;				// ��ʾ����			
		LCD_DisplayNumber( 80,40, c-i*10, 4) ;				// ��ʾ����			
		
		LCD_SetColor(LIGHT_YELLOW);								// ���û��ʣ�����ɫ	

		LCD_ShowNumMode(Fill_Space);								// ����λ��� �ո�
		LCD_DisplayNumber( 130, 80, a+i*150, 8) ;				// ��ʾ����		

		LCD_ShowNumMode(Fill_Zero);								// ����λ���0      
		LCD_DisplayNumber( 130,110, b+i*150, 8) ;				// ��ʾ����			
		
		LCD_SetColor(LIGHT_RED);									// ���û��ʣ�����ɫ			
		LCD_ShowNumMode(Fill_Space);								// ����λ���ո�		
		LCD_DisplayDecimals( 100, 150, p+i*0.1,  6,3);		// ��ʾС��	
		LCD_DisplayDecimals( 100, 180, f+i*0.01, 11,4);		// ��ʾС��		
		
		HAL_Delay(15);				
   }
	HAL_Delay(2500);		
}
/*************************************************************************************************
*	�� �� ��:	LCD_Test_Color
*
*	��������:	��ɫ��
*************************************************************************************************/
void LCD_Test_Color(void)
{
	uint16_t i;					// ��������
	uint16_t y;
// ��ɫ����>>>>>	
	LCD_SetBackColor(LCD_BLACK); //���ñ���ɫ
	LCD_Clear(); //������ˢ����ɫ
	
	LCD_SetTextFont(&CH_Font20);			// ��������
	LCD_SetColor(LCD_WHITE);				// ���û�����ɫ
	LCD_DisplayText(0,0,"RGB����ɫ:");

	//ʹ�û��ߺ�����������ɫɫ��
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_RED-(i<<16) );
      LCD_DrawLine_V(0+i,  20,10);
	}
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_GREEN-(i<<8) );
      LCD_DrawLine_V(0+i,  35,10);
	}
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_BLUE-i );
      LCD_DrawLine_V(0+i,  50,10);
	}	

   y = 70;
   LCD_SetColor(LIGHT_CYAN);    LCD_FillRect(150,y+5     ,90,10);  LCD_DisplayString(0,y     ,"LIGHT_CYAN");	   
	LCD_SetColor(LIGHT_MAGENTA); LCD_FillRect(150,y+20*1+5,90,10);  LCD_DisplayString(0,y+20*1,"LIGHT_MAGENTA");	
	LCD_SetColor(LIGHT_YELLOW);  LCD_FillRect(150,y+20*2+5,90,10);  LCD_DisplayString(0,y+20*2,"LIGHT_YELLOW");	
	LCD_SetColor(LIGHT_GREY);    LCD_FillRect(150,y+20*3+5,90,10);  LCD_DisplayString(0,y+20*3,"LIGHT_GREY");  	
	LCD_SetColor(LIGHT_RED);     LCD_FillRect(150,y+20*4+5,90,10);  LCD_DisplayString(0,y+20*4,"LIGHT_RED");  	
	LCD_SetColor(LIGHT_BLUE);    LCD_FillRect(150,y+20*5+5,90,10);  LCD_DisplayString(0,y+20*5,"LIGHT_BLUE");  	

   LCD_SetColor(DARK_CYAN);     LCD_FillRect(150,y+20*6+5,90,10);  LCD_DisplayString(0,y+20*6,"DARK_CYAN");		
	LCD_SetColor(DARK_MAGENTA);  LCD_FillRect(150,y+20*7+5,90,10);  LCD_DisplayString(0,y+20*7,"DARK_MAGENTA");	
	LCD_SetColor(DARK_YELLOW);   LCD_FillRect(150,y+20*8+5,90,10);  LCD_DisplayString(0,y+20*8,"DARK_YELLOW");	
	LCD_SetColor(DARK_GREY);     LCD_FillRect(150,y+20*9+5,90,10);	 LCD_DisplayString(0,y+20*9,"DARK_GREY");	
	LCD_SetColor(DARK_RED);   	  LCD_FillRect(150,y+20*10+5,90,10); LCD_DisplayString(0,y+20*10,"DARK_RED");	
	LCD_SetColor(DARK_GREEN);    LCD_FillRect(150,y+20*11+5,90,10); LCD_DisplayString(0,y+20*11,"DARK_GREEN");	

	HAL_Delay(2000);
}

/*************************************************************************************************
*	�� �� ��:	LCD_Test_Grahic
*
*	��������:	2Dͼ�λ���
*************************************************************************************************/
void LCD_Test_Grahic(void)
{
	LCD_SetBackColor(LCD_BLACK); //���ñ���ɫ
	LCD_Clear(); //������ˢ����ɫ	

	LCD_SetColor(LCD_WHITE);	
	LCD_DrawRect(0,0,240,320); 			//���ƾ���

	LCD_SetColor(LCD_RED);    LCD_FillCircle(140,50,30);		//���Բ��
	LCD_SetColor(LCD_GREEN);  LCD_FillCircle(170,50,30); 	
	LCD_SetColor(LCD_BLUE);   LCD_FillCircle(200,50,30);  	
	
	LCD_SetColor(LCD_YELLOW);
	LCD_DrawLine(26,26,113,64);				//��ֱ��
	LCD_DrawLine(35,22,106,81);				//��ֱ��
	LCD_DrawLine(45,20, 93,100);				//��ֱ��
	LCD_DrawLine(52,16, 69,108);				//��ֱ��
	LCD_DrawLine(62,16, 44,108);				//��ֱ��
	
	LCD_SetColor(LIGHT_CYAN);
	LCD_DrawCircle(120,170,30);			//����Բ��
	LCD_DrawCircle(120,170,20);   

	LCD_SetColor(LIGHT_RED);	
	LCD_DrawEllipse(120,170,90,40); 	//������Բ 
	LCD_DrawEllipse(120,170,70,40); 	//������Բ    
	LCD_SetColor(LIGHT_MAGENTA);	
	LCD_DrawEllipse(120,170,100,50); 	//������Բ
	LCD_DrawEllipse(120,170,110,60);  
	
	LCD_SetColor(DARK_RED);  	LCD_FillRect( 50, 250, 50, 50);			//������
	LCD_SetColor(DARK_BLUE); 	LCD_FillRect(100, 250, 50, 50);			//������	
	LCD_SetColor(LIGHT_GREEN); LCD_FillRect(150, 250, 50, 50);			//������	
	
	HAL_Delay(2000);		
}
/*************************************************************************************************
*	�� �� ��:	LCD_Test_Image
*
*	��������:	ͼƬ��ʾ����
*************************************************************************************************/
void LCD_Test_Image(void)
{
	LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
	LCD_Clear(); 								// ����
	
	LCD_SetColor( 0xffF6E58D);
	LCD_DrawImage( 19, 55, 83, 83, Image_Android_83x83) ;	   // ��ʾͼƬ

	LCD_SetColor( 0xffDFF9FB);
	LCD_DrawImage( 141, 55, 83, 83, Image_Message_83x83) ;	// ��ʾͼƬ
	
	LCD_SetColor( 0xff9DD3A8);
	LCD_DrawImage( 19, 175, 83, 83, Image_Toys_83x83) ;		// ��ʾͼƬ
	
	LCD_SetColor( 0xffFF8753);
	LCD_DrawImage( 141, 175, 83, 83, Image_Video_83x83) ;		// ��ʾͼƬ

	HAL_Delay(2000);	
}
/*************************************************************************************************
*	�� �� ��:	LCD_Test_Direction
*
*	��������:	������ʾ����
*************************************************************************************************/
void  LCD_Test_Direction(void)
{
	for(int i=0;i<4;i++)
	{  
      LCD_SetBackColor(LCD_BLACK); 			//	���ñ���ɫ
      LCD_Clear(); 								// ����
      LCD_SetTextFont(&CH_Font24);  
	   LCD_SetColor( 0xffDFF9FB);         
		switch (i)		// �л�����ɫ
		{
			case 0:  
            LCD_SetDirection(Direction_V);		   
            LCD_DisplayText(20,20,"Direction_V"); 
         break;	

			case 1:  
            LCD_SetDirection(Direction_H); 	
            LCD_DisplayText(20,20,"Direction_H"); 
         break;	

			case 2:  
            LCD_SetDirection(Direction_V_Flip); 
            LCD_DisplayText(20,20,"Direction_V_Flip"); 
         break;
			case 3: 
            LCD_SetDirection(Direction_H_Flip); 	
            LCD_DisplayText(20,20,"Direction_H_Flip"); 
         break;
	
			default:	break;			
		}
      LCD_SetColor( 0xffF6E58D);
      LCD_DrawImage( 19, 80, 83, 83, Image_Android_83x83) ;	   // ��ʾͼƬ
      LCD_SetTextFont(&CH_Font32);
      LCD_SetColor( 0xff9DD3A8);  
      LCD_DisplayText(130,90,"����");
      LCD_DisplayText(130,130,"�Ƽ�");
      LCD_SetDirection(Direction_V);	
      HAL_Delay(1000);	// ��ʱ
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
