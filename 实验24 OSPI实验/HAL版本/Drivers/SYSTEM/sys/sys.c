/**
 * �޸�˵��
 * V1.0 20200312
 * ��һ�η���
 * V1.0 20200401
 * 1, ��ͷ�ļ�����·���ĳ����·��,�����ظ����ð���·�����鷳
 * 2, �������ĳɴ�һ������,����֧��W25Q256, ʵ����֧������25Qϵ��оƬ
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"

/**
 * @brief       �����ж�������ƫ�Ƶ�ַ
 * @param       baseaddr: ��ַ
 * @param       offset: ƫ����
 * @retval      ��
 */
void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset)
{
    /* ����NVIC��������ƫ�ƼĴ���,VTOR��9λ����,��[8:0]���� */
    SCB->VTOR = baseaddr | (offset & (uint32_t)0xFFFFFE00);
}

/**
 * @brief       ִ��: WFIָ��(ִ�����ָ�����͹���״̬, �ȴ��жϻ���)
 * @param       ��
 * @retval      ��
 */
void sys_wfi_set(void)
{
    __ASM volatile("wfi");
}

/**
 * @brief       �ر������ж�(���ǲ�����fault��NMI�ж�)
 * @param       ��
 * @retval      ��
 */
void sys_intx_disable(void)
{
    __ASM volatile("cpsid i");
}

/**
 * @brief       ���������ж�
 * @param       ��
 * @retval      ��
 */
void sys_intx_enable(void)
{
    __ASM volatile("cpsie i");
}

/**
 * @brief       ����ջ����ַ
 * @note        ���ĺ�X, ����MDK��, ʵ����û�����
 * @param       addr: ջ����ַ
 * @retval      ��
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);    /* ����ջ����ַ */
}

/**
 * @brief       ʹ��STM32H7��L1-Cache, ͬʱ����D cache��ǿ��͸д
 * @param       ��
 * @retval      ��
 */
void sys_cache_enable(void)
{
    SCB_EnableICache();     /* ʹ��I-Cache,������core_cm7.h���涨�� */
    SCB_EnableDCache();     /* ʹ��D-Cache,������core_cm7.h���涨�� */
    SCB->CACR |= 1 << 2;    /* ǿ��D-Cache͸д,�粻����͸д,ʵ��ʹ���п��������������� */
}

/**����ԭ�ӵ�ע��
 * @brief       ʱ�����ú���
 * @param       plln: PLL1��Ƶϵ��(PLL��Ƶ), ȡֵ��Χ: 4~512.
 * @param       pllm: PLL1Ԥ��Ƶϵ��(��PLL֮ǰ�ķ�Ƶ), ȡֵ��Χ: 2~63.
 * @param       pllp: PLL1��p��Ƶϵ��(PLL֮��ķ�Ƶ), ��Ƶ����Ϊϵͳʱ��, ȡֵ��Χ: 2~128.(�ұ�����2�ı���)
 * @param       pllq: PLL1��q��Ƶϵ��(PLL֮��ķ�Ƶ), ȡֵ��Χ: 1~128.
 * @note
 *
 *              Fvco: VCOƵ��
 *              Fsys: ϵͳʱ��Ƶ��, Ҳ��PLL1��p��Ƶ���ʱ��Ƶ��
 *              Fq:   PLL1��q��Ƶ���ʱ��Ƶ��
 *              Fs:   PLL����ʱ��Ƶ��, ������HSI, CSI, HSE��.
 *              Fvco = Fs * (plln / pllm);
 *              Fsys = Fvco / pllp = Fs * (plln / (pllm * pllp));
 *              Fq   = Fvco / pllq = Fs * (plln / (pllm * pllq));
 *
 *              �ⲿ����Ϊ25M��ʱ��, �Ƽ�ֵ: plln = 112, pllm = 5, pllp = 2, pllq = 4.
 *              �ⲿ����Ϊ 8M��ʱ��, �Ƽ�ֵ: plln = 240, pllm = 2, pllp = 2, pllq = 4.
 *              �õ�:Fvco = 8 * (240 / 2) = 960Mhz
 *                   Fsys = pll1_p_ck = 960 / 2 = 480Mhz
 *                   Fq   = pll1_q_ck = 960 / 4 = 240Mhz
 *
 *              H750Ĭ����Ҫ���õ�Ƶ������:
 *              CPUƵ��(rcc_c_ck) = sys_d1cpre_ck = 480Mhz
 *              rcc_aclk = rcc_hclk3 = 240Mhz
 *              AHB1/2/3/4(rcc_hclk1/2/3/4) = 240Mhz
 *              APB1/2/3/4(rcc_pclk1/2/3/4) = 120Mhz
 *              pll2_p_ck = (8 / 8) * 440 / 2) = 220Mhz
 *              pll2_r_ck = FMCʱ��Ƶ�� = ((8 / 8) * 440 / 2) = 220Mhz
 *
 * @retval      �������: 0, �ɹ�; 1, ����;
 */
 /***���ͿƼ���ע��*****************************************************************************************/
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
uint8_t sys_stm32_clock_init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct= {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);                         /* ʹ�ܹ������ø��� */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);  /* VOS = 1, Scale1, 1.2V�ں˵�ѹ,FLASH���ʿ��Եõ�������� */
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY));     								/* �ȴ���ѹ�ȶ� */
		__HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /* ʹ��HSE����ѡ��HSE��ΪPLLʱ��Դ������PLL1������USBʱ�� */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;						/* �� HSE */
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;						/* �ر� HSI */
    RCC_OscInitStruct.CSIState = RCC_CSI_OFF;						/* �ر� CSI */
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;				/* �� HSI48 */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;				/* �� PLL */
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;/* ���� PLL ʱ��ԴΪ HSE */
    RCC_OscInitStruct.PLL.PLLN = plln;
    RCC_OscInitStruct.PLL.PLLM = pllm;
    RCC_OscInitStruct.PLL.PLLP = pllp;
    RCC_OscInitStruct.PLL.PLLQ = pllq;
		RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    ret=HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(ret != HAL_OK)
    {
        return 1;
    }

    /*
     *  ѡ��PLL�������Ϊϵͳʱ��
     *  ����RCC_CLOCKTYPE_SYSCLKϵͳʱ��,280M
     *  ����RCC_CLOCKTYPE_HCLK ʱ��,280Mhz,��ӦAHB1��AHB2��AHB3��AHB4����
     *  ����RCC_CLOCKTYPE_PCLK1ʱ��,140Mhz,��ӦAPB1����
     *  ����RCC_CLOCKTYPE_PCLK2ʱ��,140Mhz,��ӦAPB2����
     *  ����RCC_CLOCKTYPE_D1PCLK1ʱ��,140Mhz,��ӦAPB3����
     *  ����RCC_CLOCKTYPE_D3PCLK1ʱ��,140Mhz,��ӦAPB4����
     */

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK \
                                    | RCC_CLOCKTYPE_HCLK \
                                    | RCC_CLOCKTYPE_PCLK1 \
                                    | RCC_CLOCKTYPE_PCLK2 \
                                    | RCC_CLOCKTYPE_D1PCLK1 \
                                    | RCC_CLOCKTYPE_D3PCLK1);

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
    if(ret != HAL_OK)
    {
        return 1;
    }

    /*
     *  ����PLL2��R��Ƶ���, Ϊ220Mhz 
     *  ����FMCʱ��Դ��PLL2R
     *  ����QSPIʱ��Դ��PLL2R ��Ϊϵͳ��ʱ��HCLK ��ΪOSPI���ں�ʱ�ӣ��ٶ�Ϊ280M���پ���2��Ƶ�õ�140M����ʱ��
     *  ���ô���1 �� ����6 ��ʱ��Դ����: PCLK2 = 120Mhz
     *  ���ô���2 / 3 / 4 / 5 / 7 / 8 ��ʱ��Դ����: PCLK1 = 120Mhz
     *  USB������Ҫ48MHz��ʱ��,������PLL1Q,PLL3Q��HSI48�ṩ,��������ʱ��Դ��HSI48
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART1
                                               | RCC_PERIPHCLK_USB  | RCC_PERIPHCLK_OSPI
											   | RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.PLL2.PLL2M = 25;
    PeriphClkInitStruct.PLL2.PLL2N = 266;
    PeriphClkInitStruct.PLL2.PLL2P = 2;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 1;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
		
    PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
    PeriphClkInitStruct.OspiClockSelection = RCC_OSPICLKSOURCE_D1HCLK;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if(ret != HAL_OK)
    {
        return 1;
    }

    sys_ospi_enable_memmapmode(0);          /* ʹ��QSPI�ڴ�ӳ��ģʽ, FLASH����Ϊ��ͨ���� */

    HAL_PWREx_EnableUSBVoltageDetector();   /* ʹ��USB��ѹ��ƽ����� */
    __HAL_RCC_CSI_ENABLE() ;                /* ʹ��CSIʱ�� */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;         /* ʹ��SYSCFGʱ�� */
    HAL_EnableCompensationCell();           /* ʹ��IO������Ԫ */
    return 0;
}


#ifdef  USE_FULL_ASSERT

/**
 * @brief       ��������ʾ�����ʱ��˺����������������ļ���������
 * @param       file��ָ��Դ�ļ�
 *              line��ָ�����ļ��е�����
 * @retval      ��
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    while (1)
    {
    }
}
#endif

/**
 * @brief       OSPI�����ڴ�ӳ��ģʽ��ִ��OSPI����ر�ǰ�ᣩ
 *   @note      ���������ʹ��OSPI FLASH������������ȷ��ftypeֵ!
 * @param       ftype: flash����
 *   @arg           0, ��ͨFLASH, ������128Mbit�����ڵ�
 *   @arg           1, ������FLASH, ������256Mbit�����ϵ�.
 * @retval      ��
 */
void sys_ospi_enable_memmapmode(uint8_t ftype)
{
    uint32_t tempreg = 0; 
    GPIO_InitTypeDef ospi_gpio;
	OSPI_HandleTypeDef hospi1;
	OSPIM_CfgTypeDef sOspiManagerCfg = {0};
	OSPI_RegularCmdTypeDef     sCommand;         // QSPI��������
    OSPI_MemoryMappedTypeDef   sMemMappedCfg;    // �ڴ�ӳ����ʲ���
	
	uint32_t i;
	char *p;
	
	__HAL_RCC_OCTOSPIM_CLK_ENABLE();    // ʹ��OSPI���ŷ��������ʱ��
    __HAL_RCC_OSPI1_CLK_ENABLE();       /* QSPIʱ��ʹ�� */
	
	__HAL_RCC_OSPI1_FORCE_RESET();		// ��λ
	__HAL_RCC_OSPI1_RELEASE_RESET();	
	
	__HAL_RCC_GPIOB_CLK_ENABLE();                            /* ʹ��PORTBʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();                            /* ʹ��PORTDʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();                            /* ʹ��PORTEʱ�� */
	
	ospi_gpio.Pin = GPIO_PIN_6;                              /* PB6 AF10 */
    ospi_gpio.Mode = GPIO_MODE_AF_PP;
    ospi_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    ospi_gpio.Pull = GPIO_PULLUP;
    ospi_gpio.Alternate = GPIO_AF10_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PB2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; /* PD11,12,13 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOD, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PE2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOE, &ospi_gpio);

	/*��ĳЩ���ϣ��������������㷨ʱ����Ҫ�ֶ��������Ȳ���������Ṥ��������*/	
/* �˴��ο��������Ĵ��룬��ҿ���ȥѧϰ�������Ľ̳̣��ǳ�רҵ */
	p = (char *)&hospi1;
	for (i = 0; i < sizeof(hospi1); i++)
	{
		*p++ = 0;
	}	
	
	HAL_OSPI_DeInit(&hospi1);           // ��λOSPI

	hospi1.Instance 							= OCTOSPI1;								// OSPI����

	hospi1.Init.ClockPrescaler 			= 2;                                      // ʱ�ӷ�Ƶֵ����OSPI�ں�ʱ�ӽ��� 2 ��Ƶ�õ�OSPIͨ������ʱ��
	hospi1.Init.FifoThreshold 				= 8;                                     	// FIFO��ֵ
	hospi1.Init.DualQuad 					= HAL_OSPI_DUALQUAD_DISABLE;              // ��ֹ˫OSPIģʽ
	hospi1.Init.MemoryType 					= HAL_OSPI_MEMTYPE_MICRON;                // �洢��ģʽ��ֻ��8��ģʽ�Ż��õ�
	hospi1.Init.DeviceSize 					= 23;					                        // flash��С�����İ������8M�ֽڵ�W25Q64����������Ϊ23����2^23					         
	hospi1.Init.ChipSelectHighTime 		= 1;                                      // Ƭѡ���ָߵ�ƽ��ʱ��
	hospi1.Init.FreeRunningClock 			= HAL_OSPI_FREERUNCLK_DISABLE;            // ��ֹ����ʱ��ģʽ
	hospi1.Init.ClockMode 					= HAL_OSPI_CLOCK_MODE_3;                  // ģʽ3
	hospi1.Init.WrapSize						= HAL_OSPI_WRAP_NOT_SUPPORTED;            // ��ʹ�� wrap-size
	hospi1.Init.SampleShifting 			= HAL_OSPI_SAMPLE_SHIFTING_HALFCYCLE;     // ���CLK����֮����в���
	hospi1.Init.DelayHoldQuarterCycle	= HAL_OSPI_DHQC_DISABLE;                  // ��ʹ�����ݱ��ֹ���
	hospi1.Init.ChipSelectBoundary 		= 0;											      // ��ֹƬѡ�߽繦��
	hospi1.Init.ClkChipSelectHighTime	= 0;                                      // ͨ�Ž����� 0 ��CLK����CS����Ϊ��
	hospi1.Init.DelayBlockBypass 			= HAL_OSPI_DELAY_BLOCK_BYPASSED;          // ��ʱģ����·
   hospi1.Init.MaxTran                 = 0;                                      // ��ֹͨ�Ź�����
   hospi1.Init.Refresh                 = 0;                                      // ��ֹˢ�¹���

	HAL_OSPI_Init(&hospi1);    // ��ʼ�� OSPI1 ����
	
	sOspiManagerCfg.ClkPort = 1;     // OSPI���ŷ�����������ã�ʹ�� Port1 �� CLK
	sOspiManagerCfg.NCSPort = 1;     // OSPI���ŷ�����������ã�ʹ�� Port1 �� NCS
	sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;   // OSPI���ŷ�����������ã�ʹ�� Port1 �ĵ�4λ���ţ�IO[3:0]

	HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);  // ��ʼ��OSPI���ŷ������������

	sCommand.OperationType           = HAL_OSPI_OPTYPE_COMMON_CFG;             // ͨ������
   sCommand.FlashId                 = HAL_OSPI_FLASH_ID_1;                    // flash ID

   sCommand.Instruction             = 0xEB;             // 1-4-4ģʽ��(1��ָ��4�ߵ�ַ4������)�����ٶ�ȡָ��
   sCommand.InstructionMode         = HAL_OSPI_INSTRUCTION_1_LINE;            // 1��ָ��ģʽ
   sCommand.InstructionSize         = HAL_OSPI_INSTRUCTION_8_BITS;            // ָ���8λ
   sCommand.InstructionDtrMode      = HAL_OSPI_INSTRUCTION_DTR_DISABLE;       // ��ָֹ��DTRģʽ

   sCommand.AddressMode             = HAL_OSPI_ADDRESS_4_LINES;               // 4�ߵ�ַģʽ
   sCommand.AddressSize             = HAL_OSPI_ADDRESS_24_BITS;               // ��ַ����24λ
   sCommand.AddressDtrMode          = HAL_OSPI_ADDRESS_DTR_DISABLE;           // ��ֹ��ַDTRģʽ

   sCommand.AlternateBytesMode      = HAL_OSPI_ALTERNATE_BYTES_NONE;          // �޽����ֽ�    
   sCommand.AlternateBytesDtrMode   = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;   // ��ֹ���ֽ�DTRģʽ 

   sCommand.DataMode                = HAL_OSPI_DATA_4_LINES;                  // 4������ģʽ
   sCommand.DataDtrMode             = HAL_OSPI_DATA_DTR_DISABLE;              // ��ֹ����DTRģʽ 

   sCommand.DummyCycles             = 6;                                      // �����ڸ���
   sCommand.DQSMode                 = HAL_OSPI_DQS_DISABLE;                   // ��ʹ��DQS 
   sCommand.SIOOMode                = HAL_OSPI_SIOO_INST_EVERY_CMD;           // ÿ�δ������ݶ�����ָ��   

	HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);

	sMemMappedCfg.TimeOutActivation  = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;          // ���ó�ʱ������, nCS ���ּ���״̬
	sMemMappedCfg.TimeOutPeriod      = 0;                                         // ��ʱ�ж�����
   // �����ڴ�ӳ��ģʽ
	HAL_OSPI_MemoryMapped(&hospi1,  &sMemMappedCfg);	// ��������
	
//	/* QSPI���ã��ο�QSPIʵ���QSPI_Init���� */
//	RCC->AHB3RSTR |= 1 << 14;       /* ��λQSPI */
//    RCC->AHB3RSTR &= ~(1 << 14);    /* ֹͣ��λQSPI */
//	
//	while (OCTOSPI1->SR & (1 << 5)); /* �ȴ�BUSYλ���� */
//	
//	/* QSPIʱ��Դ�Ѿ���sys_stm32_clock_init()���������� */
//    OCTOSPI1->CR = 0X01000310;       /* ����CR�Ĵ���, ��Щֵ��ô���ģ���ο�QSPIʵ��/��H750�ο��ֲ�Ĵ����������� */
//    OCTOSPI1->DCR1 = 0X00180201;      /* ����DCR�Ĵ���(FLASH����32M(�����������Ϊ32M, Ĭ����16M��), tSHSL = 3��ʱ��) */
//    OCTOSPI1->CR |= 1 << 0;          /* ʹ��QSPI */
}

//�Ĵ����汾
void sys_ospi_enable_memmapmode1(uint8_t ftype)
{
    uint32_t tempreg = 0; 
    GPIO_InitTypeDef ospi_gpio;

	uint32_t i;
	char *p;
	
	__HAL_RCC_OCTOSPIM_CLK_ENABLE();    // ʹ��OSPI���ŷ��������ʱ��
    __HAL_RCC_OSPI1_CLK_ENABLE();       /* QSPIʱ��ʹ�� */
	
	__HAL_RCC_OSPI1_FORCE_RESET();		// ��λ
	__HAL_RCC_OSPI1_RELEASE_RESET();	
	
	__HAL_RCC_GPIOB_CLK_ENABLE();                            /* ʹ��PORTBʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();                            /* ʹ��PORTDʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();                            /* ʹ��PORTEʱ�� */
	
	ospi_gpio.Pin = GPIO_PIN_6;                              /* PB6 AF10 */
    ospi_gpio.Mode = GPIO_MODE_AF_PP;
    ospi_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    ospi_gpio.Pull = GPIO_PULLUP;
    ospi_gpio.Alternate = GPIO_AF10_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PB2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOB, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; /* PD11,12,13 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOD, &ospi_gpio);

    ospi_gpio.Pin = GPIO_PIN_2;                              /* PE2 AF9 */
    ospi_gpio.Alternate = GPIO_AF9_OCTOSPIM_P1;
    HAL_GPIO_Init(GPIOE, &ospi_gpio);
	
//	/* QSPI���ã��ο�QSPIʵ���QSPI_Init���� */
	RCC->AHB3RSTR |= 1 << 14;       /* ��λQSPI */
    RCC->AHB3RSTR &= ~(1 << 14);    /* ֹͣ��λQSPI */
//	
	while (OCTOSPI1->SR & (1 << 5)); /* �ȴ�BUSYλ���� */
	
//	
//	/* QSPIʱ��Դ�Ѿ���sys_stm32_clock_init()���������� */
//    OCTOSPI1->CR = 0X01000310;       /* ����CR�Ĵ���, ��Щֵ��ô���ģ���ο�QSPIʵ��/��H750�ο��ֲ�Ĵ����������� */
//    OCTOSPI1->DCR1 = 0X00180201;      /* ����DCR�Ĵ���(FLASH����32M(�����������Ϊ32M, Ĭ����16M��), tSHSL = 3��ʱ��) */
//    OCTOSPI1->CR |= 1 << 0;          /* ʹ��QSPI */
}


